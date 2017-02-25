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

#include "sk_application.h"
#include "sk_types_priv.h"
#include "SkGraphics.h"
#include "sk_app/Application.h"
#include "sk_app/Window.h"


typedef sk_app::Application SkApplication;

// We need to define this to avoid a linker error
namespace sk_app {

	// unused
	Application* Application::Create(int argc, char** argv, void* platformData) {
		return nullptr;
	}
}

static inline sk_app_t* ToApp(SkApplication* app) {
	return reinterpret_cast<sk_app_t*>(app);
}

static inline SkApplication* AsApp(sk_app_t* capp) {
	return reinterpret_cast<SkApplication*>(capp);
}

static sk_application_idle_delegate idle_delegate = nullptr;

class ApplicationImpl : public SkApplication
{
	// Inherited via Application
	virtual void onIdle() override
	{
		idle_delegate(ToApp(this));
	}
};

static bool isInitialized = false;

sk_app_t* sk_application_new() {
	if (isInitialized) {
		// how to throw a managed exception?
		return nullptr;
	}

	SkGraphics::Init();

	auto app = new ApplicationImpl();
	return ToApp(app);
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

void sk_application_destroy(sk_app_t* capp) {
	delete AsApp(capp);
}

void sk_application_set_idle_delegate(const sk_application_idle_delegate delegate) {
	idle_delegate = delegate;
}



