#pragma once

#include <Windows.h>
#include <dinput.h>

extern LPDIRECTINPUT8       g_pDI; // The DirectInput object         
extern LPDIRECTINPUTDEVICE8 g_pKeyboard; // The keyboard device 
extern LPDIRECTINPUTDEVICE8 g_pMouse; // The keyboard device 
extern HHOOK hKeyBoardHook;
extern HHOOK hMouseHook;
extern HHOOK hWindowHook;
extern HWND hMeteor;
extern int ntimes;
extern BOOL bTimes_Ever;
extern BOOL bAuto;
extern MMRESULT mmTimer;
extern MMRESULT mmTimerEx;
extern MMRESULT mmTimerEx2;
extern MMRESULT mmTimerOut;
extern HANDLE hProcess;
extern BOOL bStartWeaponEx;
extern BOOL bChat;
extern BOOL bJieSuo;
extern BOOL bGunDong;
extern BOOL bWDown;
extern BOOL bDefenseDown;
extern BOOL bChanging;
extern BOOL bYuanQi;
extern BOOL bAutoStart;
extern BOOL bCopyDone;
extern BOOL bBiShou;
extern DWORD dNameAddress;


extern WPARAM wForward;
extern WPARAM wBackward;
extern WPARAM wRightstep;
extern WPARAM wLeftstep;
extern WPARAM wAttack;
extern WPARAM wJump;
extern WPARAM wFake;
extern WPARAM wWeaponchange;
extern WPARAM wChat;
extern WPARAM wTeamchat;
extern WPARAM wMilSec;
extern WPARAM wJieSuo;
extern WPARAM nWeapon;
extern WPARAM nWeaponEx;
extern WPARAM wModel;

extern WPARAM wBefore;
extern WPARAM wDebug;

extern DWORD  dOldAddress1;
extern DWORD  dNewAddress1;
extern DWORD  dOldAddress2;
extern DWORD  dNewAddress2;

extern float    itimeSpeed;
extern float	icurrentSpeed;

extern int    ifirst1;
extern int    ifirst2;

extern int    iTime4Bishou;
