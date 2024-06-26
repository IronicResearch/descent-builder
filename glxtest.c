/* GLX test for creating stereo-capable visual context */
/* gcc -o glxtest glxtest.c -lGL -lX11 */

#include <stdio.h>
#include <errno.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

int make_glx_context(void)
{
	Display *dpy;
	int scrn;
	int attr[] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_STEREO,
		None
	};
	XVisualInfo *vis;
	GLXContext ctx;
	GLboolean stereo, db;
	Window win;
	const GLubyte* str;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		printf("unable to open X display connection\n");
		return -ENODEV;
	}
	scrn = XDefaultScreen(dpy);
	vis = glXChooseVisual(dpy, scrn, attr);
	if (vis == NULL) {
		printf("unable to find GLX_STEREO visual\n");
		attr[2] = None;
		vis = glXChooseVisual(dpy, scrn, attr);
		if (vis == NULL) {
			printf("unable to find GLX_DOUBLE_BUFFER visual\n");
			return -EINVAL;
		}
	}

	ctx = glXCreateContext(dpy, vis, NULL, GL_TRUE);
	if (ctx == NULL) {
		printf("unable to create GLX context\n");
		return -ENOTSUP;
	}

	win = XCreateSimpleWindow(dpy, RootWindow(dpy, scrn), 0, 0, 200, 200, 0, 0, 0);
	glXMakeCurrent(dpy, win, ctx);

	glGetBooleanv(GL_STEREO, &stereo);
	printf("GL_STEREO support = %d\n", (int)stereo);
	glGetBooleanv(GL_DOUBLEBUFFER, &db);
	printf("GL_DOUBLEBUFFER   = %d\n", (int)db);
	str = glGetString(GL_VERSION);
	printf("GL_VERSION        = %s\n", str);
	str = glGetString(GL_RENDERER);
	printf("GL_RENDERER       = %s\n", str);

	if (stereo) {
		glDrawBuffer(GL_BACK_LEFT);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawBuffer(GL_BACK_RIGHT);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glXSwapBuffers(dpy, win);
	} else {
		glDrawBuffer(GL_BACK);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glXSwapBuffers(dpy, win);
	}

	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, ctx);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);

	return 0;
}

int main(int argc, char* argv[])
{
	return make_glx_context();
}
