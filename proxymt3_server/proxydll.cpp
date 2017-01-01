// proxydll.cpp
#include "stdafx.h"
#include "proxydll.h"
#define mb(x) MessageBox(NULL, x,"MT3",MB_OK);
// global variables
HINSTANCE           gl_hOriginalDll;
HINSTANCE           gl_hThisInstance;

//static int (QDECL *orig_syscall)( int arg, ... ) = (int (QDECL *)( int, ...))-1;
int (WINAPIV *g_syscall) ( int command, ... );
//static INT		(WINAPIV *orig_syscall)		( INT command, ... );
//static int (QDECL *orig_syscall)( int arg, ... );
FILE	*logFile;

int QMM_GETINTCVAR (char *cvar_name){
	return g_syscall( G_CVAR_VARIABLE_INTEGER_VALUE, cvar_name );
}


//this uses a cycling array of strings so the return value does not need to be stored locally
#define MAX_CVAR_LEN	256
const char* QMM_GETSTRCVAR(const char* cvar) {
	if (!cvar || !*cvar)
		return NULL;

	static char temp[8][MAX_CVAR_LEN];
	static int index = 0;
	int i = index;

	g_syscall(G_CVAR_VARIABLE_STRING_BUFFER, cvar, temp[i], sizeof(temp[i]));
	index = (index + 1) & 7;
	return temp[i];
}

void mLog(const char *format, ...) {
		
	char buffer[4096];
	fopen_s(&logFile, "mousetrap_server.log", "a+");
	va_list va;
	va_start(va, format);
		vsprintf_s(buffer, sizeof(buffer), format, va);
	va_end(va);

	
	fprintf_s(logFile, "%s\n", buffer);

	fclose(logFile);
	
}
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	// to avoid compiler lvl4 warnings 
    LPVOID lpDummy = lpReserved;
    lpDummy = NULL;
    
    switch (ul_reason_for_call)
	{
	    case DLL_PROCESS_ATTACH: 
			InitInstance(hModule); 

			mLog("S1"); 
			break;
	    case DLL_PROCESS_DETACH: 

			ExitInstance(); 
			break;
        
        case DLL_THREAD_ATTACH: 
			//mLog("Thread Attach!!\n"); 
			break;// WOOHOOO
	    case DLL_THREAD_DETACH:  break;
	}
    return TRUE;
}
//INT (WINAPIV *orig_CG_vmMain) ( INT command, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5, INT arg6, INT arg7, INT arg8, INT arg9, INT arg10, INT arg11 );
extern void  (WINAPIV *orig_CG_ServerCommand) ();

char	* QDECL va( char *format, ... ) {
	va_list		argptr;
	#define	MAX_VA_STRING	32000
	static char		temp_buffer[MAX_VA_STRING];
	static char		string[MAX_VA_STRING];	// in case va is called by nested functions
	static int		index = 0;
	char	*buf;
	int len;


	va_start (argptr, format);
//	vsprintf (temp_buffer, format,argptr);
	vsprintf_s(temp_buffer, MAX_VA_STRING-1, format, argptr);
	va_end (argptr);

	len = (int)strlen(temp_buffer);

	if (len + index >= MAX_VA_STRING-1) {
		index = 0;
	}

	buf = &string[index];
	memcpy( buf, temp_buffer, len+1 );

	index += len + 1;

	return buf;
}

int vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6 ) {


	if (!gl_hOriginalDll) {
		LoadOriginalDll();
		mLog("S2a"); 
	}


	if(!orig_CG_vmMain){
		mLog("vmmain no pointer orig dll"); 
		//mb("vmmain no pointer orig dll");
	}else {
		
		//mb("vmmain success");
	}

/*	switch (command)
	{
	  case CG_INIT:
		CG_Init (arg0, arg1, arg2);
		break;
	  case CG_SHUTDOWN:
		CG_Shutdown();
		break;
	  case CG_DRAW_ACTIVE_FRAME:
		CG_DrawActiveFrame(arg0, arg1, arg2); 
		break;
	}
*/
	return CG_vmMain_Hook(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
	//return orig_CG_vmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
	//return orig_CG_vmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); //orig_CG_vmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
}

typedef void (*EntryPointfuncPtr)( int (QDECL  *syscallptr)( int arg,... ) );  
VOID (WINAPIV *orig_dllEntry) ( INT (INT, ...));

void dllEntry( int (QDECL  *syscallptr)( int arg,... ) ) {
	g_syscall = syscallptr;
	mLog("S5"); 
	if (!gl_hOriginalDll){
		LoadOriginalDll(); // looking for the "right d3d8.dll"
		mLog("S6"); 	
	}
	mLog("S7"); 
	EntryPointfuncPtr LibMainEntryPoint;   
	//fn_vmMain = (VMMAIN_FUNC)GetProcAddress( gl_hOriginalDll, "vmMain");
	LibMainEntryPoint = (EntryPointfuncPtr)GetProcAddress( gl_hOriginalDll, "dllEntry");;
	mLog("S8"); 
	//orig_CG_ServerCommand = (void (__cdecl *)(void))0x30033750;
//	orig_CG_ServerCommand = (void (__cdecl *)(void))DetourFunction( (LPBYTE)0x30033750 , (LPBYTE) CG_ServerCommand_Hook );
	mLog("S9"); 
	orig_CG_vmMain = (INT (WINAPIV*)(INT,INT,INT,INT,INT,INT,INT,INT)) (VMMAIN_FUNC)GetProcAddress( gl_hOriginalDll, "vmMain");
	mLog("S10"); 
	if(!LibMainEntryPoint){
		mLog("no pointer orig dll"); 
		//mb("no pointer orig dll");
		::ExitProcess(0); // exit the hard way
	}else {
		mLog("S12"); 
		//mb("success");
	}
	return (LibMainEntryPoint(syscallptr));
}

void InitInstance(HANDLE hModule) 
{
	OutputDebugString("PROXYDLL: InitInstance called.\r\n");

	// Initialisation
	gl_hOriginalDll        = NULL;
	gl_hThisInstance       = NULL;
	
	// Storing Instance handle into global var
	gl_hThisInstance = (HINSTANCE)  hModule;
}

void LoadOriginalDll(void)
{
    char buffer[MAX_PATH];
	strcpy_s(buffer, MAX_PATH-1, "mt_qagame_mp_x86.dll");

	// try to load the system's d3d8.dll, if pointer empty
	if (!gl_hOriginalDll) 
		gl_hOriginalDll = ::LoadLibrary(buffer);

	// Debug
	if (!gl_hOriginalDll)
	{
		mLog("Error loading dll!");
		//mb("Error loading dll!");
		OutputDebugString("PROXYDLL: Original d3d8.dll not loaded ERROR ****\r\n");
		::ExitProcess(0); // exit the hard way
	} 
}

void ExitInstance() 
{    
    OutputDebugString("PROXYDLL: ExitInstance called.\r\n");
	
	// Release the system's d3d8.dll
	if (gl_hOriginalDll)
	{
		::FreeLibrary(gl_hOriginalDll);
	    gl_hOriginalDll = NULL;  
	}
}

