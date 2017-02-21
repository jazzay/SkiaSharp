/*
* Copyright 2016 Xamarin Inc.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

#include "SkWindow.h"
#include "sk_window.h"
#include "sk_types_priv.h"
#include "SkGraphics.h"
#include "SkNativeWindow.h"

static inline sk_window_t* ToWindow(SkWindow* canvas) {
	return reinterpret_cast<sk_window_t*>(canvas);
}

static inline SkWindow* AsWindow(sk_window_t* ccanvas) {
	return reinterpret_cast<SkWindow*>(ccanvas);
}

sk_window_t* sk_window_new() {
	return ToWindow(new SkWindow());
}

void sk_window_destroy(sk_window_t* cwindow) {
	delete AsWindow(cwindow);
}

void sk_application_init() {
	SkGraphics::Init();
	SkEvent::Init();
}

void sk_application_run() {

	// WINDOWS only here for now. make this XPLAT
	MSG msg;
	// Main message loop
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (true) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}

void sk_application_term() {
	SkEvent::Term();
}

sk_window_t* sk_create_main_window() {

	void* init = SkNativeWindow::InitializeWindowClass();

	auto window = new SkNativeWindow(init);
	SkNativeWindow::ShowAllWindows();

	return ToWindow(window);
}

/*
// TEMP - to satisfy linking in main methods
#include "SkWindow.h"
#include "SkGraphics.h"

void application_init() {
	SkGraphics::Init();
	SkEvent::Init();
}

void application_term() {
	SkEvent::Term();
}

SkOSWindow* create_sk_window(void* hwnd, int, char**) {
	return nullptr;
}
*/
