// proxydll.h
#pragma once
#include "detours.h"
#include "engine/cg_local.h"
// regular functions
void InitInstance(HANDLE hModule);
void ExitInstance(void);
void LoadOriginalDll(void);

extern int  (WINAPIV *orig_CG_vmMain)  ( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );

#pragma warning( disable : 4100 )
#define	QDECL	__cdecl

typedef INT (WINAPIV* VMMAIN_FUNC)(INT,INT,INT,INT,INT,INT,INT,INT,INT,INT,INT,INT,INT);
#define PASSFLOAT( x ) (*(int*)&x)
#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}

extern void CG_ServerCommand_Hook(void);
//extern	int	 (WINAPIV *orig_syscall)		( INT command, ... );
//extern int (QDECL *orig_syscall)( int arg, ... );
//extern int orig_syscall( int arg, ... );
extern int  (WINAPIV *orig_syscall)  ( int command, ... );
INT				CG_vmMain_Hook					( INT command, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5, INT arg6, INT arg7, INT arg8, INT arg9, INT arg10, INT arg11 );
//extern	INT		(WINAPIV *orig_CG_vmMain)		( INT command, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5, INT arg6, INT arg7, INT arg8, INT arg9, INT arg10, INT arg11 );