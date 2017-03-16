/*
* Copyright 2016 Xamarin Inc.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

#if defined(SK_BUILD_FOR_WIN32)
#include <windows.h>
#include <tchar.h>
#endif

#include "sk_window.h"
#include "sk_types_priv.h"
#include "SkGraphics.h"

// may not need this anymore as new window model is more xplat out of the box
//#include "SkNativeWindow.h"

#include "sk_app/Window.h"

typedef sk_app::Window SkWindow;


static inline sk_window_t* ToWindow(SkWindow* window) {
	return reinterpret_cast<sk_window_t*>(window);
}

static inline SkWindow* AsWindow(sk_window_t* cwindow) {
	return reinterpret_cast<SkWindow*>(cwindow);
}

// paint handler signature

static sk_window_paint_delegate paint_delegate = nullptr;
static sk_window_mouse_delegate mouse_delegate = nullptr;

void native_paint_handler(SkCanvas* canvas, void* userData)
{
	if (paint_delegate != nullptr)
	{
		sk_window_t* cwindow = (sk_window_t*)userData;
		sk_canvas_t* ccanvas = ToCanvas(canvas);
		paint_delegate(cwindow, ccanvas);
	}
}

bool native_mouse_handler(int x, int y, SkWindow::InputState state, uint32_t modifiers, void* userData)
{
	if (mouse_delegate != nullptr)
	{
		sk_window_t* cwindow = (sk_window_t*)userData;
		return mouse_delegate(cwindow, x, y, state, modifiers);
	}
	return false;
}

sk_window_t* sk_window_new(int renderBackend) {

#if defined(SK_BUILD_FOR_WIN32)
	void* platformData = GetModuleHandle(NULL);
#else
	void* platformData = nullptr;
#endif

	SkWindow* window = SkWindow::CreateNativeWindow(platformData);

	sk_app::Window::BackendType backend = sk_app::Window::kRaster_BackendType;
	if (renderBackend == 1)
	{
		backend = sk_app::Window::kNativeGL_BackendType;
	}
#ifdef SK_VULKAN
	else if (renderBackend == 2)
	{
		backend = sk_app::Window::kVulkan_BackendType;
	}
#endif

	window->attach(backend, sk_app::DisplayParams());

	// setup managed callbacks
	window->registerPaintFunc(native_paint_handler, ToWindow(window));
	window->registerMouseFunc(native_mouse_handler, ToWindow(window));

	// host should call this
	window->show();

	return ToWindow(window);
}

void sk_window_destroy(sk_window_t* cwindow) {
	auto window = AsWindow(cwindow);
	window->detach();
	delete window;
}

void sk_window_set_paint_delegate(const sk_window_paint_delegate delegate) {
	paint_delegate = delegate;
}

void sk_window_set_mouse_delegate(const sk_window_mouse_delegate delegate) {
	mouse_delegate = delegate;
}

int sk_window_get_width(sk_window_t* cwindow) {
	return AsWindow(cwindow)->width();
}

int sk_window_get_height(sk_window_t* cwindow) {
	return AsWindow(cwindow)->height();
}

void sk_window_set_title(sk_window_t* cwindow, const char *title, size_t byteLength) {
	AsWindow(cwindow)->setTitle(title);
}

void sk_window_invalidate(sk_window_t* cwindow) {
	AsWindow(cwindow)->inval();
}




