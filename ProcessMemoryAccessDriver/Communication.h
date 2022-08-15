//Communication.h : Defines CTL CODES that must comply CTL CODES determined in file "Cpmmunication.hpp" of PMADriverControl poject,
//declares IO control functions for communication with the user-level control application
//and also defines the structures required to generate data (read or write) sent to or received from a user-level control application

#pragma once
#include "ntifs.h"

#define IO_GET_IMAGE_BASE_ADRESS CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_SPECIAL_ACCESS)
#define IO_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_SPECIAL_ACCESS)
#define IO_WRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN,0x803,METHOD_BUFFERED,FILE_SPECIAL_ACCESS)
#define IO_REQUEST_PROCESSID CTL_CODE(FILE_DEVICE_UNKNOWN,0x804,METHOD_BUFFERED,FILE_SPECIAL_ACCESS)

NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP Irp);

typedef struct _KERNEL_READ_REQUEST
{
	ULONG ProcessId;
	ULONGLONG Address;
	PVOID pBuff;
	ULONG Size;

}KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;

typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG ProcessId;
	ULONGLONG Address;
	PVOID pBuff;
	ULONG Size;

}KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;