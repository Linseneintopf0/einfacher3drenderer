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
			PostQuitMessage(0);
		}
		break;
	}
	case WM_CHAR:
	{
		if (wParam != VK_BACK)
		{
			title.push_back((char)wParam);
		}
		else if (title.size() > 0)
		{
			title.erase(title.size() - 1);
		}
		SetWindowTextA(hWnd, title.c_str());
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
	}

	std::cout << Outputlog::msgIdConv(msg) << " " << " " << wParam << " " << lParam << std::endl;

	return DefWindowProc(hWnd, msg, wParam, lParam);
}