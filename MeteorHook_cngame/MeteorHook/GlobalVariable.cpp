#include "GlobalVariable.h"
#include <Windows.h>

#pragma data_seg ("shared_data")
LPDIRECTINPUT8       g_pDI       = NULL; // The DirectInput object         
LPDIRECTINPUTDEVICE8 g_pKeyboard = NULL; // The keyboard device 
LPDIRECTINPUTDEVICE8 g_pMouse = NULL; // The keyboard device 
HHOOK hKeyBoardHook = NULL;
HHOOK hMouseHook = NULL;
HHOOK hWindowHook = NULL;
HWND hMeteor = NULL;
int ntimes = 30;
BOOL bTimes_Ever = TRUE;
MMRESULT mmTimer = NULL;
MMRESULT mmTimerEx = NULL;
MMRESULT mmTimerEx2 = NULL;
MMRESULT mmTimerOut = NULL;
HANDLE hProcess = NULL;
BOOL bStartWeaponEx = FALSE;
BOOL bChat = FALSE;
BOOL bJieSuo = FALSE;
BOOL bGunDong = FALSE;
BOOL bAuto = TRUE;
BOOL bWDown = FALSE;
BOOL bDefenseDown = FALSE;
BOOL bChanging = FALSE;
BOOL bYuanQi = FALSE;
BOOL bAutoStart = FALSE;
BOOL bCopyDone = FALSE;
BOOL bBiShou = TRUE;
DWORD dNameAddress = 0;

WPARAM wForward = 'W';
WPARAM wBackward = 'S';
WPARAM wRightstep = 'D';
WPARAM wLeftstep = 'A';
WPARAM wAttack = 4096;
WPARAM wJump = 4608;
WPARAM wFake = 16;
WPARAM wWeaponchange = VK_TAB;
WPARAM wChat = 'P';
WPARAM wTeamchat = 'O';
WPARAM wJieSuo = 'Q';
WPARAM wMilSec = 1;
WPARAM wBefore = -1;
WPARAM wDebug = -1;
WPARAM nWeapon = 0;
WPARAM nWeaponEx = 0;
WPARAM wModel = 0;

DWORD  dOldAddress1 = 0;
DWORD  dNewAddress1 = 0;
DWORD  dOldAddress2 = 0;
DWORD  dNewAddress2 = 0;

float    itimeSpeed = 1.0;
float	 icurrentSpeed = 1.0;

int    ifirst1 = 1;
int    ifirst2 = 1;

int    iTime4Bishou = 28;
#pragma data_seg()