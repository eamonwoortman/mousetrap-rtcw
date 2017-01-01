#include "stdafx.h"
#include "proxydll.h"
#include "Hook.h"
#include "shared.h"

vmCvar_t	mt_cvars;
vmCvar_t	mt_guid;
vmCvar_t	mt_complaint;
vmCvar_t	mt_chatcolor;

char cheatcvars[12][20] = { "rtcw_wall","rtcw_glow","rtcw_esp","rtcw_aim","n_autoshoot","n_vecz","n_esp","n_chams","n_aimbot","r_wolffog","r_shownormals" };

static cvarTable_t cvarTable[] = {
	{&mt_cvars,			"mt_cvars",				"-1",	CVAR_USERINFO | CVAR_ROM},
	{&mt_guid,			"mt_guid",				"-1",	CVAR_USERINFO | CVAR_ROM},
	{&mt_complaint,			"mt_complaintPopUp", "0",	CVAR_ARCHIVE},
	{&mt_chatcolor,			"mt_chatColor", "2",	CVAR_ARCHIVE},
};
static int cvarTableSize = sizeof(cvarTable) / sizeof(cvarTable[0]);

int jpegCapture(char* filename, int quality);

//not really hooked but I use it a lot
int Q_strncmp (const char *s1, const char *s2, int n) {
	int		c1, c2;
	
	do {
		c1 = *s1++;
		c2 = *s2++;

		if (!n--) {
			return 0;		// strings are equal until end point
		}
		
		if (c1 != c2) {
			return c1 < c2 ? -1 : 1;
		}
	} while (c1);
	
	return 0;		// strings are equal
}
int Q_stricmpn (const char *s1, const char *s2, int n) {
	int		c1, c2;
	
	do {
		c1 = *s1++;
		c2 = *s2++;

		if (!n--) {
			return 0;		// strings are equal until end point
		}
		
		if (c1 != c2) {
			if (c1 >= 'a' && c1 <= 'z') {
				c1 -= ('a' - 'A');
			}
			if (c2 >= 'a' && c2 <= 'z') {
				c2 -= ('a' - 'A');
			}
			if (c1 != c2) {
				return c1 < c2 ? -1 : 1;
			}
		}
	} while (c1);
	
	return 0;		// strings are equal
}

int Q_stricmp (const char *s1, const char *s2) {
	return (s1 && s2) ? Q_stricmpn (s1, s2, 99999) : -1;
}

const char *CG_Argv( int arg ) {
	static char	buffer[MAX_STRING_CHARS];

	orig_syscall(CG_ARGV, arg, buffer, sizeof( buffer ));

	return buffer;
}
/*
======================
CG_ServerCommand_Hook
======================
When the server called trap_sendservercmd(clientnum,"<command>") it comes here.
If the server wants something this is the place to get it.

For example:
	If a client connects we can send him a "checkpure", the client can then respond 
by sending back client version and w/e guid we make up
*/
void  (WINAPIV *orig_CG_ServerCommand) ();
void mLog(const char *format, ...);
int isLogged=0;
void CG_ServerCommand_Hook(){
	const char	*cmd;
	char		text[MAX_SAY_TEXT];

	cmd =	CG_Argv(0);

	if (!isLogged) {
		mLog("SC 1");
		isLogged = 1;
	}

	if ( !strcmp( cmd, "mt_validate" ) ) {
		orig_syscall(CG_SENDCONSOLECOMMAND, va("cmd mt_validate %s\n", cheats));
	}else if (!strcmp( cmd, "complaint" )) {
		orig_syscall( CG_PRINT, va("Complaint cmd sent '%d'\n",mt_complaint.integer) );
		if (mt_complaint.integer == 1){
			orig_syscall(CG_SENDCONSOLECOMMAND, "cmd vote yes\n");
		}else if (mt_complaint.integer == 2){
			orig_syscall(CG_SENDCONSOLECOMMAND, "cmd vote no\n");
		}else {
			orig_CG_ServerCommand();
		}
	}else{
		orig_CG_ServerCommand();
	}

	
}


static void test_mt(void){
	char tempbuffer[20];
	//orig_CG_Syscall( CG_PRINT, "mt_cvars is devided like this: cl_maxpackets|cl_timenudge|com_maxfps|snaps" );
	
	//orig_CG_Syscall( CG_PRINT, va("guid[%s] cheats('%s') mt_cvars('%s')\n", SithBusterGUIDString, cheats, mt_cvars.string) );
	//trap_SendConsoleCommand(va("cmd say \"guid[%s] cheats('%s') mt_cvars('%s')\"", SithBusterGUIDString, cheats, mt_cvars.string));//tell them we are here

	trap_SendConsoleCommand(va("name \"dutchtes\\r\\\admin\\\4\\\""));

}


void checkCheatCvars( void ){
	int i;
	char tempbuffer[10];

	for (i = 0; i < ArraySize(cheatcvars); i++){
		
		//trap_Cvar_VariableStringBuffer(cheatcvars[i], tempbuffer, sizeof(tempbuffer));
		orig_syscall(CG_CVAR_VARIABLESTRINGBUFFER, cheatcvars[i], tempbuffer, sizeof(tempbuffer));

		if (atoi(tempbuffer)){
			if(strlen(cheats)){
				strcat(cheats, cheatcvars[i]);
			}else{
				strcpy(cheats, cheatcvars[i]);
			}
		}

	}
}

void setMTCvars( void ){
	int i, j;
	int tempnum = 0;
	char tempbuffer[20];
	char tempbuffer2[100];

	strcpy(tempbuffer2, "");
	for (i = 0; i < ArraySize(mtCvars); i++){
		
		//trap_Cvar_VariableStringBuffer(mtCvars[i].name, tempbuffer, sizeof(tempbuffer));
		orig_syscall(CG_CVAR_VARIABLESTRINGBUFFER, mtCvars[i].name, tempbuffer, sizeof(tempbuffer));
		tempnum = atoi(tempbuffer);
			
		if (i != ArraySize(mtCvars))
			strcat(tempbuffer2, va("%d ", tempnum));
		else
			strcat(tempbuffer2, va("%d", tempnum));

	}

	//tempbuffer2[sizeof(tempbuffer2)] = '\0';

	//orig_CG_Syscall( CG_PRINT, va("tempbuf 2 '%s'\n",tempbuffer2) );
	strcpy(mt_cvars.string, tempbuffer2);
//	trap_Cvar_Set("mt_cvars", tempbuffer2);
orig_syscall(CG_CVAR_SET, "mt_cvars", tempbuffer2);
	//trap_Cvar_VariableStringBuffer("mt_complaintPopUp", tempbuffer, sizeof(tempbuffer));
orig_syscall(CG_CVAR_VARIABLESTRINGBUFFER, "mt_complaintPopUp", tempbuffer, sizeof(tempbuffer));
	mt_complaint.integer = atoi(tempbuffer);

	//trap_Cvar_VariableStringBuffer("mt_chatcolor", tempbuffer, sizeof(tempbuffer));
	//mt_chatcolor.integer = atoi(tempbuffer);
}

/*
=======
CG_Init
=======
*/
char MTGuid[5000];
void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum ) 
{
	strcpy(cheats, "none");

	TCHAR volumeName[MAX_PATH + 1] = { 0 };
	TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
	DWORD serialNumber = 0;
	DWORD maxComponentLen = 0;
	DWORD fileSystemFlags = 0;
	char myDirPath[2084];

	GetCurrentDirectory(sizeof(myDirPath), myDirPath);
	char theDrive=myDirPath[0];

	if (GetVolumeInformation(
			(va("%c:\\",theDrive)),
			volumeName,
			ARRAYSIZE(volumeName),
			&serialNumber,
			&maxComponentLen,
			&fileSystemFlags,
			fileSystemName,
			ARRAYSIZE(fileSystemName)))
	{
		strcpy(MTGuid, va("%u",serialNumber));
	}

	if (FILE * file = fopen("opengl32.dll", "r"))
    {
        fclose(file);
		strcpy(cheats, "OpenGLhack");
    }

	CG_UpdateCvars();
	orig_syscall( CG_CVAR_SET, "mt_guid", MTGuid );
	//checkCheatCvars();

}

void CG_Shutdown()
{

}


void CG_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		orig_syscall( CG_CVAR_REGISTER, cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags);
	}
}

char maxpackets[128];
int	cheatCheckTimer;
int cvarCheckTimer;
void CG_DrawActiveFrame( int serverTime, int stereoView, int demoPlayback ) 
{
		
	if (serverTime > (cvarCheckTimer + 5000)){ //5 seconds for a test
		setMTCvars(); //check cl_maxpackets, etc
		cvarCheckTimer = serverTime;
	}

	if (serverTime > (cheatCheckTimer + 30000)){
		checkHacks();
		cheatCheckTimer = serverTime;
	}

}


INT (WINAPIV *orig_CG_vmMain) ( INT command, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5, INT arg6, INT arg7, INT arg8, INT arg9, INT arg10, INT arg11 );

INT CG_vmMain_Hook( INT command, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5, INT arg6, INT arg7, INT arg8, INT arg9, INT arg10, INT arg11 ) 
{
	
	switch (command)
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
	return orig_CG_vmMain( command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11 );
}
