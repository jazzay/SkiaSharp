/*
* Copyright 2015 Xamarin Inc.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

// EXPERIMENTAL EXPERIMENTAL EXPERIMENTAL EXPERIMENTAL
// DO NOT USE -- FOR INTERNAL TESTING ONLY


#ifndef sk_app_DEFINED
#define sk_app_DEFINED

#include "sk_xamarin.h"
#include "sk_types.h"


SK_C_PLUS_PLUS_BEGIN_GUARD

typedef struct sk_app_t sk_app_t;

typedef void(*sk_application_idle_delegate)        (sk_app_t* capp);

SK_X_API sk_app_t* sk_application_new();
SK_X_API void sk_application_run(sk_app_t* capp);
SK_X_API void sk_application_destroy(sk_app_t* capp);

SK_X_API void sk_application_set_idle_delegate(const sk_application_idle_delegate delegate);


SK_C_PLUS_PLUS_END_GUARD

#endif
