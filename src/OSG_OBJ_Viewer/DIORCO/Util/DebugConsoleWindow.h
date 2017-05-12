#pragma once
#include <DIORCO/DIORCO_Common.h>

namespace DIORCO {

	class DebugConsoleWindow
	{
	private:
		DebugConsoleWindow()
		{
			::AllocConsole();
			FILE* stream;
			freopen_s(&stream, "CONOUT$", "wt", stdout);

			bool bDualMonitor = GetSystemMetrics(SM_CMONITORS) > 1;
			if (bDualMonitor == true)
			{
				HWND hConsoleWindow = GetConsoleWindow();
				const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
				RECT rect;
				GetWindowRect(hConsoleWindow, &rect);
				int windowX = rect.left;
				if (windowX > screenWidth)
				{
					windowX -= screenWidth;
				}
				else
				{
					windowX += screenWidth;
				}
				SetWindowPos(hConsoleWindow, NULL, windowX, rect.top, 0, 0, SWP_NOSIZE);
			}
		}

		~DebugConsoleWindow()
		{
			::FreeConsole();
		}

		static DebugConsoleWindow debugConsoleWindow;
	};
}