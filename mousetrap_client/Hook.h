///////////////////////////
//////////CREDITS//////////
///////////////////////////
//-mombatu/////////////////
//-me       ///////////////
///////////////////////////
///////////////////////////
///////////////////////////
#include <windows.h>
#include "detours.h"
#include <math.h>
#include <vector>
#include <fstream>


#include "Engine/q_shared.h"

//#include "CG_Hook.h"


#define DEBUGGING 1

#define PASSFLOAT( x ) (*(int*)&x)
#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}

extern int startlog;

extern char MTGuid[5000];

//hooked stuff
void CG_ServerCommand_Hook(  );
extern void (WINAPIV *orig_CG_ServerCommand)  (  );

extern void (WINAPIV *orig_CG_Cvar_Update)  ( vmCvar_t *vmCvar );

typedef struct {
    vmCvar_t *vmCvar;
    char *cvarName;
    char *defaultString;
    int cvarFlags;
} cvarTable_t;




typedef struct CheatsOffset_s {
	char		md5[64];
	char		cheatName[32];
	int			fileSize;
} CheatsOffset_t;

#define ArrayCount(x)  sizeof(x)/sizeof(x[0]);


int   CG_vmMain_Hook	( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );
extern int  (WINAPIV *orig_CG_vmMain)  ( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );

void trap_CG_Cvar_Register( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags );
void trap_CG_Cvar_Update( vmCvar_t *vmCvar );
void CG_RegisterCvars();


extern vmCvar_t	mt_cvars;
extern vmCvar_t	mt_guid;
extern void GetMACaddress (void);

#define ArraySize(x) (sizeof(x) / sizeof(x[0]))

extern int checkHacks(void);

extern char cheats[100];

extern DWORD wolfPid;

void mLog(const char *format, ...);

extern char HashofMTFile	[400];
extern cvarTable_t cvarTable[];

