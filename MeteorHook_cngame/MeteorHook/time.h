#pragma once
#include <Windows.h>

/*

 Multimedia Timer Functions(below)

*/
MMRESULT WINAPI MytimeBeginPeriod(UINT uPeriod);

MMRESULT WINAPI MytimeEndPeriod(UINT uPeriod);

MMRESULT WINAPI MytimeGetDevCaps(LPTIMECAPS ptc, UINT cbtc);

MMRESULT WINAPI MytimeGetSystemTime(LPMMTIME pmmt, UINT cbmmt);

DWORD WINAPI MytimeGetTime(VOID);

MMRESULT WINAPI MytimeKillEvent(UINT uTimerID);

MMRESULT WINAPI MytimeSetEvent(
  UINT           uDelay,      
  UINT           uResolution, 
  LPTIMECALLBACK lpTimeProc,  
  DWORD_PTR      dwUser,      
  UINT           fuEvent      
);
/*

 Multimedia Timer Functions(above)

*/

BOOL WINAPI QueryPerformanceCounter(
  LARGE_INTEGER *lpPerformanceCount   // counter value
);

BOOL WINAPI QueryPerformanceFrequency(
  LARGE_INTEGER *lpFrequency   // current frequency
);

UINT_PTR WINAPI MySetTimer(
  HWND hWnd,              // handle to window
  UINT_PTR nIDEvent,      // timer identifier
  UINT uElapse,           // time-out value
  TIMERPROC lpTimerFunc   // timer procedure
);

BOOL WINAPI MyKillTimer(
  HWND hWnd,          // handle to window
  UINT_PTR uIDEvent   // timer identifier
);

DWORD WINAPI MyGetTickCount(void);		// in Kernel32.dll







