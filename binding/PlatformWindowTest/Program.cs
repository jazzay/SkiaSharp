using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SkiaSharp;

namespace PlatformWindowTest
{
    class Program
    {
        static void Main(string[] args)
        {
            SKApplication.Init();

            var window = new MainWindow();
			window.Title = "SkiaSharp Windows";

			// this runs the native message loop
            SKApplication.Run();

            SKApplication.Terminate();
        }
    }

	public class MainWindow : SKWindow
	{
		protected override void HandlePaint(SKCanvas canvas)
		{
			base.HandlePaint(canvas);

			var bgColor = new SKColor(80, 80, 255, 255);
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
