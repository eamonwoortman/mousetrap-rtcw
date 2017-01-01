#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#include <windows.h>
#include <wininet.h>

#include "web.h"

#include <q_shared.h>

#include "util.h"
#include "main.h"
#include "proxydll.h"
using namespace openutils;

extern int ServerTime;

Banlist_t gBanlist[200];
// - Banlist Setup:
int		numBanned = 0;

char	BanlistBuffer[300][30];
float	ClientVersion;

//http://www.codeproject.com/KB/IP/CppHttpWebForm.aspx	
// This is universal:
void ProcessBanlist( void ){
	int i;
	char tempguid[40];
	char tempip[40];
	for (i = 0; i < numBanned; i++){
		strcpy(gBanlist[i].guid, BanlistBuffer[i]);
	}
}

void GetFileContents( void )
{
	try {
		WebForm wf;
		wf.setHost("http://www.dutchmeat.net");
		wf.setScriptFile("/mousetrap/getbanlist.php"); 
		// or wf.setScriptFile(""); 
		wf.sendRequest();

		char *pch;
		int j = 0;

		char buff[10 * 1024];
		if(wf.getResponse(buff,sizeof(buff))) 
		{
			//g_syscall(G_PRINT, va("Mousetrap: Test response '%s'\n",buff));

			pch = strtok (buff,"\n");

			while (pch != NULL)
			{
				strcpy(BanlistBuffer[j], pch);
				pch = strtok (NULL, "\n");
				
				j++;
			}
			numBanned = j;
		}
		else 
		{
			//cout << "No response from server" << endl;
			g_syscall(G_PRINT, va("Mousetrap: No response from server\n"));
		}
	}catch(WebFormException ex) {
		//cout << ex.getMessage() << endl;
		g_syscall(G_PRINT, va("Mousetrap: GetBanlist ERROR: %s\n",ex.getMessage()));
	}
	g_syscall(G_PRINT, va("Mousetrap: Banlist loaded(%d)\n",numBanned));
}

void GetMTVersion( void )
{
	try {
		WebForm wf;
		wf.setHost("http://www.dutchmeat.net");
		wf.setScriptFile("/mousetrap/mtversion.php"); 
		// or wf.setScriptFile(""); 
		wf.sendRequest();

		char *pch;
		int j = 0;

		char buff[1 * 1024];
		if(wf.getResponse(buff,sizeof(buff))) 
		{
			//g_syscall(G_PRINT, va("Mousetrap: Test response '%s'\n",buff));
			ClientVersion = atoi(buff);
			//sprintf(ClientVersion, "%d", atoi(buff));
			g_syscall(G_PRINT, va("Mousetrap: Test response '%f'\n",ClientVersion));
			if (ClientVersion == 31) 
				g_syscall(G_PRINT, "Mousetrap: YAY\n");
			else
				g_syscall(G_PRINT, "Mousetrap: NO!\n");
		}
		else 
		{
			g_syscall(G_PRINT, va("Mousetrap: No response from server\n"));
		}
	}catch(WebFormException ex) {
		g_syscall(G_PRINT, va("Mousetrap: Getversion ERROR: %s\n",ex.getMessage()));
	}
}

//Function to convert unsigned char to string of length 2
void Char2Hex(unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch%16;
	for(int i=0; i<2; i++)
	{
		if(byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}
//Function to convert string of length 2 to unsigned char
void Hex2Char(char const* szHex, unsigned char& rch)
{
	rch = 0;
	for(int i=0; i<2; i++)
	{
		if(*(szHex + i) >='0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else
			break;
	}
}   

//Function to convert string of unsigned chars to string of chars
void CharStr2HexStr(unsigned char const* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

//Function to convert string of chars to string of unsigned chars
void HexStr2CharStr(char const* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i;
	unsigned char ch;
	for(i=0; i<iSize; i++)
	{
		Hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}

void ReverseStr ( char *buff, int start, int end ) 
{ 
	char tmp ; 

	if ( start >= end ) 
	{ 
		printf ( "\n%s\n", buff ); 
		return; 
	} 

	tmp = *(buff + start); 
	*(buff + start) = *(buff + end); 
	*(buff + end) = tmp ; 

	ReverseStr (buff, ++start, --end ); 
} 


void sendBan( char *guid, char *name, char *cheats )
{
	char banStr[100];
	char dataStr[100];
	try {
		WebForm wf;
		wf.setHost("http://dutchmeat.net");
		wf.setScriptFile("/mousetrap/sendBan.php"); 
		// or wf.setScriptFile(""); 
	//g_syscall(G_PRINT,va("LEN '%d'\n", (strlen(guid) + strlen(name) + strlen(cheats))));
		strcpy(banStr, va("%s|%s|%s",guid,name,cheats));

		//g_syscall(G_PRINT,va("websend 1 '%s'", banStr));

		CharStr2HexStr((unsigned char*)banStr, dataStr, strlen(banStr));
		ReverseStr(dataStr,0,strlen(dataStr)-1); 

		//g_syscall(G_PRINT,va("websend 2 '%s'", dataStr));


		wf.putVariable("dataStr",dataStr);
		wf.sendRequest();

		char *pch;
		int j = 0;

		char buff[10 * 1024];
		if(wf.getResponse(buff,sizeof(buff))) 
		{

			g_syscall(G_PRINT,va("updating ban: '%s'\n", buff));	

		}
		else 
		{
			//cout << "No response from server" << endl;
			g_syscall(G_PRINT, va("Mousetrap: No response from server\n"));
		}
	}catch(WebFormException ex) {
		//cout << ex.getMessage() << endl;
		g_syscall(G_PRINT, va("Mousetrap: Ban ERROR: %s\n",ex.getMessage()));
	}

	//g_syscall(G_PRINT,va("websend done"));
}
