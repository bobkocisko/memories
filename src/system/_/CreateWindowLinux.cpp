//
// Created by bob on 4/24/17.
//


#include "system/_/CreateWindowLinux.h"

#include <GL/gl3w.h>
#include <GL/glx.h>

namespace memories {

const std::size_t ErrorDescriptionBufferSize = 1000;
char LastXErrorDescription[ErrorDescriptionBufferSize];

int XErrorHandler(Display *display, XErrorEvent *errorEvent) {
  XGetErrorText(display, errorEvent->error_code, LastXErrorDescription, ErrorDescriptionBufferSize);

  return 0;
}

static Bool WaitForNotify(Display *dpy, XEvent *event, XPointer arg) {
  return (event->type == MapNotify) && (event->xmap.window == (::Window) arg);
}

/**
 * Thanks to https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glXIntro.xml
 * and https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
 */
std::shared_ptr<Window> CreateWindowLinux::Create() {
  XSetErrorHandler(XErrorHandler);

  Display *display;
  if (!(display = XOpenDisplay(nullptr))) {
    throw std::runtime_error(std::string("XOpenDisplay failed: ") + LastXErrorDescription);
  }

  int defaultScreen = DefaultScreen(display);

  // Get a matching FB config
  static int visual_attribs[] =
      {
          GLX_X_RENDERABLE, True,
          GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
          GLX_RENDER_TYPE, GLX_RGBA_BIT,
          GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
          GLX_RED_SIZE, 8,
          GLX_GREEN_SIZE, 8,
          GLX_BLUE_SIZE, 8,
          GLX_ALPHA_SIZE, 8,
          GLX_DEPTH_SIZE, 24,
          GLX_STENCIL_SIZE, 8,
          // We want double-buffers but we don't want SwapBuffers because that
          // makes the graphics updates occur a frame later than the mouse
          // movements.  So we create a separate 'back buffer' and
          // blit from that to the front buffer as needed.
          GLX_DOUBLEBUFFER, False,
          None
      };

  int fbcount;
  GLXFBConfig *glxfbConfig;
  if (!(glxfbConfig = glXChooseFBConfig(display, defaultScreen, visual_attribs, &fbcount))) {
    throw std::runtime_error("glXChooseFBConfig: could not find an X server visual "
                                 "matching the attributes required by this application");
  }

  // Any fb config that matches the criteria should work fine, so let's pick the first one
  GLXFBConfig selectedFbConfig = glxfbConfig[0];
  XFree(glxfbConfig);

  XVisualInfo *xVisualInfo;
  if (!(xVisualInfo = glXGetVisualFromFBConfig(display, selectedFbConfig))) {
    throw std::runtime_error("glXGetVisualFromFBConfig failed");
  }

  ::Window rootWindow = RootWindow(display, xVisualInfo->screen);

  XSetWindowAttributes xSetWindowAttributes;
  // TODO: XFreeColormap()
  Colormap colormap; // Save this for later XFreeColormap()
  xSetWindowAttributes.event_mask = StructureNotifyMask;
  xSetWindowAttributes.colormap = colormap = XCreateColormap(display, rootWindow, xVisualInfo->visual, AllocNone);

  unsigned int windowMask = CWBorderPixel | CWColormap | CWEventMask;

  ::Window window;
  unsigned int x = 0, y = 0, width = 100, height = 100, borderWidth = 1;
  if (!(window = XCreateWindow(display, rootWindow, x, y, width, height, borderWidth, xVisualInfo->depth,
                               InputOutput, xVisualInfo->visual, windowMask, &xSetWindowAttributes))) {
    throw std::runtime_error(std::string("XCreateWindow failed: ") + LastXErrorDescription);
  }

  GLXContext glxContext;
  if (!(glxContext = glXCreateNewContext(display, selectedFbConfig, GLX_RGBA_TYPE, nullptr, True))) {
    throw std::runtime_error(std::string("glXCreateNewContext failed: ") + LastXErrorDescription);
  }

  XFree(xVisualInfo);

  GLXWindow glxWindow;
  if (!(glxWindow = glXCreateWindow(display, selectedFbConfig, window, nullptr))) {
    throw std::runtime_error(std::string("glXCreateWindow failed: ") + LastXErrorDescription);
  }

  if (!XStoreName(display, window, "memories")) // Window title
  {
    throw std::runtime_error(std::string("XStoreName failed: ") + LastXErrorDescription);
  }

  // Display the window...
  if (!XMapWindow(display, window)) {
    throw std::runtime_error(std::string("XMapWindow failed: ") + LastXErrorDescription);
  }

  // ...and wait for the window to appear
  XEvent event;
  XIfEvent(display, &event, WaitForNotify, (XPointer) window);

  if (!glXMakeContextCurrent(display, glxWindow, glxWindow, glxContext)) {
    throw std::runtime_error(std::string("glXMakeContextCurrent failed: ") + LastXErrorDescription);
  }

  if (gl3wInit())
  {
    throw std::runtime_error("Failed to load OpenGL functions");
  }

  if (!gl3wIsSupported(3, 0)) {
    throw std::runtime_error(std::string("Expected OpenGL version 3.0 "
                                             "or greater; instead found version ") +
        (char *) glGetString(GL_VERSION));
  }

  glClearColor(1.0, 0.5, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glFlush();

  // TODO: return WindowLinux
  return nullptr;
}

}