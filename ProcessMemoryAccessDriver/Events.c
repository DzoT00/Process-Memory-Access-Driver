//Events.c : defines the event handler called when the executable image is run

#pragma warning (disable : 4047 4311)
#include "Events.h"
#include "data.h"
//PCREATE_PROCESS_NOTIFY_ROUTINE_EX 
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	//in this case statement:
	//the names of all executable files loaded into the process address space
	//when starting a new application are compared with the search string
	if (wcsstr(FullImageName->Buffer, L"\\Target_MFC_Application.exe"))//if (wcsstr(FullImageName->Buffer, L"\\exe file path\\name of target application"))
	{
		DbgPrint("Target_MFC_Application.exe found!\r\n");
		base_adres_of_target_application = ImageInfo->ImageBase;
		targetProcessId = (ULONG)ProcessId;
		DbgPrint("Target_MFC_Application : %p\r\n", base_adres_of_target_application);
		DbgPrint("ProcessId : %p\r\n", ProcessId);
	}
	return STATUS_SUCCESS;
}
