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


SK_X_API sk_window_t* sk_window_new();
SK_X_API void sk_window_destroy(sk_window_t*);

SK_X_API void sk_application_init();
SK_X_API void sk_application_run();
SK_X_API void sk_application_term();

SK_X_API sk_window_t* sk_create_main_window();

SK_C_PLUS_PLUS_END_GUARD

#endif
