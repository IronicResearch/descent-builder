/* DRM test for creating page-flip capable framebuffer context */

// export CFLAGS="-g -O0 -I/usr/local/include/ -I/usr/local/include/libdrm/"
// gcc $CFLAGS -o drmtest drmtest.c -ldrm

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

int make_drm_context(void)
{
	int r = 0;
	int fd = 0;
	drmModeRes* 		resources = NULL;
	drmModeConnector* 	connector = NULL;
	drmModeModeInfo*	modeinfo = NULL;
	drmModeEncoder*		encoder = NULL;
	drmModeCrtc*		crtc = NULL;
	drmModeModeInfo 	reqmode = { .hdisplay = 640, .vdisplay = 480, .vrefresh = 85 };
	void* fbmem = NULL;
	uint32_t fbbuf_id = 0;

	fd = open("/dev/dri/card0", O_RDWR);
	if (fd < 0) {
		r = errno;
		printf("unable to open DRM connection, error = %d\n", r);
		return r;
	}

	printf("DRM open returned = %d\n", fd);

	resources = drmModeGetResources(fd);
	if (resources == NULL) {
		r = errno;
		printf("no resources for DRM fd %d, error = %d\n", fd, r);
		goto x1;
	}
	
	connector = drmModeGetConnector(fd, resources->connectors[0]);
	if (connector == NULL) {
		r = errno;
		printf("no connector for DRM fd %d, error = %d\n", fd, r);
		goto x2;
	}

	for (int i = 0; i < connector->count_modes; i++) {
		modeinfo = &connector->modes[i];
		printf("%d: %dx%d @%d\n", i, 
			modeinfo->hdisplay, 
			modeinfo->vdisplay,
			modeinfo->vrefresh);
		if (modeinfo->hdisplay == reqmode.hdisplay
			&& modeinfo->vdisplay == reqmode.vdisplay
			&& modeinfo->vrefresh >= reqmode.vrefresh)
				reqmode = *modeinfo;
	}

	encoder = drmModeGetEncoder(fd, connector->encoder_id);
	if (encoder == NULL) {
		r = errno;
		printf("no encoder for DRM fd %d, error = %d\n", fd, r);
		goto x3;
	}

	crtc = drmModeGetCrtc(fd, encoder->crtc_id);
	if (crtc == NULL) {
		r = errno;
		printf("no crtc for DRM fd %d, error = %d\n", fd, r);
		goto x4;
	}

	printf("CRTC ID %d: %d,%d %dx%d\n", crtc->crtc_id, crtc->x, crtc->y, crtc->width, crtc->height);
	printf("CRTC mode: %dx%d @%d\n", crtc->mode.hdisplay, crtc->mode.vdisplay, crtc->mode.vrefresh);

	// create dumb framebuffer via low-level API
	struct drm_mode_create_dumb request = { .width = 640, .height = 480, .bpp = 32 };
	r = ioctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &request);
	if (r != 0) {
		printf("no framebuffer creation for DRM fd %d, error = %d\n", fd, r);
		goto x5;
	}

	// request framebuffer offset for mmap() mapping
	struct drm_mode_map_dumb req_map = { .handle = request.handle };
	r = ioctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &req_map);
	if (r != 0) {
		printf("no framebuffer mapping for DRM fd %d, error = %d\n", fd, r);
		goto x5;
	}

	// memory-map framebuffer for userspace access
	fbmem = mmap(NULL, request.size, PROT_READ | PROT_WRITE, MAP_SHARED, 
		fd, req_map.offset);
	if (fbmem == NULL) {
		r = errno;
		printf("no framebuffer mapping for DRM fd %d, error = %d\n", fd, r);
		goto x6;
	}

	// fill framebuffer with test pattern
	memset(fbmem, 0x55, request.size);

	// add framebuffer for CRTC mode setting
	r = drmModeAddFB(fd, request.width, request.height, 24,
			request.bpp, request.pitch, request.handle, &fbbuf_id);
	if (r != 0) {
		printf("no framebuffer binding for DRM fd %d, error = %d\n", fd, r);
		goto x7;
	}

	// switch to framebuffer test mode setting
	drmModeSetCrtc(fd, crtc->crtc_id, fbbuf_id, 0, 0,
		&connector->connector_id, 1, &reqmode);

	printf("CRTC mode: %dx%d @%d\n", reqmode.hdisplay, reqmode.vdisplay, reqmode.vrefresh);
	sleep(10);

	// restore original CRTC mode
	drmModeSetCrtc(fd, crtc->crtc_id, crtc->buffer_id, crtc->x, crtc->y, 
		&connector->connector_id, 1, &crtc->mode);

	// destroy framebuffer binding + mapping
	drmModeRmFB(fd, fbbuf_id);
x7:
	munmap(fbmem, request.size);
x6:
	ioctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &req_map);
x5:
	drmModeFreeCrtc(crtc);
x4:
	drmModeFreeEncoder(encoder);
x3:
	drmModeFreeConnector(connector);
x2:
	drmModeFreeResources(resources);
x1:
	close(fd);

	return r;
}

int main(int argc, char* argv[])
{
	return make_drm_context();
}
