#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>
 
int main(){
 
 
	HANDLE hDir;
	char notify[1024];
	DWORD cbBytes,i;
	FILE_NOTIFY_INFORMATION *pNotify=(FILE_NOTIFY_INFORMATION*)notify;
	hDir=CreateFile("c:\\windows",FILE_LIST_DIRECTORY,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
		NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,NULL);
	if(hDir==INVALID_HANDLE_VALUE)
	{
 
		return -1;
 
	}
	while(TRUE)
	{
 
		if(ReadDirectoryChangesW(hDir,notify,sizeof(notify),FALSE,FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_LAST_WRITE,&cbBytes,NULL,NULL))
		{
 
			do{
 
				switch(pNotify->Action)
				{
 
				case FILE_ACTION_ADDED:
					printf("Directory/File added!\n");;
					break;
				case FILE_ACTION_REMOVED:
					
					printf("Directory/File Removed!\n");
					break;
				case FILE_ACTION_MODIFIED:
					printf("Directory/File Modified!\n");
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					printf("Directory/File old name!\n");
					break;
				case FILE_ACTION_RENAMED_NEW_NAME:
					printf("Directory/File new name!\n");
					break;
				default:
					break;
 
				}
				if(pNotify->NextEntryOffset!=0)
				{
 
					i=pNotify->NextEntryOffset;
					pNotify=(PFILE_NOTIFY_INFORMATION)((LPBYTE)pNotify+i);
				}else{
					break;
				}
 
			}while(TRUE);
		}else{
		
			printf("ReadDirectoryChangeW ERROR!\n");
		}
 
	}
	return 0;
}