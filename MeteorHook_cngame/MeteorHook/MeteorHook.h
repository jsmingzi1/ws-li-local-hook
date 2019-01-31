#pragma once

#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

struct sFunStore  
{
	char	dllName[30];
	char	apiName[30];
	DWORD	oldAddress;
	DWORD	newAddress;
};

struct stAction
{
	WPARAM	wParam;
	BOOL	bDown;
};

#define NEXT_ACTION 2

BOOL WINAPI DllMain (HANDLE, DWORD, LPVOID);

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

extern "C" LRESULT CALLBACK KeyboardProc(int, WPARAM, LPARAM);

LRESULT CALLBACK MouseProc(int, WPARAM, LPARAM);

void DoStart();

void DoEnd();

// default use .pps
void FakePos(char[]="%s.pps");

// default use .abc
void FakeRes(char[]="meteor.abc");

// default use .abc
void FakeUserCfg(char[] = "user.abc");

void WuXianQi(BOOL);

int BiShou(WPARAM, LPARAM = 0, LPARAM = 0 );

int ChangJian(WPARAM, LPARAM = 0, LPARAM = 0);

int ChuiZi(WPARAM, LPARAM = 0, LPARAM = 0);

int ChangQiang(WPARAM, LPARAM = 0, LPARAM = 0);

int ZhiHu(WPARAM, LPARAM = 0, LPARAM = 0);

int RenDao(WPARAM, LPARAM = 0, LPARAM = 0);

int DaDao(WPARAM, LPARAM = 0, LPARAM = 0);

int QianKun(WPARAM, LPARAM = 0, LPARAM = 0);

int FeiPan(WPARAM, LPARAM = 0, LPARAM = 0);

int ZhiHu_ChuiZi(WPARAM, LPARAM = 0, LPARAM = 0);

int ZhiHu_ChangQiang(WPARAM, LPARAM = 0, LPARAM = 0);

int ZhiHu_RenDao(WPARAM, LPARAM = 0, LPARAM = 0);

int Next(WPARAM);

#define ACTION_BOTH		0
#define ACTION_DOWN		1
#define ACTION_UP		2

void Action(WPARAM wParam, LPARAM = 0, LPARAM = ACTION_BOTH);

void UnAttack(BOOL bYes = TRUE);

void KeyInput(WPARAM wParam, LPARAM lParam = KEYEVENTF_KEYUP);

void KeyInputEx(WPARAM wParam, LPARAM lParam = KEYEVENTF_KEYUP);

void ChangeWeapon(WPARAM wParam);

void Play();

void Pose(int p);

void Pose(char *);

void Chat(char *);

void Command(char *);

void TurnLeft90();

void TurnRight90();

void TurnLeft45();

void TurnRight45();

void Turn(int radis);

bool GetPosition(int iBase, float &x, float &y, float &z);


VOID CALLBACK TimerEventProc(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
VOID CALLBACK TimerOutProc(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

DWORD WINAPI ThreadProc(LPVOID);


BOOL WINAPI MyPeekMessageA(
  _Out_     LPMSG lpMsg,
  _In_opt_  HWND hWnd,
  _In_      UINT wMsgFilterMin,
  _In_      UINT wMsgFilterMax,
  _In_      UINT wRemoveMsg
);
DWORD WINAPI MyGetTickCount(void);
DWORD WINAPI MytimeGetTime(void);

// Init the paramer of hMeteor
#define INIT_PARAMER \
{\
	if (hMeteor == NULL) { \
		hMeteor = FindWindowA("Lz3DCWANG",""); \
	} \
	if (hMeteor == NULL) { \
		return 0; \
	} \
}


// Makesure the key is down instead of up
#define MAKE_KEYDOWN(wParam, lParam) \
{\
	if ((HIWORD(lParam) & KF_UP)) { \
		return 0; \
	}\
}

/*
	
*/
#define SET_TIMER_EVENT(num, t) \
{\
	if (mmTimer != NULL) { \
		timeKillEvent(mmTimer); \
	} \
	mmTimer = timeSetEvent(t, 1, TimerEventProc, num, TIME_ONESHOT);\
}


#define SET_TIMER_EVENT_EVER(num, t) \
{\
	if (mmTimer != NULL) { \
		timeKillEvent(mmTimer); \
	} \
	mmTimer = timeSetEvent(t, 1, TimerEventProc, num, TIME_PERIODIC); \
	if (mmTimerOut) { \
		timeKillEvent(mmTimerOut); \
		mmTimerOut = NULL; \
	} \
	mmTimerOut = timeSetEvent(1000, 1, TimerOutProc, 0, TIME_ONESHOT);\
}

#define KILL_TIMER_EVENT \
{\
	if (mmTimer != NULL) { \
		timeKillEvent(mmTimer); \
	} \
	if (mmTimerEx != NULL) { \
		timeKillEvent(mmTimerEx); \
	} \
	if (mmTimerOut) { \
		timeKillEvent(mmTimerOut); \
		mmTimerOut = NULL; \
	}\
}
