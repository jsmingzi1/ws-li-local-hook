#include "MeteorHook.h"
#include "GlobalVariable.h"
#include "Config.h"
#include <Dbghelp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <dinput.h>
#include <detours.h>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"detours.lib")
#pragma warning(disable:4018)
#pragma warning(disable:4244)

#define MOUSE_LBUTTON 4096
#define MOUSE_RBUTTON 4608

static BOOL (WINAPI * TruePeekMessageA)(LPMSG, HWND, UINT, UINT, UINT) = PeekMessageA;
BOOL WINAPI MyPeekMessageA(
  _Out_     LPMSG lpMsg,
  _In_opt_  HWND hWnd,
  _In_      UINT wMsgFilterMin,
  _In_      UINT wMsgFilterMax,
  _In_      UINT wRemoveMsg
)
{
	BOOL bRet = TruePeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);

	if ( lpMsg->message == WM_KEYDOWN && lpMsg->wParam != 'W' && lpMsg->wParam != 'A' && lpMsg->wParam != 'D' && lpMsg->wParam != 'S' && lpMsg->wParam != VK_SPACE)
	{
		KeyboardProc(1, lpMsg->wParam, lpMsg->lParam);
	}
	else if (lpMsg->message == WM_MBUTTONDOWN || lpMsg->message == WM_MOUSEWHEEL )
	{
		MouseProc(1, lpMsg->message, 0);
	}
	return bRet;
}

static DWORD (WINAPI * TrueGetTickCount)(void) = GetTickCount;
DWORD WINAPI MyGetTickCount(void)
{
	return TrueGetTickCount() * itimeSpeed;
}

static DWORD (WINAPI * TruetimeGetTime)(void) = timeGetTime;
DWORD WINAPI MytimeGetTime(void)
{
	return TruetimeGetTime() * itimeSpeed;
}


void FakePos(char szDst[])
{
	unsigned int nAddress1 = 0x005c9df8;
	strcpy((char *)nAddress1, szDst);
}

void FakeRes(char szDst[])
{
	unsigned int nAddress1 = 0x005c9ce4;
	strcpy((char *)nAddress1, szDst);
}

void FakeUserCfg(char szDst[])
{
	unsigned int nAddress1 = 0x005c7940;
	strcpy((char *)nAddress1, szDst);
}

void DoStart()
{
	FILE *flag= freopen("1.txt", "r", stdin);
	if (flag != NULL)
	{
		fclose(flag);
		bAutoStart = TRUE;
	}
	if (bAutoStart)
	{
		CopyFile("meteor.res", "meteor.txt", FALSE);
		FakeRes("meteor.txt");
	}
	else
	{
		FILE *res = freopen("meteor.abc", "r", stdin);
		if (res != NULL)
		{
			fclose(res);
			FakeRes("meteor.abc");
		}
		/*
		FILE *cfg = freopen("user.abc", "r", stdin);
		if (cfg != NULL)
		{
			fclose(cfg);
			FakeUserCfg("user.abc");
		}*/
	}
}

void DoEnd()
{
	if (bAutoStart)
	{
		DeleteFile("meteor.txt");

		char szFile[255];
		for (int i = 0; i < 20; i++)
		{
			sprintf(szFile, "./pmodel/P%d.txt", i);
			DeleteFile(szFile);
		}
	}
}

void WuXianQi(BOOL bWu)
{
	byte value = 0;

	if (bWu == TRUE)
	{
		value = 0x10;
		WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)(0x4D5D51 + 3), &value, 1, NULL);
		value = 0x00;
		WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)(0x4DCFBF + 1), &value, 1, NULL);
		WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)(0x4DCFDE + 1), &value, 1, NULL);
		WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)(0x4BBAC9 + 1), &value, 1, NULL);
	}
	else
	{
		value = 0x08;
		WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)(0x4D5D51 + 3), &value, 1, NULL);
		value = 0x01;
		WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)(0x4DCFBF + 1), &value, 1, NULL);
		value = 0x02;
		WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)(0x4DCFDE + 1), &value, 1, NULL);
		value = 0x24;
		WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)(0x4BBAC9 + 1), &value, 1, NULL);
	}
}

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved) {
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		itimeSpeed = ReadSpeed();
		srand( (unsigned)time( NULL ) );
		CreateThread(NULL, 0, ThreadProc, NULL, 0, 0);

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TruePeekMessageA, MyPeekMessageA);
		DetourAttach(&(PVOID&)TrueGetTickCount, MyGetTickCount);
		DetourAttach(&(PVOID&)TruetimeGetTime, MytimeGetTime);

		DetourTransactionCommit();

		DoStart();
		break;
	case DLL_PROCESS_DETACH:
		DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)TruePeekMessageA, MyPeekMessageA);
		DetourDetach(&(PVOID&)TrueGetTickCount, MyGetTickCount);
		DetourDetach(&(PVOID&)TruetimeGetTime, MytimeGetTime);

        DetourTransactionCommit();	
		DoEnd();
		break;
	}
	return TRUE;
}
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {

	
	unsigned int nAddress;
	unsigned int nAddress1 = 0x006659B8;
	unsigned int nAddress2;
	unsigned int nYuanQi = 0;

	if ( code < 0 )
	{
		return CallNextHookEx( hKeyBoardHook, code, wParam, lParam );
	}

	if (KEYDOWN('W'))
		bWDown = TRUE;
	else
		bWDown = FALSE;

	if (KEYDOWN(VK_SPACE))
		bDefenseDown = TRUE;
	else
		bDefenseDown = FALSE;


	//MAKE_KEYDOWN(wParam, lParam)
	INIT_PARAMER

	if (wParam == VK_F1)
	{
		WuXianQi(TRUE);
		return 1;
	}
	else if (wParam == VK_F2)
	{
		WuXianQi(FALSE);
		return 1;
	}
	else if (wParam == VK_F11)
	{
		FakePos("%s.pps");
	}

	if (wParam == VK_HOME)	// ¿ª/¹Ø¸¨Öú
	{
		bStartWeaponEx = TRUE;
		ReadConfig();
		return 0;
	}
	if (wParam == VK_END)
	{
		bStartWeaponEx = FALSE;
		return 0;
	}

	if (bStartWeaponEx == FALSE)
		return 0;


	if (wParam == VK_OEM_5 || wParam == wChat || wParam == wTeamchat)
	{
		bChat = TRUE;
	}
	else if (wParam == VK_F12 )
	{
		ExitProcess(0);
		return 0;
	}

	if (wParam == VK_RETURN)
	{
		bChat = FALSE;
	}
	if (bChat)
		return 0;

	
	if (wParam >= VK_NUMPAD5 && wParam <= VK_NUMPAD9)
	{
		if (KEYDOWN(VK_LCONTROL))
			ChangeWeapon( 32 + (wParam - VK_NUMPAD5) );
		else
			ChangeWeapon( 14 + (wParam - VK_NUMPAD5) );
		if (bWDown)
		{
			keybd_event( 'W', 0, KEYEVENTF_KEYUP, 0);
			keybd_event( 'W', 0, 0, 0);
		}
		bAuto = FALSE;
		nWeaponEx = nWeapon;
		nWeapon = wParam - VK_NUMPAD5 + 5;

		return 1;
	}
	if (wParam == VK_NUMPAD4)
	{
		ChangeWeapon( 56 );
		if (bWDown)
		{
			keybd_event( 'W', 0, KEYEVENTF_KEYUP, 0);
			keybd_event( 'W', 0, 0, 0);
		}
		bAuto = FALSE;
		nWeaponEx = nWeapon;
		nWeapon = 12;

		return 0;
	}
	if (wParam == VK_NUMPAD2)
	{
		ChangeWeapon( 52 );
		if (bWDown)
		{
			keybd_event( 'W', 0, KEYEVENTF_KEYUP, 0);
			keybd_event( 'W', 0, 0, 0);
		}
		bAuto = FALSE;
		nWeaponEx = nWeapon;
		nWeapon = 11;

		return 0;
	}
	switch(wParam)
	{
		case 'L':
			{
				ntimes = 1;
				return 1;
			}
		case 'K':
			{
				//bAuto = TRUE;
				//Pose(161);
				UnAttack(FALSE);
				return 1;
			}
		case 'J':
			{
				//ntimes = 2;
				//Pose(162);
				UnAttack();
				return 1;
			}
		case 'M':
			{
				int w = rand() % 11 + 1;
				ChangeWeapon( w );
				keybd_event( 'H', 0, 0, 0);
				keybd_event( 'H', 0, KEYEVENTF_KEYUP, 0);
				
				return 1;
			}
		case VK_MULTIPLY:
			{
				Pose( 90 );
				return 1;
			}
		case VK_DIVIDE:
			{
				Play();
				return 1;
			}
		case VK_TAB:
			{
				bAuto = TRUE;
				break;
			}
		case VK_ADD:
			{
				iTime4Bishou = iTime4Bishou + 10;
				return 1;
			}
		case VK_SUBTRACT:
			{
				iTime4Bishou = 28;
				return 1;
			}
		case '0':
			{
				ntimes = 30;
				break;
			}

		}
		
	if (wParam == 'E')
	{
		if (bDefenseDown)
				{
					Action(VK_SPACE, lParam, ACTION_UP);
				}
		Action(wRightstep, 0, ACTION_BOTH);
		Action(wRightstep, 0, ACTION_BOTH);
		if (bDefenseDown)
				{
					Action(VK_SPACE, lParam, ACTION_DOWN);
				}
		return 1;
	}
	
		

		

		if ( bAuto )
		{
			if (hProcess == NULL)
			{
				hProcess = GetCurrentProcess();
			}
			//ReadProcessMemory(hProcess, (LPCVOID)nAddress1, &nAddress2, sizeof(int), NULL);
			memcpy(&nAddress2, (void *)nAddress1,sizeof(int));
			nAddress = nAddress2 + 0x818;
			dNameAddress = nAddress2 - 0x8F98;
			//ReadProcessMemory(hProcess, (LPCVOID)nAddress, &nWeapon, sizeof(int), NULL);
			memcpy(&nWeapon, (void *)nAddress,sizeof(int));
			memcpy(&nYuanQi, (void *)(nAddress - 276), sizeof(int));
			if (nWeapon == 11 || nWeapon == 5)
			{
				//ReadProcessMemory(hProcess, (LPCVOID)(nAddress + 4), &nWeapon, sizeof(int), NULL);
				memcpy(&nWeaponEx, (void *)(nAddress + 4),sizeof(int));
			}
		
		}

		lParam = 0;
		switch (nWeapon)
		{
		case 5:
			{
				if (nYuanQi == 100)
				{
					bYuanQi = TRUE;
				}
				else
				{
					bYuanQi = FALSE;
				}
				return Next(BiShou(wParam, lParam, nWeaponEx));
			}
		case 6:
			return Next(ChangJian(wParam, lParam));
		case 7:
			return Next(ChangQiang(wParam, lParam));
		case 8:
			return Next(DaDao(wParam, lParam));
		case 9:
			return Next(ChuiZi(wParam, lParam));
		case 11:
			{
				if (nWeaponEx == 12)
					return Next(ZhiHu_RenDao(wParam, lParam));
				else if (nWeaponEx == 7)
					return Next(ZhiHu_ChangQiang(wParam, lParam));
				else if (nWeaponEx == 9)
					return Next(ZhiHu_ChuiZi(wParam, lParam));
				else return Next(ZhiHu(wParam, lParam));
			}
			return Next(ZhiHu(wParam, lParam));
		case 12:
			return Next(RenDao(wParam, lParam));
		case 2:
			return Next(FeiPan(wParam, lParam));
		default:
			return Next(QianKun(wParam, lParam));
		}
	

	return 0;
}
LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
{

	lParam = 0;
	if ( wParam == WM_MBUTTONDOWN )
	{
		if (KEYDOWN('W'))
			keybd_event( 'W', 0, KEYEVENTF_KEYUP, 0);
		Action(wBackward);
		Action(wForward);
		Action(wAttack);
		if (KEYDOWN('W'))
			keybd_event( 'W', 0, 0, 0);
		return 1;
	}
	//else if (wParam == WM_RBUTTONDOWN && KEYDOWN('W') && KEYDOWN('D'))
	//{
	//	keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
	//	SET_TIMER_EVENT(104, 150);
	//	return 1;
	//}
	else if (wParam == WM_MOUSEWHEEL)
	{
		if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_UP);
			}
		
		if (nWeapon == 12)
		{
			Action(wBackward);
			Action(wBackward);
			Action(wAttack);
			//SET_TIMER_EVENT(1, 10/*wMilSec*/)
		}
		else
		{
			Action(wJump);
			SET_TIMER_EVENT(1, 5/*wMilSec*/)
		}
			if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_DOWN);
			}
		return 1;
	}
	return 0;
}

int BiShou(WPARAM wParam, LPARAM lParam, LPARAM WeaponEx) {

	switch (wParam)
	{
	case '2':
	{
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Turn(350);
		SET_TIMER_EVENT(105, 20);
		return 1;
	}
	case '3':
		{
			Action('S', 0, ACTION_BOTH);
			Action('W', 0, ACTION_BOTH);
			Action('W', 0, ACTION_BOTH);
			Action(wAttack, 0, ACTION_BOTH);
			return 1;
		}
	case '6':
		ntimes = 2;
		return 1;
	case '7':
		ntimes = 3;
		return 1;
	case '8':
		ntimes = 5;
		return 1;
	case '9':
		ntimes = 9;
		return 1;

	case '4':
		{
			//if (bWDown)
			{
				if (bDefenseDown)
				{
					Action(VK_SPACE, lParam, ACTION_UP);
				}
				
				if (KEYDOWN('W') || bBiShou)
				{
					Action(wJump, 0, 0);
					SET_TIMER_EVENT(28, 7);
				}
				else
				{
					keybd_event('W', 0, 0, 0);
					//keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
					Action(wJump, 0, 0);
					SET_TIMER_EVENT(102, 150);
				}
			}
			return 1;
		}
		
	case '5':
		{
			Action(wJump, lParam);
			Action(wJump, lParam);
			Action(wJump, lParam);
			Action(wJump, lParam);
			Action(wJump, lParam);
			return 1;
		}
		return 1;
	case 'R':
		{
			/*if (WeaponEx != 7)
			{*/
			if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_UP);
			}
			
				Action(wJump, lParam);
				if (bWDown)
				{
					Action(wForward);
				}
				Action(wBackward, lParam);
				Action(wAttack, lParam);
			if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_DOWN);
			}
			/*}
			else
			{
				Action(wJump, lParam);
				SET_TIMER_EVENT(28, 3)
			}*/
			return 1;
		}
	case 'T':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
		}
		return 1;
	case 'C':
		{
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Action( wLeftstep, lParam);
		Action(wJump, lParam);
		
		

			SET_TIMER_EVENT(1, 5)

		return 1;
		}
	case 'V':
		{
		if (bDefenseDown)
			Action(VK_SPACE, lParam, ACTION_UP);
		Action(wBackward, lParam);
		Action(wForward, lParam);
		Action(wAttack, lParam);
		if (bDefenseDown)
			Action(VK_SPACE, lParam, ACTION_DOWN);
		return 1;
		}
case 'F':
		{
			if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_UP);
			}
				if (KEYDOWN('W'))
				{
					Action('W', 0, ACTION_UP);
					Action(wForward, lParam);
					Action(wForward, lParam);
					Action('W', 0, ACTION_DOWN);
				}
				else
				{
					Action(VK_ESCAPE);
					Action(VK_ESCAPE);
					Action(wBackward);
					Action(wAttack);
				}

				if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_DOWN);
			}
			return 1;
		}

	//case '3':
	//	{
	//		Action(VK_ESCAPE);
	//		Action(VK_ESCAPE);
	//		Action(wBackward, lParam);
	//		Action(wAttack, lParam);
	//		Action(VK_ESCAPE);
	//		Action(VK_ESCAPE);
	//		return 1;
	//	}
	default:
		;
	}
	return 0;
}
int ChangJian(WPARAM wParam, LPARAM lParam, LPARAM)
{
	switch(wParam)
	{
	case 'F':
		{
			if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_UP);
			}
				if (KEYDOWN('W'))
				{
					Action('W', 0, ACTION_UP);
					Action(wForward, lParam);
					Action(wForward, lParam);
					Action('W', 0, ACTION_DOWN);
				}
				else
				{
			
					Action(wBackward, lParam);
					Action(wFake, lParam);
					Action(wBackward, lParam);
				}

				if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_DOWN);
			}
			return 1;
		}
	case 'X':
		Action(wRightstep, lParam);
		Action(wJump, lParam);
		Action(wRightstep, lParam);
		if (wMilSec)
			SET_TIMER_EVENT(1, wMilSec)
		else
			Action(wAttack, lParam);

		return 1;
	case 'T':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
			return 1;
		}
	case 'C':
		Action(wForward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'V':
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'R':
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'E':
		Action(wRightstep, lParam);
		if (bGunDong)
			Action(wFake, lParam);
		Action(wRightstep, lParam);
		return 1;
	}
	return 0;
}
int ChuiZi(WPARAM wParam, LPARAM lParam, LPARAM) {
	switch (wParam)
	{
	case '4':
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Turn(350);
		SET_TIMER_EVENT(100, 20);
		return 1;
	case '2':
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Turn(350);
		SET_TIMER_EVENT(101, 20);
		return 1;
	case '5':
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Turn(120);
		SET_TIMER_EVENT(100, 20);
		return 1;
	case '3':
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Turn(-120);
		SET_TIMER_EVENT(100, 20);
		return 1;
	case 'C':
		//Action(wRightstep, lParam, 1);
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Action(wJump, lParam);
		SET_TIMER_EVENT(1, 5)


		return 1;
	case 'V':
		Action(wJump, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'X':
		//Action(VK_ESCAPE);
		//Action(VK_ESCAPE);
		Action(wLeftstep, lParam);
		
		Action(wRightstep, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'R':
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}

			Action(wBackward, lParam);

			Action(wAttack, lParam);
			if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_DOWN);
			}
		
		return 1;
	case 'T':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
			
			return 1;
		}
	case 'F':
		{
			if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_UP);
			}
				if (KEYDOWN('W'))
				{
					Action('W', 0, ACTION_UP);
					Action(wForward, lParam);
					Action(wForward, lParam);
					Action('W', 0, ACTION_DOWN);
				}
				else
				{
			
					Action(wBackward, lParam);
					Action(wFake, lParam);
					Action(wBackward, lParam);
				}

				if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_DOWN);
			}
			return 1;
		}
		
	}
	return 0;
}
int ChangQiang(WPARAM wParam, LPARAM lParam, LPARAM) {
	switch (wParam)
	{
	case 'E':
		if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_UP);
			}
		Action(wRightstep, lParam);
		if (bGunDong)
			Action(wFake, lParam);
		Action(wRightstep, lParam);
		if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_DOWN);
			}
		return 1;
	case 'R':
		if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_UP);
			}
			
		Action(wJump, lParam);
		

		SET_TIMER_EVENT(4, 10)
			if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_DOWN);
			}
			
		return 1;
	case 'T':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
			return 1;
		}
	case 'C':
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Action(wRightstep, lParam);
		Action(wJump, lParam);
		Action(wRightstep, lParam);
		if (wMilSec)
			SET_TIMER_EVENT(1, wMilSec)
		else
		{
			Action(wAttack, lParam);
			if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_DOWN);
			}
		}

		return 1;
	case 'F':
		{
			if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_UP);
			}
				if (KEYDOWN('W'))
				{
					Action('W', 0, ACTION_UP);
					Action(wForward, lParam);
					Action(wForward, lParam);
					Action('W', 0, ACTION_DOWN);
				}
				else
				{
			
					Action(wRightstep, lParam);
					Action(wLeftstep, lParam);
					Action(wAttack, lParam);
				}

				if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_DOWN);
			}
			return 1;
		}
	case 'X':
		if (bWDown)
		{
			Action(wBackward, lParam);
		
			Action(wBackward, lParam);
		
		
			Action(wAttack, lParam);
		}
		else
		{
			Action(wBackward, lParam);
		
			Action(wBackward, lParam);
		
			Action(wForward, lParam);
		
			Action(wAttack, lParam);
		}
		
		return 1;
	case 'V':

		Action(wJump, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case '5':
		{
		Action(wJump, lParam);
		SET_TIMER_EVENT(28, 3)
		}
		return 1;
	case '4':
		{
			if (bWDown)
			{
				Action(wForward, lParam, ACTION_UP);
			}
			else
			{
				Action(wForward);
			}
			Action(wAttack, lParam);
			Action(VK_ESCAPE);
			Action(VK_ESCAPE);
			SET_TIMER_EVENT(36, 200)
			return 1;
		}
	case '3':
		if (bWDown)
		{
			Action('W', lParam, ACTION_UP);
		}
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		if (bWDown)
		{
			Action('W', lParam, ACTION_DOWN);
		}
		return 1;
	}
	return 0;
}
int ZhiHu(WPARAM wParam, LPARAM lParam, LPARAM) {
	switch (wParam) 
	{
	case '3':
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Turn(650);
		SET_TIMER_EVENT(100, 20);

		return 1;
	case 'E':
		Action(wRightstep, lParam);
		if (bGunDong)
			Action(wFake, lParam);
		Action(wRightstep, lParam);
		return 1;
	case '5':
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case '4':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
			
			return 1;
		}
	case 'T':
		Action(wForward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'F':
		{
			if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_UP);
			}
				if (KEYDOWN('W'))
				{
					Action('W', 0, ACTION_UP);
					Action(wForward, lParam);
					Action(wForward, lParam);
					Action('W', 0, ACTION_DOWN);
				}
				else
				{
			
					Action(wBackward, lParam);
					//Action(wFake, lParam);
					Action(wBackward, lParam);
				}
				if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_DOWN);
			}
			return 1;
		}
	case 'C':
		Action(wJump, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'R':
		//Action(wBackward, lParam);
		//
		//Action(wForward, lParam);
		//
		//Action(wAttack, lParam);

		for (int i=0; i<20; i++)
		{
			Action(wRightstep);
		
			Action(wRightstep);
		
			Action(wAttack, lParam);
		}
		bAuto = TRUE;
		
		
		//keybd_event( VK_OEM_5, 0, 0, 0);
		//keybd_event( VK_OEM_5, 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( 'W', 0, 0, 0);
		//keybd_event( 'W', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( 'E', 0, 0, 0);
		//keybd_event( 'E', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( 'A', 0, 0, 0);
		//keybd_event( 'A', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( 'P', 0, 0, 0);
		//keybd_event( 'P', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( 'O', 0, 0, 0);
		//keybd_event( 'O', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( 'N', 0, 0, 0);
		//keybd_event( 'N', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( ' ', 0, 0, 0);
		//keybd_event( ' ', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( '1', 0, 0, 0);
		//keybd_event( '1', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( '8', 0, 0, 0);
		//keybd_event( '8', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event( VK_RETURN, 0, 0, 0);
		//keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
		
		


		/*Action(wLeftstep, lParam);
		
		Action(wRightstep);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);*/
		return 1;
	case 'V':
		Action(wBackward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'X':
		Action(wLeftstep, lParam);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);
		
		return 1;
	}

	return 0;
}
int RenDao(WPARAM wParam, LPARAM lParam, LPARAM) {
	switch (wParam)
	{
	case 'E':
		if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_UP);
			}
		Action(wRightstep, lParam);
		if (bGunDong)
			Action(wFake, lParam);
		Action(wRightstep, lParam);
		if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_DOWN);
			}
		return 1;
	case '4':
		Action(wForward, lParam);
		
		Action(wForward, lParam);
		
		Action(wJump, lParam, ACTION_DOWN);
		Action(wAttack, lParam);
		Action(VK_ESCAPE);
		Action(VK_ESCAPE);
		return 1;
	case 'R':
		Action(wLeftstep, lParam);
		Action(wRightstep);
		Action(wForward, lParam);
		Action(wBackward, lParam);
		Action(wAttack, lParam);
		return 1;
	case 'T':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
			
			return 1;
		}
	case 'C':
		{
		Action(wJump, lParam);
		//if (wMilSec)
			SET_TIMER_EVENT(37, 15/*wMilSec*/)
		//else
		//	Action(wAttack, lParam);

		return 1;
		}
	case '5':
			Action(wJump, lParam);
			SET_TIMER_EVENT(28, 3)
			return 1;
	case 'B':
	case 'V':
		Action(wForward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		

		SET_TIMER_EVENT(17, 800);

		return 1;
	case 'X':
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		Action(wBackward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'F':
		{
			if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_UP);
			}
				if (KEYDOWN('W'))
				{
					Action('W', 0, ACTION_UP);
					Action(wForward, lParam);
					Action(wForward, lParam);
					Action('W', 0, ACTION_DOWN);
				}
				else if (KEYDOWN('S'))
				{
					Action(wBackward, lParam);
					Action(wBackward, lParam);
					
				}
				else
				{
			
					Action(wBackward, lParam);
					
					Action(wForward, lParam);
					Action(wForward, lParam);
					Action(wAttack, lParam);
				}
				if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_DOWN);
			}
			return 1;
		}

	}

	return 0;
}
int DaDao(WPARAM wParam, LPARAM lParam, LPARAM) {
	switch (wParam)
	{
	case 'E':

		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Action(VK_ESCAPE);
		Action(VK_ESCAPE);
		Action(wRightstep, lParam);
		if (bGunDong)
			Action(wFake, lParam);
		Action(wRightstep, lParam);
		
		Action(wJieSuo, lParam);	
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_DOWN);
		}
		return 1;
	case 'R':
		Action(wJump, lParam);
		
		
		SET_TIMER_EVENT(4, 30)

		return 1;
	case 'T':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
			
			return 1;
		}
	case 'X':
		Action(wRightstep, lParam);
		Action(wJump, lParam);
		Action(wRightstep, lParam);
		if (wMilSec)
			SET_TIMER_EVENT(1, wMilSec)
		else
			Action(wAttack, lParam);

		return 1;
	case 'V':
		Action(wLeftstep, lParam);
		
		Action(wRightstep);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'F':
		{
		if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_UP);
			}
		Action(wForward);
		Action(wForward);
		if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_DOWN);
			}
		return 1;
		}
	case 'C':
		Action(wLeftstep, 0, ACTION_DOWN);
		Action(wJump);

		SET_TIMER_EVENT(1, 100)
	
		return 1;
	case '5':
		Action(wJump, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case '4':
		{
			if (bDefenseDown)
			{
				Action(VK_SPACE, lParam, ACTION_UP);
			}
			KeyInput(wLeftstep, 0);
			SET_TIMER_EVENT(40, 80)
			return 1;
		}
	}

	return 0;
}
int QianKun(WPARAM wParam, LPARAM lParam, LPARAM)
{
	switch (wParam)
	{
	case 'E':
		Action(wRightstep, lParam);
		if (bGunDong)
			Action(wFake, lParam);
		Action(wRightstep, lParam);
		
		Action(wJieSuo, lParam);		
		return 1;
	case 'C':
		Action(wRightstep, lParam);
		Action(wJump, lParam);
		Action(wRightstep, lParam);
		if (wMilSec)
			SET_TIMER_EVENT(1, wMilSec)
		else
			Action(wAttack, lParam);
		return 1;
	case 'R':
		Action(wJump, lParam);
		
		
		SET_TIMER_EVENT(4, 30)

		return 1;
	case 'T':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
			
			return 1;
		}
	case 'F':
		{
			Action(wBackward, lParam);
			Action(wForward, lParam);
			Action(wAttack, lParam);
			return 1;
		}
	}
	return 0;
}

int FeiPan(WPARAM wParam, LPARAM lParam, LPARAM)
{
	switch (wParam)
	{
	case '4':
	{
		//keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
		//keybd_event('D', 0, KEYEVENTF_KEYUP, 0);
		//Action(wJump);
		keybd_event('D', 0, 0, 0);
		Action(wJump);
		Action(wJump);
		keybd_event('W', 0, 0, 0);
		//keybd_event('D', 0, 0, 0);
		//keybd_event('D', 0, 0, 0);
		return 1;
	}
	case 'E':
		Action(wRightstep, lParam);
		if (bGunDong)
			Action(wFake, lParam);
		Action(wRightstep, lParam);

		Action(wJieSuo, lParam);
		return 1;
	case 'C':
		Action(wJump, lParam);
		Action(wAttack, lParam);
		return 1;
	case 'R':
		Action(wJump, lParam, ACTION_DOWN);


		Action(wAttack, lParam);

		return 1;
	case 'T':
	{
		int nt = ntimes;
		while (nt--) {
			Action(wJump, lParam);
		}

		return 1;
	}
	}
	return 0;
}

int ZhiHu_ChuiZi(WPARAM wParam, LPARAM lParam, LPARAM) {
	switch (wParam)
	{
	case '3':
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Turn(650);
		SET_TIMER_EVENT(100, 20);

		return 1;
	case '1':
		bTimes_Ever = TRUE;
		return 1;
	case '2':
		bTimes_Ever = FALSE;
		return 1;
	case 'E':
		Action(wRightstep, lParam);
		if (bGunDong)
			Action(wFake, lParam);
		Action(wRightstep, lParam);
		return 1;
	case 'R':
		Action(wLeftstep);
		
		Action(wLeftstep);
		
		Action(wAttack, lParam);
		
		bAuto = TRUE;

		Action(wLeftstep, lParam);
		
		Action(wRightstep);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		if (bTimes_Ever)
		{
			SET_TIMER_EVENT_EVER(9, 200)
		}
		return 1;
	case 'C':
		Action(wJump, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'T':
		Action(wRightstep);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);
		
		bAuto = TRUE;
		
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		if (bTimes_Ever)
		{
			SET_TIMER_EVENT_EVER(10, 200)
		}
		return 1;
	case '6':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}	
			
			return 1;
		}
	case '4':
		Action(wRightstep);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);
		
		bAuto = TRUE;

		Action(wLeftstep, lParam);
		
		Action(wRightstep);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		if (bTimes_Ever)
		{
			SET_TIMER_EVENT_EVER(9, 200)
		}
		return 1;
	case '5':
		Action(wRightstep);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);
		

		Action(wBackward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		if (bTimes_Ever)
		{
			SET_TIMER_EVENT_EVER(12, 200)
		}
		return 1;
	case 'X':
		Action(wLeftstep, lParam);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'V':
		Action(wLeftstep);
		
		Action(wLeftstep);
		
		Action(wAttack, lParam);
		
		bAuto = TRUE;
		
		Action(wBackward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		if (bTimes_Ever)
		{
			SET_TIMER_EVENT_EVER(11, 200)
		}
		return 1;
	case 'F':
		{
			if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_UP);
			}
				if (KEYDOWN('W'))
				{
					Action('W', 0, ACTION_UP);
					Action(wForward, lParam);
					Action(wForward, lParam);
					Action('W', 0, ACTION_DOWN);
				}
				else if (KEYDOWN('S'))
				{
					Action('S', 0, ACTION_UP);
					Action(wBackward, lParam);
					Action(wBackward, lParam);
					Action('S', 0, ACTION_DOWN);
				}
				else
				{
			
					Action(wForward, lParam);
					Action(wBackward, lParam);
					//Action(wFake, lParam);
					Action(wAttack, lParam);
				}
				if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_DOWN);
			}
			return 1;
		}
	}

	return 0;
}
int ZhiHu_ChangQiang(WPARAM wParam, LPARAM lParam, LPARAM) {
	switch (wParam)
	{
	case '3':
		if (bDefenseDown)
		{
			Action(VK_SPACE, lParam, ACTION_UP);
		}
		Turn(650);
		SET_TIMER_EVENT(100, 20);

		return 1;
	case '1':
		bTimes_Ever = TRUE;
		return 1;
	case '2':
		bTimes_Ever = FALSE;
		return 1;
	case 'E':
		Action(wRightstep, lParam);
		if (bGunDong)
			Action(wFake, lParam);
		Action(wRightstep, lParam);
		return 1;
	case 'R':
		Action(wRightstep);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);
		

		bAuto = TRUE;
	
		Action(wForward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		if (bTimes_Ever)
		{
			SET_TIMER_EVENT_EVER(14, 200)
		}
		return 1;
	case 'C':
		Action(wJump, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'T':
		Action(wRightstep);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);
		
		bAuto = TRUE;

		Action(wBackward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		if (bTimes_Ever)
		{
			SET_TIMER_EVENT_EVER(16, 200)
		}
		return 1;
	case 'X':
		Action(wLeftstep, lParam);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);
		
		return 1;
	case 'V':
		Action(wJump, lParam);
		
		SET_TIMER_EVENT(20, 20);
		return 1;
	case 'F':
		Action(wForward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		return 1;
	case '4':
		{
			int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
			return 1;
		}
	}

	return 0;
}
int ZhiHu_RenDao(WPARAM wParam, LPARAM lParam, LPARAM)
{
	switch(wParam)
	{
	case 'T':
		{
		int nt = ntimes;
			while (nt --) {
				Action(wJump, lParam);
			}
		return 1;
		}
	case 'R':
		Action(wRightstep);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);

		Action(wLeftstep, lParam);
		Action(wRightstep);
		Action(wForward, lParam);
		Action(wBackward, lParam);
		Action(wAttack, lParam);
		return 1;
		case 'F':
		{
			if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_UP);
			}
				if (KEYDOWN('W'))
				{
					Action('W', 0, ACTION_UP);
					Action(wForward, lParam);
					Action(wForward, lParam);
					Action('W', 0, ACTION_DOWN);
				}
				else
				{
			
					Action(wBackward, lParam);
					Action(wFake, lParam);
					Action(wBackward, lParam);
				}
				if (bDefenseDown)
			{
				Action(VK_SPACE, 0, ACTION_DOWN);
			}
			return 1;
		}
	}
	return 0;
}
int Next(WPARAM wParam)
{
	return wParam;
}
void Action(WPARAM wParam, LPARAM lParam, LPARAM bDown)
{
	switch(wParam)
	{
	case MOUSE_LBUTTON:	
		{
			switch (bDown)
			{
			case ACTION_BOTH:
				SendMessage(hMeteor, WM_LBUTTONDOWN, 0, 0);
				SendMessage(hMeteor, WM_LBUTTONUP, 0, 0);
				break;
			case ACTION_DOWN:
				SendMessage(hMeteor, WM_LBUTTONDOWN, 0, 0);
				break;
			case ACTION_UP:
				SendMessage(hMeteor, WM_LBUTTONUP, 0, 0);
				break;
			}
			break;
		}
	case MOUSE_RBUTTON:
		{
			switch (bDown)
			{
			case ACTION_BOTH:
				SendMessage(hMeteor, WM_RBUTTONDOWN, 0, 0);
				SendMessage(hMeteor, WM_RBUTTONUP, 0, 0);
				break;
			case ACTION_DOWN:
				SendMessage(hMeteor, WM_RBUTTONDOWN, 0, 0);
				break;
			case ACTION_UP:
				SendMessage(hMeteor, WM_RBUTTONUP, 0, 0);
				break;
			}
			break;
		}
	default:
		{
			switch (bDown)
			{
			case ACTION_BOTH:
				SendMessage(hMeteor, WM_KEYDOWN, wParam, lParam);
				SendMessage(hMeteor, WM_KEYUP, wParam, lParam);
				break;
			case ACTION_DOWN:
				SendMessage(hMeteor, WM_KEYDOWN, wParam, lParam);
				break;
			case ACTION_UP:
				SendMessage(hMeteor, WM_KEYUP, wParam, lParam);
				break;
			}
			break;
		}
	}
}
void KeyInput(WPARAM wParam, LPARAM lParam)
{
	KeyInputEx( wParam, lParam );
}
void KeyInputEx(WPARAM wParam, LPARAM lParam)
{
	INPUT it;
	it.type = INPUT_KEYBOARD;
	it.ki.wVk = wParam;
	it.ki.wScan = 0;
	it.ki.dwFlags = 0;
	it.ki.time = 0;
	it.ki.dwExtraInfo = 0;

	SendInput( 1, &it, sizeof( INPUT ));

	if ( lParam )
	{
		INPUT itp;
		itp.type = INPUT_KEYBOARD;
		itp.ki.wVk = wParam;
		itp.ki.wScan = 0;
		itp.ki.dwFlags = KEYEVENTF_KEYUP;
		itp.ki.time = 0;
		itp.ki.dwExtraInfo = 0;

		SendInput( 1, &itp, sizeof( INPUT ));
	}
}
void ChangeWeapon(WPARAM wParam)
{
	char buffer[30];
	sprintf(buffer, "WEAPON %d", wParam);
	Command(buffer);

}
void Command(char * s)
{
	keybd_event( VK_OEM_5, 0, 0, 0);
	keybd_event( VK_OEM_5, 0, KEYEVENTF_KEYUP, 0);
	//Sleep(1000);
	for(int i=0; i<strlen(s); i++)
	{
		keybd_event( s[i], 0, 0, 0);
		keybd_event( s[i], 0, KEYEVENTF_KEYUP, 0);
	}

	keybd_event( VK_RETURN, 0, 0, 0);
	keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}
void Chat(char *s, bool in)
{
	if (in)
	{
		keybd_event( 'O', 0, 0, 0);
		keybd_event( 'O', 0, KEYEVENTF_KEYUP, 0);
	}
	else
	{
		keybd_event( 'P', 0, 0, 0);
		keybd_event( 'P', 0, KEYEVENTF_KEYUP, 0);
	}

	for(int i=0; i<strlen(s); i++)
	{
		keybd_event( s[i], 0, 0, 0);
		keybd_event( s[i], 0, KEYEVENTF_KEYUP, 0);
	}

}
void Play()
{
	Command("PLAY");

}
void Pose(char *s)
{
	keybd_event( VK_OEM_5, 0, 0, 0);
	keybd_event( VK_OEM_5, 0, KEYEVENTF_KEYUP, 0);
	keybd_event( 'P', 0, 0, 0);
	keybd_event( 'P', 0, KEYEVENTF_KEYUP, 0);
	keybd_event( 'O', 0, 0, 0);
	keybd_event( 'O', 0, KEYEVENTF_KEYUP, 0);
	keybd_event( 'S', 0, 0, 0);
	keybd_event( 'S', 0, KEYEVENTF_KEYUP, 0);
	keybd_event( 'E', 0, 0, 0);
	keybd_event( 'E', 0, KEYEVENTF_KEYUP, 0);
	keybd_event( ' ', 0, 0, 0);
	keybd_event( ' ', 0, KEYEVENTF_KEYUP, 0);

	for(int i=0; i<strlen(s); i++)
	{
		keybd_event(s[i], 0, 0, 0);
		keybd_event(s[i], 0, KEYEVENTF_KEYUP, 0);
	}

	keybd_event( VK_RETURN, 0, 0, 0);
	keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}
void Pose(int wParam)
{
	char buffer[20];
	Pose(_itoa(wParam, buffer, 10));

}
void TurnLeft90()
{
	Turn(-400);
}
void TurnRight90()
{
	Turn(300);
}
void TurnLeft45()
{
	Turn(-150);
}
void TurnRight45()
{
	Turn(150);
}
void Turn(int radis)
{
	INPUT it;
	it.type = INPUT_MOUSE;
	it.mi.dx = radis;
	it.mi.dy = 0;
	it.mi.mouseData = 0;
	it.mi.dwFlags = MOUSEEVENTF_MOVE;
	it.mi.time = 0;
	it.mi.dwExtraInfo = 0;

	SendInput( 1, &it, sizeof( INPUT ));
}
VOID CALLBACK TimerEventProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	LPARAM lParam = 0;

	if (KEYDOWN('W'))
		bWDown = TRUE;
	else
		bWDown = FALSE;


	if (KEYDOWN(VK_SPACE))
		bDefenseDown = TRUE;
	else
		bDefenseDown = FALSE;

	if (bDefenseDown)
		Action(VK_SPACE, lParam, ACTION_UP);
	switch (dwUser)
	{
	case 105:
	{
		timeKillEvent(mmTimer);
		Action(wJump);
		Action(wAttack);
		break;
	}
	case 104:
	{
		Action(wJump);
		Action(wJump);
		keybd_event('W', 0, 0, 0);
	}
	case 103:
		keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
		Action(wWeaponchange);
		SET_TIMER_EVENT(27, 320)
			break;
	case 102:
	{
		//keybd_event('W', 0, 0, 0);
		//keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
		Action(wJump, 0, 0);
		SET_TIMER_EVENT(103, 7);
		break;
	}
	case 101:
	{
		if (KEYDOWN('W'))
			keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
		Action(wBackward);
		Action(wForward);
		Action(wAttack);
		if (KEYDOWN('W'))
			keybd_event('W', 0, 0, 0);
		break;
	}
	case 100:
		{
			timeKillEvent(mmTimer);
			if (nWeapon == 9)
			{
				Action(wBackward);
				Action(wAttack);
			}
			else if (nWeapon == 11)
			{
				Action(wForward);
				Action(wBackward);
				Action(wAttack);
			}
			break;
		}
	case 1:
		Action(wAttack, lParam);

		timeKillEvent(mmTimer);
		Action(wLeftstep, lParam, ACTION_UP);


		break;
	case 2:
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		timeKillEvent(mmTimer);
		break;
	case 3:
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		timeKillEvent(mmTimer);
		break;
	case 4:
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		timeKillEvent(mmTimer);
		break;
	case 5:
		timeKillEvent(mmTimer);
		break;
	case 6:
		Action(wForward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		timeKillEvent(mmTimer);
		SET_TIMER_EVENT(7, 700)
		break;
	case 7:
		Action(wAttack, lParam);
		
		timeKillEvent(mmTimer);
		break;
	case 8:
		Action(wLeftstep, lParam);
		
		Action(wAttack, lParam);
		
		timeKillEvent(mmTimer);
		break;
	case 9:
		Action(wLeftstep, lParam);
		
		Action(wRightstep);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		break;
	case 10:
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		

		break;
	case 11:
		Action(wBackward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		break;
	case 12:
		Action(wBackward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		break;
	case 13:
		Action(wRightstep);
		
		Action(wRightstep);
		
		Action(wAttack, lParam);
		

		
		timeKillEvent(mmTimer);
		SET_TIMER_EVENT_EVER(12, 80)
		break;

	case 14:
		Action(wForward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		break;
	case 15:
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		break;
	case 16:
		Action(wBackward, lParam);
		
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		break;
	case 17:
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		

		timeKillEvent(mmTimer);
		SET_TIMER_EVENT(18, 500);
		break;
	case 18:
		//Action(VK_F12, lParam, ACTION_DOWN);
		Action(wBackward, lParam);
		
		Action(wForward, lParam);
		
		Action(wAttack, lParam);
		
		
		timeKillEvent(mmTimer);


		SET_TIMER_EVENT_EVER(32, 100+wMilSec);

		break;
	case 19:
		Action(wBackward, lParam);
		
		Action(wAttack, lParam);
		
		timeKillEvent(mmTimer);
		break;
	case 20:
		Action(wAttack, lParam);
		
		Action(wAttack, lParam);
		
		timeKillEvent(mmTimer);
		break;
	case 21:
		Action(wWeaponchange);
		
		timeKillEvent(mmTimer);
		SET_TIMER_EVENT(22, 350);
		break;
	case 22:
		Action(wBackward, lParam);
		Action(wAttack, lParam);
		timeKillEvent(mmTimer);
		break;
	case 23:
		Action(VK_ESCAPE);
		Action(VK_ESCAPE);
		Action(wBackward, lParam);
		Action(wAttack, lParam);
		timeKillEvent(mmTimer);
		break;
	case 24:
		Action(VK_ESCAPE);
		Action(VK_ESCAPE);
		timeKillEvent(mmTimerEx);
		break;
	case 25:
		Action(wWeaponchange);
		timeKillEvent(mmTimer);
		break;
	case 26:
		Action(wWeaponchange);
		timeKillEvent(mmTimer);
		break;
	case 27:
		Action(wBackward, lParam, ACTION_DOWN);
		Action(wAttack, lParam);
		Action(wBackward, lParam, ACTION_UP);
		timeKillEvent(mmTimer);
		break;
	case 28:
		//if (KEYDOWN('W'))
		//	keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
		Action(wWeaponchange);
		SET_TIMER_EVENT(27, 320)
		break;
	case 29:
		{
				Action(wBackward, lParam);
				Action(wBackward, lParam);
				Action(wForward, lParam);
				Action(wAttack, lParam);
				timeKillEvent(mmTimer);
				break;
		}
	case 30:
		{
				Action(wBackward, lParam);
				Action(wForward, lParam);
				Action(wForward, lParam);
				Action(wAttack, lParam);
				timeKillEvent(mmTimer);
				break;
		}
	case 31:
		{
				Action(wLeftstep, lParam);
				Action(wRightstep, lParam);
				Action(wBackward, lParam);
				Action(wAttack, lParam);
				timeKillEvent(mmTimer);
				break;
		}
	case 32:
		{
			//Action(VK_F12, lParam, ACTION_UP);
			Action(wAttack, lParam);
			timeKillEvent(mmTimer);
			if ( wMilSec == 0 )
			{
				SET_TIMER_EVENT(33, 100);
			}
			else
			{
				SET_TIMER_EVENT_EVER(33, 1);
			}
			
			break;
		}
	case 33:
		{
			Action(wBackward, lParam);
			Action(wAttack, lParam);
			timeKillEvent(mmTimer);
			break;
		}
	case 34:
		{
			Action(wJieSuo, lParam);
			Action(wBackward, lParam);
			Action(wBackward, lParam);
			Action(wAttack, lParam);
			timeKillEvent(mmTimer);
			break;
		}
	case 35:
		{
			Action(wBackward, lParam);
			Action(wAttack, lParam);
			timeKillEvent(mmTimer);
			break;
		}
	case 36:
		{
			Action(wBackward, lParam);
			Action(wBackward, lParam);
			Action(wAttack, lParam);
			timeKillEvent(mmTimer);
			break;
		}
	case 37:
		{
			Action(wAttack, lParam);

			timeKillEvent(mmTimer);
			SET_TIMER_EVENT(38, 700);
			break;
		}
	case 38:
		{
			Action(wRightstep, lParam);
			Action(wFake, lParam);
			Action(wRightstep, lParam);
			timeKillEvent(mmTimer);
			break;
		}
	case 39:
		{
			Action(wJump, lParam);
			SET_TIMER_EVENT(28, 100);
			break;
		}
	case 40:
		{
			Action(wJump, lParam);
			SET_TIMER_EVENT(41, 100);
			break;	
		}
	case 41:
		{
			KeyInput(wLeftstep, 1);
			Action(wBackward, lParam);
			Action(wAttack, lParam);
			timeKillEvent(mmTimer);
			break;
		}
	case 42:
		{
			Action(wJump, 0, ACTION_DOWN);
			SET_TIMER_EVENT(46, 600);
			break;
		}
	case 43:
		{
			Action(wJump, 0, ACTION_DOWN);
			SET_TIMER_EVENT(47, 600);
			break;
		}
	case 44:
		{
			if (bWDown)
				keybd_event('W', 0, 0, 0);
			keybd_event('S', 0, KEYEVENTF_KEYUP, 0);
			timeKillEvent(mmTimer);
			break;
		}
	case 45:
		{	
			Action(wWeaponchange, 0, ACTION_DOWN);

			SET_TIMER_EVENT(48, 100);
			break;
		}
	case 46:
		{
			Action(wJump, 0, ACTION_UP);
			KeyInput(wLeftstep, 1);

			timeKillEvent(mmTimer);
			break;
		}
	case 47:
		{
			Action(wJump, 0, ACTION_UP);
			KeyInput(wRightstep, 1);

			timeKillEvent(mmTimer);
			break;
		}
	case 48:
		{
			Action(wWeaponchange, 0, ACTION_UP);

			if (bWDown)
				Action(wForward, 0, ACTION_DOWN);

			timeKillEvent(mmTimer);
			break;
		}
	case 49:
		{
			Action(wJump, 0, ACTION_UP);
			SET_TIMER_EVENT(45, 1);
			//timeKillEvent(mmTimer);
			break;
		}
	case 50:
		{
			KeyInput(wRightstep, 1);

			timeKillEvent(mmTimer);
			break;
		}
	case 51:
		{
			for (int i = 0; i < wMilSec; i++)
				Action(wJump);
			keybd_event('W', 0, 0, 0);
			//SET_TIMER_EVENT(52, 10)
			break;
		}
	case 52:
		{
			Action(wJump, 0, ACTION_UP);
			timeKillEvent(mmTimer);
			break;
		}
	}
	
	if (bDefenseDown)
		Action(VK_SPACE, lParam, ACTION_DOWN);
}
VOID CALLBACK TimerOutProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	if (mmTimer)
	{
		timeKillEvent(mmTimer);
		mmTimer = NULL;
	}
	if (mmTimerEx)
	{
		timeKillEvent(mmTimerEx);
		mmTimerEx = NULL;
	}
	timeKillEvent(mmTimerOut);
	mmTimerOut = NULL;
}
DWORD WINAPI ThreadProc(LPVOID)
{
	while (true)
	{
		if (bStartWeaponEx)
		{
			if (nWeapon == 5 && KEYDOWN('X'))
			{
				if (bDefenseDown)
				{
					Action(VK_SPACE, 0, ACTION_UP);
				}
				
				Action(wJump);
				Action(wAttack);
				if (bDefenseDown)
				{
					Action(VK_SPACE, 0, ACTION_DOWN);
				}
			}
		}
	}
	
	return 0;
}

void UnAttack(BOOL bYes)
{
	unsigned int iAddress = (unsigned int)GetModuleHandle(NULL);
	iAddress += 0xba715;
	
	if (bYes)
	{
		byte str[11] = {0x0F, 0x84, 0x5B, 0x05, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90};
		SIZE_T ilen = 0;
		BOOL bRet = WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)iAddress, str, 11, &ilen);
		//memcpy((void *)iAddress, str, 11);

		char sMsg[255] = "";
		sprintf(sMsg, "address is 0x%x. with ret 0x%x, real len 0x%x", iAddress, bRet, ilen);
	}
	else
	{
		byte str[11] = {0x8B, 0x16, 0x57, 0x8B, 0xCE, 0xFF, 0x92, 0xF4, 0x00, 0x00, 0x00};
		//memcpy((void *)iAddress, str, 11);
		SIZE_T ilen = 0;
		BOOL bRet = WriteProcessMemory(INVALID_HANDLE_VALUE, (void *)iAddress, str, 11, &ilen);
	}
}

//iBase is the base address of meteor.exe
bool GetPosition(int iBase, float &x, float &y, float &z)
{
	//get x
	int offset0 = *((int *)(iBase + 0x000042ec)) + 0xfc;
	int offset1 = *((int *)offset0) + 0x668;
	int offset2 = *((int *)offset1) + 0x38;
	int offset3 = *((int *)offset2) + 0x240;
	int offset4 = *((int *)offset3) + 0x6a0;
	x = *((float *)offset4);

	//char str[255] = {0};
	//sprintf(str, "get x position: %f\n", x);
	//WriteConfig(str);
	
	//get y
	offset0 = *((int *)(iBase + 0x000042ec)) + 0xfc;
	offset1 = *((int *)offset0) + 0x38;
	offset2 = *((int *)offset1) + 0xe0;
	offset3 = *((int *)offset2) + 0x20;
	offset4 = *((int *)offset3) + 0x4ec;
	y = *((float *)offset4);

	//sprintf(str, "get y position: %f\n", y);
	//WriteConfig(str);

	//get z
	offset0 = *((int *)(iBase + 0x000042ec)) + 0xfc;
	offset1 = *((int *)offset0) + 0x668;
	offset2 = *((int *)offset1) + 0x38;
	offset3 = *((int *)offset2) + 0x78;
	offset4 = *((int *)offset3) + 0x1bc;
	z = *((float *)offset4);

	//sprintf(str, "get z position: %f\n", z);
	//WriteConfig(str);

	return true;
}