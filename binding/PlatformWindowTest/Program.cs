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
		DateTime mPrevTime = DateTime.Now;

		public MainWindow()
			: base(NativeRenderBackend.OpenGL)
		{
			SKApplication.Current.Idle += (s, e) =>
			{
				// poor man's way to hit 60 fps
				var elapsed = (DateTime.Now - mPrevTime).TotalMilliseconds;
				if (elapsed >= 16)
				{
					Invalidate();
					mPrevTime = DateTime.Now;
				}
			};
		}

		static Random R = new Random();

		struct Particle
		{
			public float X;
			public float Y;

			public Particle(float x, float y)
			{
				X = x;
				Y = y;
			}

			public void Update()
			{
				// Random walk
				X += (R.NextFloat() - 0.5f) * 0.005f;
				Y += (R.NextFloat() - 0.5f) * 0.005f;
			}
		}

		// Early findings on my PC (Quad Core 3.4 GHz):
		// Raster - With Alpha blending (see below) can only handle a dozen of these
		// OpenGL - Full Alpha - 10s of thousands of these without breaking a sweat!
		//
		const int ParticleCount = 256;
		Particle[] Particles;

		protected override void OnPaint(SKCanvas canvas)
		{
			base.OnPaint(canvas);

			// setup our Particles
			const float ShapeRadius = 128;
			if(Particles == null || Particles.Length == 0)
			{
				Particles = new Particle[ParticleCount];

				for(int i = 0; i < Particles.Length; i++)
				{
					Particles[i] = new Particle(R.NextFloat(), R.NextFloat());
				}
			}

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
				Color = bgColor.WithBlue(80).WithAlpha(25)		// RASTER slows dramatically when Alpha is != 255 :(
			};

			for (int i = 0; i < Particles.Length; i++)
			{
				//var fillRect = SKRect.Create(Particles[i].X * this.Width, Particles[i].Y * this.Height, ShapeRadius * 2, ShapeRadius * 2);
				//canvas.DrawRect(fillRect, paint);
				canvas.DrawCircle(Particles[i].X * this.Width, Particles[i].Y * this.Height, ShapeRadius, paint);
				Particles[i].Update();
			}

			paint.Typeface = SKTypeface.FromFamilyName("Arial");
			paint.TextSize = ((float)modulator * 72) + 36;
			paint.Color = SKColors.Black;

			var message = "Hello World!";
			var textWidth = paint.MeasureText(message);
			canvas.DrawText(message, centerX - (textWidth / 2), centerY - 10, paint);
		}

		protected override bool OnMouse(int x, int y, InputState state, uint modifiers)
		{
			var handled = base.OnMouse(x, y, state, modifiers);
			Console.WriteLine($"Mouse: {state.ToString()} - {x}, {y}");
			return handled;
		}
	}

	public static class RandomExtensions
	{
		public static float NextFloat(this Random r)
		{
			return (float)r.NextDouble();
		}
	}
}
