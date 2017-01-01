// - ST It is actually C, not C++

//#include "common.h" // - ST
#include "stdafx.h"
#include "Hook.h"
#include "proxydll.h"
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")

char SithBusterGUIDString[5000];

// - ST Woo HOO!!! I created this!
// NOTE!!! It will ONLY work if the Destination already has SOMETHING there, even if it's just a space!
char *stradd( char *strDestination, const char *strSource )
{
	int i = 0;
	int b = 0;

	i = strlen( strDestination );

	while ( *strDestination > i && *strSource ) {
		strDestination[i] = strSource[b];
		i++;
		b++;
	}
	return strDestination;
}

// - ST I'm adding this:
void lowercase(char string[])
{
   int  i = 0;

   while ( string[i] )
   {
      string[i] = tolower(string[i]);
	  i++;
   }
   return;
}

// Prints the MAC address stored
/*
void PrintMACaddress(unsigned char MACData[])
{

Originally, I just had it print MAC address, but I didn't want people to know what is is based on so I switched it up
by the new PrintMACaddress function below

	stradd( SithBusterGUIDString, va("%02X%02X.%02X%02X.%02X%02X ", // - ST
		MACData[5], MACData[0], MACData[1], MACData[3], MACData[2], MACData[4]) );
}
*/


void PrintMACaddress(unsigned char MACData[])
{
	char StringPart1[100];
	char StringPart2[100];
	char FinalString[100];
	long DecimalGUIDPart1;
	long DecimalGUIDPart2;
	long FinalDecimal;

	// The limit for strtol is 8 digits. Therefore we'll break it into two parts, and encrypt it:

	// First, we get the capitalized hex MAC address, in two parts, already a little scrambled:
	strcpy( StringPart1, va("%02X%02X%02X%02X",
		MACData[5], MACData[0], MACData[2], MACData[3] ) );
	strcpy( StringPart2, va("%02X%02X%02X%02X",
		MACData[2], MACData[4], MACData[0], MACData[1] ) );

	// To convert to decimal, it must be lowercase:
	lowercase( StringPart1 );
	lowercase( StringPart2 );

	// Convert to decimal:
	DecimalGUIDPart1 = strtol(StringPart1, NULL, 16);
	DecimalGUIDPart2 = strtol(StringPart2, NULL, 16);	

//	Com_Printf( va("1st Decimal: %i, from this hex: %s\n", DecimalGUIDPart1, StringPart1 ) );
//	Com_Printf( va("2nd Decimal: %i, from this hex: %s\n", DecimalGUIDPart2, StringPart2 ) );

	// Encrypt each:
	DecimalGUIDPart1 -= 234921;
	DecimalGUIDPart2 -= 195062;

	// Add together:

	FinalDecimal = DecimalGUIDPart1 * DecimalGUIDPart2;
	// Com_Printf( va("Decimal1: %i, Decimal2: %i, Final: %i\n", DecimalGUIDPart1, DecimalGUIDPart2, FinalDecimal ) );

	// End result:

	itoa(  FinalDecimal, FinalString, 16 );
	//orig_syscall(CG_PRINT, va("DEBUG!!!!\n '%s'\n",FinalString));
    strcat( SithBusterGUIDString, va( "%s", FinalString ) );
}

// Fetches the MAC address and prints it
void GetMACaddress(void)
{
	strcpy(SithBusterGUIDString, "");
	IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
	PIP_ADAPTER_INFO pAdapterInfo;
	DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
		AdapterInfo,							// [out] buffer to receive data
		&dwBufLen);								// [in] size of receive data buffer
	assert(dwStatus == ERROR_SUCCESS);			// Verify return value is valid, no buffer overflow

	pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info

	do {
		PrintMACaddress(pAdapterInfo->Address);	// Print MAC address
		pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
	}
	while(pAdapterInfo);						// Terminate if last adapter
}