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
    public class SKApplication : SKObject
    {
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		internal delegate void  idle_delegate (IntPtr appPtr);

		// delegate fields
		private static readonly idle_delegate fIdle;

		static SKApplication()
		{
			fIdle = new idle_delegate(IdleInternal);
			SkiaApi.sk_application_set_idle_delegate(Marshal.GetFunctionPointerForDelegate(fIdle));
		}

		public static SKApplication Current
		{
			get { return sInstance; }
		}
		private static SKApplication sInstance;

		[Preserve]
		internal SKApplication (IntPtr handle, bool owns)
			: base (handle, owns)
		{
			sInstance = this;
		}

		public SKApplication ()
			: this (IntPtr.Zero, true)
		{
			Handle = SkiaApi.sk_application_new ();
		}

		protected override void Dispose (bool disposing)
		{
			if (Handle != IntPtr.Zero && OwnsHandle) {
				SkiaApi.sk_application_destroy (Handle);
			}

			base.Dispose (disposing);
		}

        public void Run()
        {
            SkiaApi.sk_application_run(Handle);
        }

		public event EventHandler Idle;

		protected virtual void OnIdle()
		{
			Idle?.Invoke(this, null);
		}

		#if __IOS__
		[MonoPInvokeCallback(typeof(read_delegate))]
		#endif
		private static void IdleInternal (IntPtr appPtr)
		{
			var app = GetObject<SKApplication> (appPtr);
			if(app != null)
			{
				app.OnIdle();
			}
		}
    }
}