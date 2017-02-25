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

            SKApplication.Run();
            SKApplication.Terminate();
        }
    }

	public class MainWindow : SKWindow
	{
		protected override void HandlePaint(SKCanvas canvas)
		{
			base.HandlePaint(canvas);

			var paint = new SKPaint
			{
				Style = SKPaintStyle.Fill,
				Color = SKColors.Green
			};
			canvas.DrawRect(new SKRect(100, 100, 400, 400), paint);
		}
	}
}
