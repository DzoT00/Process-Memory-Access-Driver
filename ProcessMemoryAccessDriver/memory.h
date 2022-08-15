//memory.h : Declaration of memory management functions

#pragma once
#pragma warning (disable : 4047 4024)
#include "ntifs.h"
#include <ntdef.h>

// API function from ntoskrnl.exe which we use
// to copy memory to and from an user process.
NTSTATUS NTAPI MmCopyVirtualMemory
(
    PEPROCESS SourceProcess,
    PVOID SourceAddress,
    PEPROCESS TargetProcess,
    PVOID TargetAddress,
    SIZE_T BufferSize,
    KPROCESSOR_MODE PreviousMode,
    PSIZE_T ReturnSize
);

NTSTATUS KernelReadVirtualMemoru(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size);

NTSTATUS KernelWriteVirtualMemoru(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size);