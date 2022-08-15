//ProcMemyAccessDriver.c : Defines the entry point of the driver (DriverEntry) and Unload function.

#pragma warning (disable : 4047 4024)
#include "ProcMemyAccessDriver.h"
#include "Communication.h"
#include "data.h"
#include "Events.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRregistryPatch)
{
	UNREFERENCED_PARAMETER(pRregistryPatch);
	pDriverObject->DriverUnload = UnloadDriver;
	DbgPrint("driver Load\r\n");
	PsSetLoadImageNotifyRoutine(ImageLoadCallback);

	RtlInitUnicodeString(&DeviceName, L"\\Device\\ThirdParty_PMADriver");
	RtlInitUnicodeString(&SymLinkName, L"\\DosDevices\\ProcMemAccessDrv");


	IoCreateDevice(pDriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	IoCreateSymbolicLink(&SymLinkName, &DeviceName);

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;

	pDeviceObject->Flags |= DO_DIRECT_IO;
	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
	IoDeleteSymbolicLink(&SymLinkName);
	IoDeleteDevice(pDriverObject->DeviceObject);
	DbgPrint("driver Unload\r\n");
	return STATUS_SUCCESS;
}