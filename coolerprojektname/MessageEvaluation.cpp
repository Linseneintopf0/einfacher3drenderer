#include "Window.h"
#include <sstream>
#include <iostream>
#include "DebugTools.h"
#include <array>


LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		for (size_t i = 0; i < Window::windowlist.size(); i++)
		{
			if (Window::windowlist[i]->hWnd == hWnd)
			{
				delete Window::windowlist[i];
			}
		}
		if (WindowCount == 0)
		{
			PostQuitMessage(69);
		}
		break;
	}
	case WM_KEYDOWN:
	{
		Window::currentpressedkey = wParam;
		break;
	}
	case WM_KEYUP:
	{
		Window::currentpressedkey = 0;
		break;
	}
	case WM_SIZE:
	{
		for (size_t i = 0; i < Window::windowlist.size(); i++)
		{
			if (Window::windowlist[i]->hWnd == hWnd)
			{
				Window* foundwindow = Window::windowlist[i];
				RECT wr;
				GetWindowRect(hWnd, &wr);
				foundwindow->width = wr.right - wr.left;
				foundwindow->height = wr.bottom - wr.top;
				foundwindow->pGraphics().UpdateAspectratio(foundwindow->width,foundwindow->height);
			}
		}
		break;
	}
	}

	std::cout << Outputlog::msgIdConv(msg) << " " << " " << wParam << " " << lParam << std::endl;

	return DefWindowProc(hWnd, msg, wParam, lParam);
}