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

// Newer windowing model which supports callbacks, good for managed interop

#include "sk_app/Application.h"
#include "sk_app/Window.h"

typedef sk_app::Window SkWindow;


// We need to define this to avoid linker error

namespace sk_app {

	Application* Application::Create(int argc, char** argv, void* platformData) {
		return nullptr;
	}
}

static inline sk_app::Application* AsApp(sk_app_t* capp) {
	return reinterpret_cast<sk_app::Application*>(capp);
}



void sk_application_init() {
	SkGraphics::Init();
}

void sk_application_run(sk_app_t* capp) {

#if defined(SK_BUILD_FOR_WIN32)

	MSG msg = { 0 };

	// Main message loop
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			AsApp(capp)->onIdle();
		}
	}
#endif

}

void sk_application_term() {
}

static inline sk_window_t* ToWindow(SkWindow* window) {
	return reinterpret_cast<sk_window_t*>(window);
}

static inline SkWindow* AsWindow(sk_window_t* cwindow) {
	return reinterpret_cast<SkWindow*>(cwindow);
}

// paint handler signature

static sk_window_paint_delegate paint_delegate = nullptr;

void native_paint_handler(SkCanvas* canvas, void* userData)
{
	if (paint_delegate != nullptr)
	{
		sk_window_t* cwindow = (sk_window_t*)userData;
		sk_canvas_t* ccanvas = ToCanvas(canvas);
		paint_delegate(cwindow, ccanvas);
	}
}

sk_window_t* sk_window_new() {

#if defined(SK_BUILD_FOR_WIN32)
	void* platformData = GetModuleHandle(NULL);
#endif

	SkWindow* window = SkWindow::CreateNativeWindow(platformData);

	// eventually we should pass these down somehow
	// GL backend does not work on Windows yet. Probably missing some global config step or  DLL
	window->attach(sk_app::Window::kRaster_BackendType, sk_app::DisplayParams());
	//window->attach(sk_app::Window::kNativeGL_BackendType, sk_app::DisplayParams());

	// setup managed callbacks
	window->registerPaintFunc(native_paint_handler, ToWindow(window));

	// host should call this
	window->show();

	return ToWindow(window);
}

void sk_window_destroy(sk_window_t* cwindow) {
	delete AsWindow(cwindow);
}

void sk_window_set_paint_delegate(const sk_window_paint_delegate delegate) {
	paint_delegate = delegate;
}

