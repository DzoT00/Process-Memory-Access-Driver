//Communication.c : Defines CTL CODES that must comply CTL CODES determined in file "Cpmmunication.hpp" of PMADriverControl poject,
//Defines IO control functions for communication with the user-level control application
//and also defines the structures required to generate data (read or write) sent to or received from a user-level control application

#pragma warning (disable : 4100 4022)
#include "Communication.h"
#include "data.h"
#include "memory.h"

NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS status = STATUS_SUCCESS;
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	DbgPrint("Create call success\r\n");
	return status;
}

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS status = STATUS_SUCCESS;
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	DbgPrint("Conection terminated!\r\n");
	return status;
}

NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//DBG_UNREFERENCED_PARAMETER(pDeviceObject);
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	ULONG ByteIO = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	ULONG ControlCode = stack->Parameters.DeviceIoControl.IoControlCode;

	if (ControlCode == IO_GET_IMAGE_BASE_ADRESS)
	{
		DbgPrint("GetBaseAddress requested!\r\n");
		DbgPrint("\r\n");
		PULONGLONG OutPut = (PULONGLONG)Irp->AssociatedIrp.SystemBuffer;
		*OutPut = base_adres_of_target_application;

		status = STATUS_SUCCESS;
		ByteIO = sizeof(*OutPut);
	}
	else if (ControlCode == IO_REQUEST_PROCESSID)//последовательность?
	{
		DbgPrint("IO_REQUEST_PROCESSID !\r\n");
		PULONGLONG OutPut = (PULONGLONG)Irp->AssociatedIrp.SystemBuffer;
		*OutPut = targetProcessId;

		status = STATUS_SUCCESS;
		ByteIO = sizeof(*OutPut);
	}
	else if (ControlCode == IO_READ_REQUEST)
	{
		DbgPrint("IO_READ_REQUEST !\r\n");
		PKERNEL_READ_REQUEST ReadInput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS Process;//Process = EPROCESS structure
		if (NT_SUCCESS(PsLookupProcessByProcessId(ReadInput->ProcessId, &Process)))
		{
			DbgPrint("ReadInput->ProcessId hex %p\r\n", ReadInput->ProcessId);
			DbgPrint("ReadInput->Address %p\r\n", ReadInput->Address);
			
			//if (NT_SUCCESS(KernelReadVirtualMemoru(Process, ReadInput->Address, ReadInput->pBuff, ReadInput->Size)))
			if (NT_SUCCESS(KernelReadVirtualMemoru(Process, ReadInput->Address, ReadInput->pBuff, ReadInput->Size)))//теперь здесь есть проверка на успешность чтения из памяти!
			{
				status = STATUS_SUCCESS;
				ByteIO = sizeof(KERNEL_READ_REQUEST);
				DbgPrint("read memory return STATUS_SUCESS\r\n");
				//ObDereferenceObject(Process) нужно вызвать чтобы освободить указатель на структуру EPROCESS !!!
				//DbgPrint("ta, %p\r\n", ta);
			}
			else
			{
				DbgPrint("read memory failed!\r\n");
				//return STATUS_ACCESS_DENIED;
			}
			DbgPrint("\r\n");
			//ByteIO = 0;
		}

	}
	//в случае чтения памяти нужно подключится и к процессу источнику данных,
	//и к процессу для записи данных, поэтому для чтения памяти нужно добавить функцию KeStackAttachProcess
	//а также KeUnstackDetachProcess
	else if (ControlCode == IO_WRITE_REQUEST)//
	{
		DbgPrint("IO_WRITE_REQUEST !\r\n");
		PKERNEL_WRITE_REQUEST WriteInput = (PKERNEL_WRITE_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS Process;
		if (NT_SUCCESS(PsLookupProcessByProcessId(WriteInput->ProcessId, &Process)))
		{
			KernelWriteVirtualMemoru(Process, WriteInput->pBuff, WriteInput->Address, WriteInput->Size);//здесь нет проверки на успешность чтения из памяти!
			status = STATUS_SUCCESS;
			ByteIO = sizeof(KERNEL_WRITE_REQUEST);
		}
	}
	else
	{
		ByteIO = 0;
		DbgPrint("else unsupported IO request!\r\n");
	}
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = ByteIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}