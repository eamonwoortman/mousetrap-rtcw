// proxydll.h
#pragma once

#include "g_local.h"
// regular functions
void InitInstance(HANDLE hModule);
void ExitInstance(void);
void LoadOriginalDll(void);

extern int  (WINAPIV *orig_CG_vmMain)  ( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);

#pragma warning( disable : 4100 )
#define	QDECL	__cdecl

typedef INT (WINAPIV* VMMAIN_FUNC)(INT,INT,INT,INT,INT,INT,INT,INT);
#define PASSFLOAT( x ) (*(int*)&x)
#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}
void mLog(const char *format, ...);
extern void CG_ServerCommand_Hook(void);
//extern	int	 (WINAPIV *orig_syscall)		( INT command, ... );
//extern int (QDECL *orig_syscall)( int arg, ... );
//extern int orig_syscall( int arg, ... );
extern int  (WINAPIV *g_syscall)  ( int command, ... );
INT				CG_vmMain_Hook					( INT command, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5, INT arg6 );
//extern	INT		(WINAPIV *orig_CG_vmMain)		( INT command, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5, INT arg6, INT arg7, INT arg8, INT arg9, INT arg10, INT arg11 );

#define MAX_STRING_LENGTH 1024
#define MAX_DATA_LENGTH 200
#define MAX_COMMAND_LENGTH 64
#define MAX_NUMBER_LENGTH 11

#define MAX_USER_ENTRIES 64

//#define MAX_NETNAME (35 + 1)
#define MAX_USER_LENGTH MAX_NETNAME
#define MAX_PASS_LENGTH 32 + 1
#define MAX_IP_LENGTH 15 + 1

#ifdef WIN32
 #define strcasecmp stricmp
#endif

#define ArraySize(x) (sizeof(x) / sizeof(x[0]))

struct SCvars_t{
	int	min_val;
	int max_val;
	bool equal;
};

struct Banlist_t{
	char guid[40];
//	char ipAddr[40];
};

typedef struct playerinfo_s {
//	char ip[MAX_IP_LENGTH];
	char name[MAX_NETNAME];
	char stripname[MAX_NETNAME];
	bool connected;
	bool mt_checked;
	bool clean;
	int ping;
	int failed;
	char guid[60];
	char cvars[4];
	int restrictedCvar;
	char ipaddress[50];
} playerinfo_t;

extern playerinfo_t g_playerinfo[];

void GetFileContents( void );
void ProcessBanlist( void );
void GetMTVersion (void);

extern Banlist_t gBanlist[200];

extern int numBanned;

void sendBan( char *guid, char *name, char *cheats );

//extern Banlist_t bBanlist[];

#define MAX_CVARS 4

struct mtCvars_t {
	char name[30];
	char defaultValue[60];
};

extern mtCvars_t mtCvars[MAX_CVARS];

int QMM_GETINTCVAR (char *cvar_name);
//char *QMM_GETSTRCVAR(char *cvar_name);
const char* QMM_GETSTRCVAR(const char* cvar);

void HexStr2CharStr(char const* pszHexStr, unsigned char* pucCharStr, int iSize);