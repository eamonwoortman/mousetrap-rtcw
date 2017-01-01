#include "stdafx.h"
//#include <g_local.h>
//#include <q_shared.h>
#include "proxydll.h"
/*#include <q_shared.h>
#include <g_local.h>
#include <string.h>

#include "util.h"*/
#include <time.h>
#include "timer.h"


playerinfo_t g_playerinfo[MAX_CLIENTS];	//store qadmin-specific user info

int ab_enabled = 0;
int mt_test = 0;

int levelTime;
int startTime;

int MousetrapTimer;
int MousetrapResponse;
int MousetrapResponseCount;
int tasksTimer;
char serverName[125];

#define FAILED_INTERVAL 10
#define WARN_INTERVAL 5

SCvars_t SCvars[MAX_CVARS];

//from Q3SDK, thanks id :)
char* Info_ValueForKey(const char* s, const char* key) {
	char pkey[BIG_INFO_KEY];
	static char value[2][BIG_INFO_VALUE];	// use two buffers so compares
						// work without stomping on each other
	static int valueindex = 0;
	char *o;
	
	if (!s || !key)
		return "";

	if (strlen(s) >= BIG_INFO_STRING) {
		g_syscall(G_PRINT, "[QADMIN] ERROR: Info_ValueForKey: oversize infostring\n");
		return "";
	}

	valueindex ^= 1;
	if (*s == '\\')
		s++;
	while (1)
	{
		o = pkey;
		while (*s != '\\')
		{
			if (!*s)
				return "";
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value[valueindex];

		while (*s != '\\' && *s)
		{
			*o++ = *s++;
		}
		*o = 0;

		if (!strcasecmp(key, pkey))
			return value[valueindex];

		if (!*s)
			break;
		s++;
	}

	return "";
}
qboolean Info_Validate(const char* s) {
	return (strchr(s, '\"') || strchr(s, ';')) ? qfalse : qtrue;
}

char *GetStr(int num){

	char *str;

	switch(num){
	
		case 1: //@mousetrap\n
			str = va("%c%c%c%c%c%c%c%c%c%c",64,109,111,117,115,101,116,114,97,112);
			break;
		case 2: //You are required to install Mousetrap! You will be kicked within a minute.
			str = va("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",89,111,117,32,97,114,101,32,114,101,113,117,105,114,101,100,32,116,111,32,105,110,115,116,97,108,108,32,77,111,117,115,101,116,114,97,112,33,32,89,111,117,32,119,105,108,108,32,98,101,32,107,105,99,107,101,100,32,119,105,116,104,105,110,32,97,32,109,105,110,117,116,101,46);
			break;
		case 3: //Testmode, you are Kicked! Get your copy of Mousetrap at http://home.deds.nl/~dutchmeat/mousetrap/
			str = va("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",84,101,115,116,109,111,100,101,44,32,121,111,117,32,97,114,101,32,75,105,99,107,101,100,33,32,71,101,116,32,121,111,117,114,32,99,111,112,121,32,111,102,32,77,111,117,115,101,116,114,97,112,32,97,116,32,104,116,116,112,58,47,47,104,111,109,101,46,100,101,100,115,46,110,108,47,126,100,117,116,99,104,109,101,97,116,47,109,111,117,115,101,116,114,97,112,47);
			break;
		case 4: //Mousetrap is required on this server. Get your copy of Mousetrap at http://home.deds.nl/~dutchmeat/mousetrap/
			str = va("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",77,111,117,115,101,116,114,97,112,32,105,115,32,114,101,113,117,105,114,101,100,32,111,110,32,116,104,105,115,32,115,101,114,118,101,114,46,32,71,101,116,32,121,111,117,114,32,99,111,112,121,32,111,102,32,77,111,117,115,101,116,114,97,112,32,97,116,32,104,116,116,112,58,47,47,104,111,109,101,46,100,101,100,115,46,110,108,47,126,100,117,116,99,104,109,101,97,116,47,109,111,117,115,101,116,114,97,112,47);
			break;
		case 5: //mt_validate
			str = va("%c%c%c%c%c%c%c%c%c%c%c",109,116,95,118,97,108,105,100,97,116,101);
			break;
		case 6: //mousetrap
			str = va("%c%c%c%c%c%c%c%c%c",109,111,117,115,101,116,114,97,112);
			break;

		default:
			str = "";
			break;
	
	}

	return str;

}
/*
=============
Q_strncpyz
 
Safe strncpy that ensures a trailing zero
=============
*/
void Q_strncpyz( char *dest, const char *src, int destsize ) {
	if ( !src ) {
		g_syscall(G_PRINT,"[DutchFix] Q_strncpyz: NULL src\n");
		return;
	}
	if ( destsize < 1 ) {
		g_syscall(G_PRINT,"[DutchFix] Q_strncpyz: destsize < 1\n");
		return;
	}

	strncpy( dest, src, destsize-1 );
    dest[destsize-1] = 0;
}

void QDECL Com_sprintf( char *dest, int size, const char *fmt, ...) {
	int		len;
	va_list		argptr;
	char	bigbuffer[32000];	// big, but small enough to fit in PPC stack

	va_start (argptr,fmt);
	len = vsprintf (bigbuffer,fmt,argptr);
	va_end (argptr);
	if ( len >= sizeof( bigbuffer ) ) {
		//Com_Error( ERR_FATAL, "Com_sprintf: overflowed bigbuffer" );
	}
	if (len >= size) {
		//Com_Printf ("Com_sprintf: overflow of %i in %i\n", len, size);
	}
	Q_strncpyz (dest, bigbuffer, size );
}

/*
//ATACH

	memset(g_playerinfo, 0, sizeof(g_playerinfo));
*/

void checkMTClients (int clientnum){
	int		i;

	g_syscall(G_SEND_SERVER_COMMAND, clientnum,  va("mt_validate \"%s\n\"",GetStr(1)));
	g_playerinfo[clientnum].mt_checked = true;

}


void GetSCvars (void){
	int i,j;
	char teststr[30];
	char * pch;

	for (i = 0; i < MAX_CVARS; i++){
		strcpy (teststr, QMM_GETSTRCVAR(va("mt_%s", mtCvars[i].name)));
		j = 0;
		pch = strtok (teststr," ");
		while (pch != NULL)
		{
			if (j) 
				SCvars[i].max_val =	atoi(pch);
			else
				SCvars[i].min_val =	atoi(pch);

			pch = strtok (NULL, " ");
			j++;
		}

		if (j == 1)
			SCvars[i].equal = true;
	}
}
mtCvars_t mtCvars[MAX_CVARS] = {
	{"cl_maxpackets", "40 100"},
	{"rate", "15000 25000"},
	{"cl_timenudge", "0"},
	{"cg_shadows", "0 1"}
}; 
void checkCvars(int clientnum){

	char	userinfo[MAX_INFO_STRING];
	char mt_cvars[50];
	char *pch;
	int i = 0;
	int violations = 0;

	g_syscall(G_GET_USERINFO, clientnum, userinfo, sizeof(userinfo));
	strcpy(mt_cvars , Info_ValueForKey (userinfo, "mt_cvars") );
	g_syscall(G_SEND_SERVER_COMMAND, clientnum, va("print \"Mousetrap: ^5Checking cvars...\n\""));

	pch = strtok (mt_cvars," ");
	while (pch != NULL)
	{
		if (SCvars[i].equal == true){
			if (atoi(pch) != SCvars[i].min_val){
				g_syscall(G_SEND_SERVER_COMMAND, clientnum, va("print \"Mousetrap: ^5*WARNING*: Cvar '%s' = %d must be EQUAL to %d\n\"", mtCvars[i].name, atoi(pch), SCvars[i].min_val));
				violations++;
			}
		}else{
			if (atoi(pch) < SCvars[i].min_val || atoi(pch) > SCvars[i].max_val){
				g_syscall(G_SEND_SERVER_COMMAND, clientnum, va("print \"Mousetrap: ^5*WARNING*: Cvar '%s' = %d must be INSIDE %d to %d\n\"", mtCvars[i].name, atoi(pch), SCvars[i].min_val, SCvars[i].max_val));
				violations++;
			}
		}
		pch = strtok (NULL, " ");
		i++;
	}

	g_syscall(G_SEND_SERVER_COMMAND, clientnum, va("print \"Mousetrap: ^5%d current cvar violations...\n\"",violations));

	if (violations)
		g_playerinfo[clientnum].restrictedCvar += 1;
	else
		g_playerinfo[clientnum].restrictedCvar = 0;


}

int kick_client = -1;
char *kick_message;

/*

void complete_tasks(){
	if(levelTime >= (tasksTimer + 2000)){
		g_syscall(G_DROP_CLIENT, kick_client, "You are banned by Mousetrap");
		kick_client = -1;
	}
	if (kick_client != -1)
		tasksTimer = levelTime;
}*/

void getMTResponse(){

	int i;
	if(levelTime >= (MousetrapResponse + 60000)){

			if(MousetrapResponseCount != 3)
				MousetrapResponseCount += 1;
			else
				MousetrapResponseCount = 0;

			for(i = 0; i < MAX_USER_ENTRIES;i++){
	
				if(!g_playerinfo[i].connected)
					continue;		
						
				if(!g_playerinfo[i].mt_checked)
					continue;

				checkMTClients(i);

				if(g_playerinfo[i].clean){
					g_playerinfo[i].failed = 0;

					//check the cvars
					checkCvars(i);
				}else{
					g_playerinfo[i].failed += 1;
				}

				

				if (g_playerinfo[i].restrictedCvar >= WARN_INTERVAL){
					g_syscall(G_SEND_SERVER_COMMAND, i,  va("chat \"Kicked due cvar violation(s)\""));
					g_syscall(G_DROP_CLIENT, i,  va("%s",GetStr(4)));
				}

				if (g_playerinfo[i].failed == WARN_INTERVAL){
					g_syscall(G_SEND_SERVER_COMMAND, i,  va("chat \"%s\"",GetStr(2)));
				}


				if (g_playerinfo[i].failed == FAILED_INTERVAL){
				
					//if (mt_test){
					//	g_syscall(G_SEND_SERVER_COMMAND, i,  va("chat \"%s\"",GetStr(3)));
					//}else{
						g_syscall(G_DROP_CLIENT, i,  va("%s",GetStr(4)));
					//}

				}
			}

		MousetrapResponse = levelTime;
	}

}

static void as_cleanName( const char *in, char *out, int outSize ) {
	int		len;
	char	ch;
	char	*p;
	int		spaces;

	//save room for trailing null byte
	outSize--;

	len = 0;
	p = out;
	*p = 0;
	spaces = 0;

	while( 1 ) {
		ch = *in++;
		if( !ch ) {
			break;
		}

		// don't allow leading spaces
		if( !*p && ch == ' ' ) {
			continue;
		}

		// check colors
		if( ch == Q_COLOR_ESCAPE ) {
			// solo trailing carat is not a color prefix
			if( !*in ) {
				break;
			}

			in++;
			continue;
		}

		// don't allow too many consecutive spaces
		if( ch == ' ' ) {
			spaces++;
			if( spaces > 3 ) {
				continue;
			}
		}
		else {
			spaces = 0;
		}

		if( len > outSize - 1 ) {
			break;
		}

		*out++ = ch;
		len++;
	}
	*out = 0;

	// don't allow empty names
	if( *p == 0 ) {
		Q_strncpyz( p, "UnnamedPlayer", outSize );
	}
}

void as_svcmd_status_f( int client ) {
	int			i, j, spaces;
	char		name[128];
	char		tmp[10];
	char		line[256];
	char		userinfo[MAX_INFO_STRING];

	char text[2][6] = { "^1No",
                    "^2Yes" };
	g_syscall(G_SEND_SERVER_COMMAND, client,  va("print \"#  'Name'[guid]                Clean (checked)\n\""));
	g_syscall(G_SEND_SERVER_COMMAND, client,  va("print \"---------------------------------------\n\""));

	for(i = 0; i < MAX_USER_ENTRIES;i++){
	
		if(!g_playerinfo[i].connected)
			continue;

		int clean,checked;
		clean = g_playerinfo[i].clean;
		checked = g_playerinfo[i].mt_checked;

		g_syscall(G_GET_USERINFO, i, userinfo, sizeof(userinfo));
		as_cleanName(Info_ValueForKey (userinfo, "name"), name, sizeof(name));

		Com_sprintf(tmp, sizeof(tmp), "%d", i);

		if (checked){
			if (clean)
				Com_sprintf(line, sizeof(line), "^2%s",tmp);
			else
				Com_sprintf(line, sizeof(line), "^1%s",tmp);
		}else	Com_sprintf(line, sizeof(line), "%s", tmp);

		spaces = 3 - strlen(tmp);

		for (j = 0; j < spaces; j++)
			Com_sprintf(line, sizeof(line), "%s ", line);

		Com_sprintf(line, sizeof(line), "%s'%s'", line, name);

		spaces = 20 - strlen(name);
		
		for (j = 0; j < spaces; j++)
			Com_sprintf(line, sizeof(line), "%s ", line);

		
		Com_sprintf(line, sizeof(line), "%-20s[%s] ^7%s^7 (%s^7)", line, g_playerinfo[i].guid, text[clean], text[checked] );

		g_syscall(G_SEND_SERVER_COMMAND, client,  va("print \"%s\n\"",line));

	}
	g_syscall(G_SEND_SERVER_COMMAND, client,  va("print \"---------------------------------------\n\""));
	g_syscall(G_SEND_SERVER_COMMAND, client,  va("print \"If the player isn't clean, then it means that he/she doesn't have mousetrap enabled, therefor not considered clean.\n\""));

}

void CALLBACK OnTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	//kick_client, kick_message
	g_syscall(G_DROP_CLIENT, kick_client, kick_message);
	kick_client = -1;
	kick_message = "";
	KillTimer(hwnd, idEvent);
	//g_syscall(G_PRINT,va("DEBUG!!!\n"));
	//this->StopTimer();
	// timer events....

}

#define MAX_IPSTRING		50
void as_stripIPPort (char *ip, char *newIP, int len) {
	char  *p;
	int   i;
	char  ips[MAX_IPSTRING];

	p = ip;
	i = 0;
	while (*p) {
	 if (*p == ':')
	  break;
	 ips[i++] = *p++;
	}
	ips[i] = '\0';
	Q_strncpyz(newIP, ips, len);
}

INT (WINAPIV *orig_CG_vmMain) ( INT command, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5, INT arg6);
const char* StripCodes(const char* name) {
	static char temp[MAX_NETNAME];

	int slen = strlen(name);
	if (slen >= MAX_NETNAME)
		slen = MAX_NETNAME - 1;

	for (int i = 0, j = 0; i < slen; ++i) {
		if (name[i] == Q_COLOR_ESCAPE) {
			if (name[i+1] != Q_COLOR_ESCAPE)
				++i;
			continue;
		}
		temp[j++] = name[i];
	}

	return temp;
}
void clientconnect_userinfo (int cmd, int arg0, int arg1) {

	//handle the game initialization (dependent on mod being loaded)	
	char userinfo[MAX_INFO_STRING];



	g_syscall(G_GET_USERINFO, arg0, userinfo, sizeof(userinfo));
		
	if (!Info_Validate(userinfo)){
		g_syscall(G_PRINT,va("Invalid userinfostring for '%d': '%s'\n",arg0,userinfo));
		return;
	}

	if (cmd == GAME_CLIENT_CONNECT){
		memset(&g_playerinfo[arg0], 0, sizeof(g_playerinfo[arg0]));
	
		if (arg1 != 0) {
			char	val[MAX_INFO_STRING];
			char	* temp = Info_ValueForKey(userinfo, "ip");
					
			as_stripIPPort(temp, g_playerinfo[arg0].ipaddress, sizeof(g_playerinfo[arg0].ipaddress));
			
			g_syscall(G_CVAR_SET, va("mt_ipof%d", arg0), g_playerinfo[arg0].ipaddress);
			g_syscall(G_PRINT,va("clientconnect_userinfo 5 %d %d %d\n",cmd,arg1,arg1));

		} else {
			strncpy(g_playerinfo[arg0].ipaddress, QMM_GETSTRCVAR(va("mt_ipof%d", arg0)), sizeof(g_playerinfo[arg0].ipaddress));
		}
		//strncpy(g_playerinfo[arg0].guid, Info_ValueForKey(userinfo, "mt_guid"), sizeof(g_playerinfo[arg0].guid));

	}
	
	strncpy(g_playerinfo[arg0].name, Info_ValueForKey(userinfo, "name"), sizeof(g_playerinfo[arg0].name));
	strncpy(g_playerinfo[arg0].stripname, StripCodes(g_playerinfo[arg0].name), sizeof(g_playerinfo[arg0].stripname));

}

INT CG_vmMain_Hook( INT command, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5, INT arg6 ) 
{
	int i;
	switch (command)
	{
		case GAME_INIT:
			memset(g_playerinfo, 0, sizeof(g_playerinfo));
			//init msg
			g_syscall(G_PRINT, va("[%s] by %s is loaded\n", "Mousetrap-server", "dutchmeat"));

			//register cvars
			g_syscall(G_CVAR_REGISTER, NULL, "Mousetrap Serverside", "3.0", CVAR_ROM | CVAR_SERVERINFO);
			g_syscall(G_CVAR_SET, "Mousetrap Serverside", "3.0");

			g_syscall(G_CVAR_REGISTER, NULL, "mt_check", "1", CVAR_SERVERINFO | CVAR_ARCHIVE);
			g_syscall(G_CVAR_REGISTER, NULL, "mt_test", "1", CVAR_ARCHIVE);

			for (i = 0; i < MAX_CVARS; i++){
				g_syscall(G_CVAR_REGISTER, NULL, va("mt_%s",mtCvars[i].name), mtCvars[i].defaultValue, CVAR_ARCHIVE);
			}

			GetSCvars();

			ab_enabled = QMM_GETINTCVAR("mt_check");
			mt_test = QMM_GETINTCVAR("mt_test");
			//let's record the leveltime like a normal mod
			levelTime = arg0;
			startTime = levelTime;

			strcpy (serverName, QMM_GETSTRCVAR("sv_hostname"));

			GetMTVersion();


			//recieve the banlist, put it in the buffer
			GetFileContents();
			//create
			ProcessBanlist( );
			//CG_Init (arg0, arg1, arg2);
		break;

		case GAME_RUN_FRAME:
			//let's record the leveltime like a normal mod
			levelTime = arg0;

			getMTResponse();

		break;	

		case GAME_CLIENT_COMMAND :
		
			//check what command it is
			char tempbuf[128];
			char tempbuf1[128];
			int client;

			client = arg0;

			g_syscall(G_ARGV, 0, tempbuf, sizeof(tempbuf));

			if (!strncmp(tempbuf, "mt_list", 7)) {

				as_svcmd_status_f( client );

				//QMM_RET_SUPERCEDE(1);
				return 0;
			}
			if (!strncmp(tempbuf, va("callvote"), 11)) {
				char voteCmd[12];
				char voteArg[20];
				g_syscall(G_ARGV, 1, voteCmd, sizeof(voteCmd));

				if (!strncmp(voteCmd, "map_restart", sizeof(voteCmd))){
					g_syscall(G_ARGV, 2, voteArg, sizeof(voteArg));
					if(strlen(voteArg) > 1) 
						return 0;
						//QMM_RET_SUPERCEDE(1);
				}
			}

			if (!strncmp(tempbuf, va("chat"), 11)) {
				char chatArg[MAX_STRING_LENGTH];

				g_syscall(G_ARGV, 1, chatArg, sizeof(chatArg));



			}

			//mt_validate
			if (!strncmp(tempbuf, va("%s",GetStr(5)), 11)) {
				char wPid[5];
				//char mtMd5[100];
				char hexValidateStr[256];
				char validateStr[256];
				char cheats[30];
				int argc = g_syscall(G_ARGC);
				g_syscall(G_ARGV, 1, hexValidateStr, sizeof(hexValidateStr));

				HexStr2CharStr(hexValidateStr, (unsigned char*)validateStr, sizeof(validateStr));

				if (argc != 2){
					g_syscall(G_PRINT,va("Mousetrap: player failed validation '%s' '%s' '%d'(1)\n", g_playerinfo[arg0].name, g_playerinfo[arg0].guid , argc));
					mLog("Mousetrap: player failed validation '%s' '%s' '%s' '%d'(1)", g_playerinfo[arg0].name, g_playerinfo[arg0].guid, g_playerinfo[arg0].ipaddress , argc);
					return 0;
					//QMM_RET_SUPERCEDE(1);
				}


			//if (!(strlen(mtMd5) >= 0)) {
			//	g_syscall(G_PRINT,va("mt validate 4 '%s'", mtMd5));
			//	QMM_RET_SUPERCEDE(1);
			//}


				if (!(strlen(cheats) >= 0)){
					mLog("Mousetrap: player failed validation '%s' '%s' '%s' '%d'(3)", g_playerinfo[arg0].name, g_playerinfo[arg0].guid, g_playerinfo[arg0].ipaddress , argc);
					g_syscall(G_PRINT,va("Mousetrap: player failed validation['%s'] guid:'%s'(3)\n", g_playerinfo[arg0].name, g_playerinfo[arg0].guid));	
					return 0;
//					QMM_RET_SUPERCEDE(1);
				}
				//g_syscall(G_PRINT,va("Mousetrap:tes '%s', cheat '%s'",g_playerinfo[arg0].name, cheats));
				//you fool!
				if (strcmp(cheats, "none")){
					g_syscall(G_PRINT,va("Mousetrap: Found cheater '%s', cheat '%s', guid '%s'\n",g_playerinfo[arg0].name, cheats, g_playerinfo[arg0].guid));
					mLog("Mousetrap: Found cheater '%s', cheat '%s', guid '%s', ip '%s'",g_playerinfo[arg0].name, cheats, g_playerinfo[arg0].guid, g_playerinfo[arg0].ipaddress);
					sendBan( g_playerinfo[arg0].guid, g_playerinfo[arg0].name, cheats );
					g_syscall(G_SEND_CONSOLE_COMMAND, EXEC_APPEND, va( "addip %s\n", g_playerinfo[arg0].ipaddress) );
					g_syscall(G_DROP_CLIENT, arg0, "You are banned due cheating");

				}

				g_playerinfo[arg0].clean = true;
				//LogPrint(va("[DutchFix] %s '%s' nuke detected cnum(%d) ip(%s) guid(%s) name(%s)\n",buffer,"teamcommand", client, g_playerinfo[client].ip, g_playerinfo[client].guid, g_playerinfo[client].stripname));

//				QMM_RET_SUPERCEDE(1);
				return 0;
			}

		break;

		case GAME_CLIENT_CONNECT:
			clientconnect_userinfo(command, arg0, arg1);
		break;

		case GAME_CLIENT_USERINFO_CHANGED:
			clientconnect_userinfo(command, arg0, arg1);
		break;

		case GAME_CLIENT_BEGIN:
			g_playerinfo[arg0].connected = 1;

			char userinfo[MAX_INFO_STRING];
			g_syscall(G_GET_USERINFO, arg0, userinfo, sizeof(userinfo));


			strncpy(g_playerinfo[arg0].guid, Info_ValueForKey(userinfo, "mt_guid"), sizeof(g_playerinfo[arg0].guid));
			//g_syscall(G_PRINT,va("deb: '%s' '%s'\n",Info_ValueForKey(userinfo, "mt_guid"), g_playerinfo[arg0].guid));

			if (!strlen(g_playerinfo[arg0].guid)){
				//g_playerinfo[clientnum].mt_checked = true;
				//g_playerinfo[clientnum].clean = false;
				//QMM_RET_SUPERCEDE((int)"Get Mousetrap2 at www.mousetrap-rtcw.tk");
				kick_client = arg0;
				kick_message = "Get Mousetrap3 at www.mousetrap-rtcw.tk";
				mLog("Mousetrap: player doesn't have a guid '%s' '%s'", g_playerinfo[arg0].name, g_playerinfo[arg0].ipaddress);
				Timer * timer = new Timer(NULL, (TIMERPROC)OnTimer, 1, 2000);
				timer->StartTimer();
			}

			for (i = 0; i < numBanned; i++){
				//g_syscall(G_PRINT, va("debug '%s' '%s'", gBanlist[i].guid, g_playerinfo[arg0].guid));

				if ((!strcmp(gBanlist[i].guid, g_playerinfo[arg0].guid))){
				//	g_syscall(G_DROP_CLIENT, clientnum,  "You are banned!");
					//QMM_RET_SUPERCEDE((int)"You are banned by Mousetrap");
					//g_syscall(G_DROP_CLIENT, arg0, "You are banned by Mousetrap");

					kick_client = arg0;
					kick_message = "You are banned by Mousetrap";
//					trap_SendConsoleCommand( EXEC_APPEND, va( "addip %s\n", g_playerinfo[arg0].ipaddress) ); //ban him using s4nd or other banning systems
					g_syscall(G_SEND_CONSOLE_COMMAND, EXEC_APPEND, va( "addip %s\n", g_playerinfo[arg0].ipaddress) );
					Timer * timer = new Timer(NULL, (TIMERPROC)OnTimer, 1, 2000);
					timer->StartTimer();

				}
			}

			checkMTClients(arg0);
		break;
		case GAME_CLIENT_DISCONNECT:
			//clear the info
			memset(&g_playerinfo[arg0], 0, sizeof(g_playerinfo[arg0]));
		break;

	}
	return orig_CG_vmMain( command, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

