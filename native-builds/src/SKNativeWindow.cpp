/*
* Copyright 2015 Google Inc.
*
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*
*/

#include "SkNativeWindow.h"

#include "gl/GrGLInterface.h"
#include "GrContext.h"
#include "SkApplication.h"
#include "SkCanvas.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkGr.h"

// These 2 methods are require to link in Skia/Views
void application_init() {
	SkGraphics::Init();
	SkEvent::Init();
}

void application_term() {
	SkEvent::Term();
}

// TODO: Should put this in Windows specific sources soon
#if defined(_WINDOWS)

#include <tchar.h>

extern LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Returns the main window Win32 class name.
const TCHAR* register_skiasharp_window_class(HINSTANCE hInstance) {
	WNDCLASSEX wcex;
	// The main window class name
	static const TCHAR gSZWindowClass[] = _T("SkiaSharpWindow");

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = gSZWindowClass;
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);

	return gSZWindowClass;
}

void* SkNativeWindow::InitializeWindowClass()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	static const TCHAR* windowClass = nullptr;
	static SkOSWindow::WindowInit winInit;
	if (windowClass == nullptr)
	{
		windowClass = register_skiasharp_window_class(hInstance);
		winInit.fInstance = hInstance;
		winInit.fClass = windowClass;
	}
	return &winInit;
}

void SkNativeWindow::ShowAllWindows()
{
	bool show = true;
	ForAllWindows([show](void* hWnd, SkOSWindow**) {
		ShowWindow((HWND)hWnd, show);
		UpdateWindow((HWND)hWnd); }
	);
}

#endif


SkNativeWindow::SkNativeWindow(void* hwnd)
	: INHERITED(hwnd) {
	fType = kGPU_DeviceType;
	fRotationAngle = 0;
	this->setTitle();
	this->setUpBackend();
}

SkNativeWindow::~SkNativeWindow() {
	tearDownBackend();
}

void SkNativeWindow::tearDownBackend() {
	SkSafeUnref(fContext);
	fContext = NULL;

	SkSafeUnref(fInterface);
	fInterface = NULL;

	fGpuSurface = nullptr;

	INHERITED::release();
}

void SkNativeWindow::setTitle() {
	SkString title("SkNativeWindow - ");
	title.appendf(fType == kRaster_DeviceType ? "Raster" : "OpenGL");
	INHERITED::setTitle(title.c_str());
}

bool SkNativeWindow::setUpBackend() {
	this->setVisibleP(true);
	this->setClipToBounds(false);

	bool result = attach(kNativeGL_BackEndType, 0 /*msaa*/, false, &fAttachmentInfo);
	if (false == result) {
		SkDebugf("Not possible to create backend.\n");
		release();
		return false;
	}

	fInterface = GrGLCreateNativeInterface();
	SkASSERT(NULL != fInterface);

	fContext = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)fInterface);
	SkASSERT(NULL != fContext);

	this->setUpGpuBackedSurface();
	return true;
}

void SkNativeWindow::setUpGpuBackedSurface() {
	fGpuSurface = this->makeGpuBackedSurface(fAttachmentInfo, fInterface, fContext);
}

void SkNativeWindow::drawContents(SkCanvas* canvas) {

	// TODO: This is just here for testing, eventually replace
	// with clean default presentatin.

	// Clear background
	canvas->drawColor(SK_ColorWHITE);

	SkPaint paint;
	paint.setColor(SK_ColorRED);

	// Draw a rectangle with red paint
	SkRect rect = SkRect::MakeXYWH(10, 10, 128, 128);
	canvas->drawRect(rect, paint);

	// Set up a linear gradient and draw a circle
	{
		SkPoint linearPoints[] = {
			{ 0, 0 },
			{ 300, 300 }
		};
		SkColor linearColors[] = { SK_ColorGREEN, SK_ColorBLACK };

		paint.setShader(SkGradientShader::MakeLinear(
			linearPoints, linearColors, nullptr, 2,
			SkShader::kMirror_TileMode));
		paint.setFlags(SkPaint::kAntiAlias_Flag);

		canvas->drawCircle(200, 200, 64, paint);

		// Detach shader
		paint.setShader(nullptr);
	}

	// Draw a message with a nice black paint.
	paint.setFlags(
		SkPaint::kAntiAlias_Flag |
		SkPaint::kSubpixelText_Flag |  // ... avoid waggly text when rotating.
		SkPaint::kUnderlineText_Flag);
	paint.setColor(SK_ColorBLACK);
	paint.setTextSize(20);

	canvas->save();

	static const char message[] = "Hello World";

	// Translate and rotate
	canvas->translate(300, 300);
	fRotationAngle += 0.2f;
	if (fRotationAngle > 360) {
		fRotationAngle -= 360;
	}
	canvas->rotate(fRotationAngle);

	// Draw the text:
	canvas->drawText(message, strlen(message), 0, 0, paint);

	canvas->restore();
}

void SkNativeWindow::draw(SkCanvas* canvas) {
	this->drawContents(canvas);

	// HACK from example. May be useful for testing?
	// Invalidate the window to force a redraw. Poor man's animation mechanism.
	//this->inval(NULL);

	if (kRaster_DeviceType == fType) {
		fRasterSurface->draw(fGpuSurface->getCanvas(), 0, 0, nullptr);
	}
	fGpuSurface->getCanvas()->flush();
	INHERITED::present();
}

void SkNativeWindow::onSizeChange() {
	this->setUpGpuBackedSurface();
}

bool SkNativeWindow::onHandleChar(SkUnichar unichar) {
	if (' ' == unichar) {
		fType = fType == kRaster_DeviceType ? kGPU_DeviceType : kRaster_DeviceType;
		tearDownBackend();
		setUpBackend();
		this->setTitle();
		this->inval(NULL);
	}
	return true;
}

SkOSWindow* create_sk_window(void* hwnd, int, char**) {
	return new SkNativeWindow(hwnd);
}
