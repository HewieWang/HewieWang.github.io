#include <windows.h>
#include <stdio.h>
 
int main()
{
	DWORD	dwWaitStatus; 
	HANDLE	dwChangeHandles[2]; 	
	// 监视C:\Windows目录下的文件创建和删除 
	dwChangeHandles[0] = FindFirstChangeNotification(
		"C:\\WINDOWS", 						// 监视路径
		FALSE,								// 不监视子目录
		FILE_NOTIFY_CHANGE_FILE_NAME);		// 监视文件名变动
	if(dwChangeHandles[0] == INVALID_HANDLE_VALUE)
		return -1;
	
	// 监视C:\下子目录树的目录创建和删除 
	dwChangeHandles[1] = FindFirstChangeNotification( 
		"C:\\", 							// 监视路径
		TRUE,								// 监视子目录
		FILE_NOTIFY_CHANGE_DIR_NAME);		// 监视目录名变动
	if(dwChangeHandles[1] == INVALID_HANDLE_VALUE)
		return -1; 
	
	// 循环监视
	while(TRUE) 
	{	
		// 接收到事件通知
		dwWaitStatus = WaitForMultipleObjects(2, dwChangeHandles,FALSE, INFINITE); 
		switch(dwWaitStatus) 
		{
		case WAIT_OBJECT_0:
			printf("Directory C:\\Windows Has Changed!\n");
			if(FindNextChangeNotification(dwChangeHandles[0]) == FALSE ) 
				return -1; 
			break; 
			
		case WAIT_OBJECT_0 + 1: 
			printf("Directory C:\\ Has Changed!\n");
			if(FindNextChangeNotification(dwChangeHandles[1]) == FALSE) 
				return -1; 
			break; 
			
		default: 
			return -1; 
		}
	}
	
	FindCloseChangeNotification(dwChangeHandles[0]);
	FindCloseChangeNotification(dwChangeHandles[1]);
	system("pause");
	return 0;
}