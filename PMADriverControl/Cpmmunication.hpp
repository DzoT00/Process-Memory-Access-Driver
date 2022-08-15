//Communication.hpp : Defines CTL CODES that must comply CTL CODES determined in file "Cpmmunication.h" of ProcessMemoryAccessDriver poject
//and also defines the structures required to generate data (read or write) sent to or received from a kernel-level driver
#pragma once
#include <Windows.h>
#include "winioctl.h"

#define IO_GET_IMAGE_BASE_ADRESS CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_SPECIAL_ACCESS)//было IO_GET_DLLADRESS
#define IO_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_SPECIAL_ACCESS)
#define IO_WRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN,0x803,METHOD_BUFFERED,FILE_SPECIAL_ACCESS)
#define IO_REQUEST_PROCESSID CTL_CODE(FILE_DEVICE_UNKNOWN,0x804,METHOD_BUFFERED,FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_READ_REQUEST
{
	ULONG ProcessId;
	ULONGLONG Address;
	PVOID pBuff;
	ULONG size;

}KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;

typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG ProcessId;
	ULONGLONG Address;
	PVOID pBuff;
	ULONG size;

}KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;