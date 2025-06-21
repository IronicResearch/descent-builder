/* DRM test for creating page-flip capable framebuffer context */

// export CFLAGS="-g -O0 -I/usr/local/include/ -I/usr/local/include/libdrm/"
// gcc $CFLAGS -o drmtest drmtest.c -ldrm

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
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
