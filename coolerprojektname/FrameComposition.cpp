#include "Graphics.h"
#include "Window.h"
#include "gfxupdate.h"
#include <sstream>
#include "Camera.h"

void gfxupdate::DoFrame(Window &wnd)
{
	if (wnd.sGfx->dateipfad == "")
	{
		wnd.pGraphics().ClearBuffer(wnd.sGfx->colorobj);
	}
	else if (wnd.sGfx->dateipfad == "RAINBOW")
	{
		static float rainbow[4]{ 
			wnd.sGfx->colorobj->color[0], 
			wnd.sGfx->colorobj->color[1], 
			wnd.sGfx->colorobj->color[2], 
			1.0f};

		for (int i = 0; i < 3; i++)
		{
			if (rainbow[i] < 1.0f)
			{
				rainbow[i] = rainbow[i] + 0.05f;
			}
			else
			{
				rainbow[i] = 0.0f;
			}
		}

		wnd.pGraphics().ClearBuffer(rainbow[0], rainbow[1], rainbow[2]);

	}
	else if (wnd.sGfx->dateipfad == "TRIANGLE")
	{
		wnd.stf = Camera::UpDatePosition(wnd.stf, wnd.currentpressedkey);
		wnd.stf.rx = Camera::RotationLoop(wnd.stf.rx);
		wnd.stf.ry = Camera::RotationLoop(wnd.stf.ry);
		wnd.stf.rz = Camera::RotationLoop(wnd.stf.rz);

		//wnd.pGraphics().ClearBuffer(wnd.sGfx->colorobj);
		wnd.pGraphics().DrawTestTriangle(wnd.width, wnd.height, wnd.stf);
	}
	else
	{
		wnd.pGraphics().ClearBuffer(0,0,0);
	}
	wnd.pGraphics().EndFrame();
}