//
// Bindings for SKCanvas
//
// Author:
//   Miguel de Icaza
//
// Copyright 2016 Xamarin Inc
//
using System;

namespace SkiaSharp
{
	// No dispose, the Canvas is only valid while the Surface is valid.
	public class SKWindow : SKObject
	{
		[Preserve]
		internal SKWindow (IntPtr handle, bool owns)
			: base (handle, owns)
		{
		}

		public SKWindow ()
			: this (IntPtr.Zero, true)
		{
			Handle = SkiaApi.sk_window_new ();
		}

		protected override void Dispose (bool disposing)
		{
			if (Handle != IntPtr.Zero && OwnsHandle) {
				SkiaApi.sk_window_destroy (Handle);
			}

			base.Dispose (disposing);
		}
    }

    public class SKApplication //: SKObject
    {
        public static void Init()
        {
            SkiaApi.sk_application_init();
        }

        public static void Terminate()
        {
            SkiaApi.sk_application_term();
        }

        public static void Run()
        {
            SkiaApi.sk_application_run();
        }

        public static SKWindow CreateMainWindow()
        {
            var nativeWnd = SkiaApi.sk_create_main_window();
            return new SKWindow(nativeWnd, false);  // do we own this window now?
        }
    }
}