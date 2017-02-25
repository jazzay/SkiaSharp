/*
* Copyright 2015 Xamarin Inc.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

// EXPERIMENTAL EXPERIMENTAL EXPERIMENTAL EXPERIMENTAL
// DO NOT USE -- FOR INTERNAL TESTING ONLY


#ifndef sk_window_DEFINED
#define sk_window_DEFINED

#include "sk_xamarin.h"
#include "sk_types.h"


SK_C_PLUS_PLUS_BEGIN_GUARD

typedef struct sk_window_t sk_window_t;

typedef void(*sk_window_paint_delegate)        (sk_window_t* cwindow, sk_canvas_t* ccanvas);


SK_X_API sk_window_t* sk_window_new(int renderBackend);
SK_X_API void sk_window_destroy(sk_window_t*);
SK_X_API void sk_window_set_paint_delegate(const sk_window_paint_delegate delegate);
SK_X_API int sk_window_get_width(sk_window_t* cwindow);
SK_X_API int sk_window_get_height(sk_window_t* cwindow);
SK_X_API void sk_window_set_title(sk_window_t* cwindow, const char *title, size_t byteLength);
SK_X_API void sk_window_invalidate(sk_window_t* cwindow);

SK_C_PLUS_PLUS_END_GUARD

#endif
