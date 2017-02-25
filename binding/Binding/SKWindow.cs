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
	public enum NativeRenderBackend {
		Raster = 0,		// CPU
		OpenGL = 1,		// GPU
		Vulkan = 2		// GPU - Not available on some platforms, falls back to Raster if not
	};

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

		public SKWindow (NativeRenderBackend renderBackend = NativeRenderBackend.Raster)
			: this (IntPtr.Zero, true)
		{
			Handle = SkiaApi.sk_window_new ((int)renderBackend);
		}

		protected override void Dispose (bool disposing)
		{
			if (Handle != IntPtr.Zero && OwnsHandle) {
				SkiaApi.sk_window_destroy (Handle);
			}

			base.Dispose (disposing);
		}

		public int Width
		{
			get { return SkiaApi.sk_window_get_width (Handle); }
		}

		public int Height
		{
			get { return SkiaApi.sk_window_get_height (Handle); }
		}

		public string Title
		{
			set
			{
				var bytes = StringUtilities.GetEncodedText (value, SKTextEncoding.Utf8);
				SkiaApi.sk_window_set_title(Handle, bytes, bytes.Length);
			}
		}

		public void Invalidate()
		{
			SkiaApi.sk_window_invalidate(Handle);
		}

		protected virtual void OnPaint(SKCanvas canvas)
		{
		}

		private void HandlePaint(SKCanvas canvas)
		{
			OnPaint(canvas);
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
}