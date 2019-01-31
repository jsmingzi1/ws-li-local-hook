#include <Windows.h>
#include <stdio.h>
#include "Config.h"
#include "GlobalVariable.h"

VOID ReadConfig()
{
	FILE *stream = freopen("user.cfg", "r", stdin);
	if (stream == NULL)
	{
		return;
	}
	char s[255];
	char key[255];
	
	while(gets_s(s))
	{
		memset(key, 0, sizeof(key));
		int value = 0;
		bool bKey = true;
		int j=0;
		for(int i=0; i<strlen(s); i++)
		{
			if(bKey)
			{
				if(s[i] != ' ')
				{
					key[j++] = s[i];
				}
				else
				{
					bKey = false;
				}
			}
			else
			{
				value = value * 10 + s[i] - '0';
			}
		} // end for

		if (strcmp(key, "FORWARD") == 0)
		{
			wForward = value;
		}
		else if (strcmp(key, "BACKWARD") == 0)
		{
			wBackward = value;
			
		}
		else if (strcmp(key, "RIGHTSTEP") == 0)
		{
			wRightstep = value;
			
		}
		else if (strcmp(key, "LEFTSTEP") == 0)
		{
			wLeftstep = value;
			
		}
		else if (strcmp(key, "ATTACK") == 0)
		{
			wAttack = value;
			
		}
		else if (strcmp(key, "JUMP") == 0)
		{
			wJump = value;
			
		}
		else if (strcmp(key, "FAKE") == 0)
		{
			wFake = value;
			
		}
	}	// end while

	fclose(stream);
}

float  ReadSpeed()
{
	FILE *stream = freopen("speed.cfg", "r", stdin);
	if(stream == NULL)
		return 1.0;
	float s;
	scanf("%f", &s);
	fclose(stream);
	return s;
}
VOID WriteConfig()
{
	FILE *stream = freopen("log.txt", "a", stdout);
	if(stream == NULL)
		return;
	printf("\n\n\nThis is the MeteorHook.dll log \nbegin++++\n");
	printf("FORWARD: %d\n", wForward);
	printf("BACKWARD: %d\n", wBackward);
	printf("LEFTSTEP: %d\n", wLeftstep);
	printf("RIGHTSTEP: %d\n", wRightstep);
	printf("ATTACK: %d\n", wAttack);
	printf("JUMP: %d\n", wJump);
	printf("FAKE: %d\n", wFake);
	printf("end++++\n\n\n");
	fclose(stream);
}

VOID WriteConfig(LPCTSTR s)
{
	FILE *stream = freopen("log_timer.txt", "a", stdout);
	if(stream == NULL)
		return;
	printf(s);
	printf("\n");
	fclose(stream);
}

VOID WriteConfig(LPCWSTR WStr)
{
	size_t len = wcslen(WStr) + 1;
	size_t converted = 0;
	char *CStr;
	CStr=(char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, CStr, len, WStr, _TRUNCATE);

	WriteConfig(CStr);
}