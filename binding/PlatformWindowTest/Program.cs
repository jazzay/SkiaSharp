using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SkiaSharp;
using System.Threading;

namespace PlatformWindowTest
{
    class Program
    {
        static void Main(string[] args)
        {
            var app = new SKApplication();

            var window = new MainWindow();
			window.Title = "SkiaSharp Windows";

			// this runs the native message loop
            app.Run();
        }
    }

	public class MainWindow : SKWindow
	{
		DateTime mStartTime = DateTime.Now;

		public MainWindow()
		{
			SKApplication.Current.Idle += (s, e) => Invalidate();
		}

		protected override void OnPaint(SKCanvas canvas)
		{
			base.OnPaint(canvas);

			// some basic color animation
			var totalElapsed = (DateTime.Now - mStartTime).TotalMilliseconds;

			var modulator = Math.Sin(totalElapsed * 0.005) * 0.5 + 0.5;
			var bgColor = new SKColor(80, (byte) (modulator * 128), 255, 255);
			canvas.Clear(bgColor);

			var centerX = this.Width / 2;
			var centerY = this.Height / 2;

			var paint = new SKPaint
			{
				IsAntialias = true,
				Style = SKPaintStyle.Fill,
				Color = bgColor.WithBlue(80)
			};
			//canvas.DrawRect(new SKRect(centerX - 200, centerY - 100, centerX + 200, centerY + 100), paint);

			paint.Typeface = SKTypeface.FromFamilyName("Arial");
			paint.TextSize = 72.0f;
			paint.Color = SKColors.Black;

			var message = "Hello World!";
			var textWidth = paint.MeasureText(message);
			canvas.DrawText(message, centerX - (textWidth / 2), centerY - 10, paint);
		}
	}
}
