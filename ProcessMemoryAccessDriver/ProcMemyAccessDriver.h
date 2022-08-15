//ProcMemyAccessDriver.c : Declaration of main driver functions.

#pragma once

#include "ntifs.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRregistryPatch);


NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject);