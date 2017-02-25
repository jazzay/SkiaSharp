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

typedef struct sk_app_t sk_app_t;
typedef struct sk_window_t sk_window_t;

typedef void(*sk_window_paint_delegate)        (sk_window_t* cwindow, sk_canvas_t* ccanvas);

SK_X_API sk_window_t* sk_window_new();
SK_X_API void sk_window_destroy(sk_window_t*);
SK_X_API void sk_window_set_paint_delegate(const sk_window_paint_delegate delegate);

SK_X_API void sk_application_init();
SK_X_API void sk_application_run(sk_app_t* capp);
SK_X_API void sk_application_term();

SK_C_PLUS_PLUS_END_GUARD

#endif
