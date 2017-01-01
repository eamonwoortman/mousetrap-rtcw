// proxydll.cpp
#include "stdafx.h"
#include "proxydll.h"
#define mb(x) MessageBox(NULL, x,"MT3",MB_OK);
// global variables
HINSTANCE           gl_hOriginalDll;
HINSTANCE           gl_hThisInstance;

//static int (QDECL *orig_syscall)( int arg, ... ) = (int (QDECL *)( int, ...))-1;
int (WINAPIV *orig_syscall) ( int command, ... );
//static INT		(WINAPIV *orig_syscall)		( INT command, ... );
//static int (QDECL *orig_syscall)( int arg, ... );
FILE	*logFile;


void mLog(const char *format, ...) {
		
	char buffer[4096];
	va_list va;
	va_start(va, format);
		vsprintf_s(buffer, sizeof(buffer), format, va);
	va_end(va);

	
	fprintf_s(logFile, "%s\n", buffer);
	
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
			fopen_s(&logFile, "mousetrap.log", "w");
			mLog("S1"); 
			break;
	    case DLL_PROCESS_DETACH: 
			fclose(logFile);
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

int vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11  ) {

	if (command == CG_INIT){
		mLog("S2"); 
	}
	if (!gl_hOriginalDll) {
		LoadOriginalDll();
		mLog("S2a"); 
	}
	if (command == CG_INIT){
		mLog("S3"); 
	}

	if(!orig_CG_vmMain){
		mLog("S4"); 
		mb("vmmain no pointer orig dll");
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
	return CG_vmMain_Hook(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
	//return orig_CG_vmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
	//return orig_CG_vmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11); //orig_CG_vmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
}

typedef void (*EntryPointfuncPtr)( int (QDECL  *syscallptr)( int arg,... ) );  
VOID (WINAPIV *orig_dllEntry) ( INT (INT, ...));

void dllEntry( int (QDECL  *syscallptr)( int arg,... ) ) {
	orig_syscall = syscallptr;
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
	orig_CG_ServerCommand = (void (__cdecl *)(void))DetourFunction( (LPBYTE)0x30033750 , (LPBYTE) CG_ServerCommand_Hook );
	mLog("S9"); 
	orig_CG_vmMain = (INT (WINAPIV*)(INT,INT,INT,INT,INT,INT,INT,INT,INT,INT,INT,INT,INT)) (VMMAIN_FUNC)GetProcAddress( gl_hOriginalDll, "vmMain");
	mLog("S10"); 
	if(!LibMainEntryPoint){
		mLog("S11"); 
		mb("no pointer orig dll");
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
	strcpy_s(buffer, MAX_PATH-1, "mt_cgame_mp_x86.dll");

	// try to load the system's d3d8.dll, if pointer empty
	if (!gl_hOriginalDll) 
		gl_hOriginalDll = ::LoadLibrary(buffer);

	// Debug
	if (!gl_hOriginalDll)
	{
		mb("Error loading dll!");
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

