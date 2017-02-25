//
// Bindings for SKCanvas
//
// Author:
//   Miguel de Icaza
//
// Copyright 2016 Xamarin Inc
//
using System;
using System.Runtime.InteropServices;

namespace SkiaSharp
{
	// No dispose, the Canvas is only valid while the Surface is valid.
	public class SKWindow : SKObject
	{
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		internal delegate void  paint_delegate (IntPtr windowPtr, IntPtr canvas);

		// delegate fields
		private static readonly paint_delegate fPaint;


		static SKWindow()
		{
			fPaint = new paint_delegate(PaintInternal);
			SkiaApi.sk_window_set_paint_delegate(Marshal.GetFunctionPointerForDelegate(fPaint));
		}

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

		protected virtual void HandlePaint(SKCanvas canvas)
		{
			// Fill with some nice default color
			canvas.Clear(SKColors.CornflowerBlue);
		}

		#if __IOS__
		[MonoPInvokeCallback(typeof(read_delegate))]
		#endif
		private static void PaintInternal (IntPtr windowPtr, IntPtr canvasPtr)
		{
			var window = GetObject<SKWindow> (windowPtr);
			var canvas = GetObject<SKCanvas> (canvasPtr, false);
			if(window != null && canvas != null)
			{
				window.HandlePaint (canvas);
			}
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