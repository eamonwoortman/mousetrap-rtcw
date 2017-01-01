#include "stdafx.h"
#include "Hook.h"
#include "proxydll.h"
#include <tchar.h>

#include <tlhelp32.h>
#include <iostream> 
#include <string>

#include "processapi.h"

#include "RSA_MD5.h"

char cheats[100];
int lastProcessCount = 0;
int lastModuleCount = 0;
CheatsOffset_t		InjectorTable[]=
{
	{"1DE8AFDCE3D6C1C2E05B83C7CE77FD30", "Sinject",27136},
	{"D7A7966CC50771BE6BD25BF494EE9673", "DCInjector",589824}, 
	{"5C71BF80B291452CFF22524688D38297", "Winject",159744}, //159744
	{"D17E73C68C23598558F5B3C23DA04755", "Winject", 139264},
	{"C0FEE0E979608714436A7871F53D051C", "SinjectETC",70656},	//todo get filesize
	{"5DD3F0F7F0335B8A146016428502F8D1", "InjectX",237568}
};

CheatsOffset_t		ModuleTable[]={
	{"A88C08D1D3B63FA107C51169B5FF837B", "etnclas.dll", 299008},
	{"6781981FF2F051533F74ADD44DCC8FB5", "glhookET.dll", 35840},
	{"CCA15BF20D92A0EE618876560979A846", "glut32.dll", 211456},
	{"3A48B2DFDFA9865126FC64DEB0650445", "N7XQZ.dll", 46500},
	{"0807C9741A8BD39D8EBAE87CFAC10534", "opengl32.dll",65536},
	{"2DDDD5EEB01327E1BB3328D092EB96A7", "RTCW.dll",90112},
	{"337E20A1C7A6C6D24BEC2A6B5421CFFE", "rtcwhack.dll",118272},
	{"D646618CF3A08D660AC413565D7CAD0E", "RtcwHook.dll",113959},
	{"1C214AD52D09873E51EB323BF3DB73F6", "RTCWMombot.dll",196608},
	{"258C279708EC3C8F3743CA195867BFD7", "Wolfenstein.dll",69632},
	{"2F64B1185EBCBFAD3EF9A86CC3257EA4", "wzcnxa.dll",32768},
	{"C7C32E895D17F0E423D198EFFA528A74", "qrtcw.dll",53248}
};


int ValidateMD5 ( char *File , char *CorrectMD5 )
{
	char tmpstr[500] = "";
	char HashofFile	[400] = "";
	char FilePath [400] = "";
	unsigned long seed = 0 ;
	int i;
	MD5_CTX mdC ;

	if(md5file( File , 0 , &mdC ) == 0)
		return 0;
	
	for ( i = 0 ; i < 16 ; i++ ) 
	{
		sprintf(tmpstr,"%02X", mdC.digest[i] );
		strcat( HashofFile, tmpstr );
	}

	if (strstr( HashofFile, CorrectMD5))
		return 1;

	return 0;
}

int checkHacks(void)
{
	CProcessApi papi;
	int a = 0;
	int b = 0;


	if (!papi.Init(true))
	{
		orig_syscall( CG_PRINT, "Failed to load either of process api libraries!!\n" );
		return 1;
	}

	DWORD ProcessID;
	DWORD pl = papi.ProcessesGetList();
	int i, j;
  
	if (pl)
	{
		int processCount = papi.ProcessesCount(pl);
	
		if (processCount != lastProcessCount){
			CProcessApi::tProcessInfo pi;
			for (i=processCount-1;i>=0;i--)
			{
				if (!papi.ProcessesWalk(pl, &pi, i))
				{
					continue;
				}

				for (a = 0; a < ArraySize(InjectorTable); a++){
					if(pi.FileSize == InjectorTable[a].fileSize){
				  
						if(ValidateMD5(pi.FileName, InjectorTable[a].md5)){
						
							strcpy(cheats, InjectorTable[a].cheatName); 
							papi.ProcessesFreeList(pl);
							return 0;
						}		
					}
				}
			}
		}
		lastProcessCount = processCount;
	}
	papi.ProcessesFreeList(pl);

	long wolfPid = GetCurrentProcessId(); 

	CProcessApi::tModuleInfo mi = {0};
	DWORD ml = papi.ModulesGetList(wolfPid);

	int moduleCount = papi.ModulesCount(ml);
	if (moduleCount != lastModuleCount){
		for (j=moduleCount-1;j>=0;j--)
		{
			if (!papi.ModulesWalk(ml, &mi, j))
			continue;

			for (b = 0; b < ArraySize(ModuleTable); b++){
				if(mi.FileSize == ModuleTable[b].fileSize){
					if(ValidateMD5(mi.FileName, ModuleTable[b].md5)){
						strcpy(cheats, ModuleTable[b].cheatName);
						papi.ModulesFreeList(ml);
						return 0;
					}
				}
			}
		} 
	}

	papi.ModulesFreeList(ml);
	lastModuleCount = moduleCount;
	return 0;
}
