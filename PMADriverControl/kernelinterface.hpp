//kernelinterface.hpp : Defines a class for the interface of interaction of the user-mode application with the kernel-mode driver

#pragma once
#include "Cpmmunication.hpp"

class KernelInterface
{
public:
	HANDLE hDriver;
	KernelInterface(LPCSTR RegistryPatch)
	{
		hDriver = CreateFileA(RegistryPatch, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	}
	//новый конструктор может вызвать проблемы!
	KernelInterface(HANDLE ExistingHandle)
	{
		hDriver = ExistingHandle;
	}

	ULONGLONG GetClientAddress()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			return 0;
		}
		ULONGLONG Address;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_IMAGE_BASE_ADRESS, &Address, sizeof(Address), &Address, sizeof(Address), &Bytes, NULL))
		{
			return Address;
		}
		return 0;
	}

	DWORD GetProcessId()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			return 0;
		}
		ULONG ProcessId;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_REQUEST_PROCESSID, &ProcessId, sizeof(ProcessId), &ProcessId, sizeof(ProcessId), &Bytes, NULL))
		{
			return ProcessId;
		}
		return 0;
	}

	template <typename type>
	type ReadVirtualMemory(ULONG ProcessId, ULONGLONG ReadAddress, SIZE_T Size)
	{
		type Buffer = 0;
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			MessageBoxW(AfxGetMainWnd()->m_hWnd,L"INVALID_HANDLE_VALUE", 0, 0);
			return Buffer;
		}
		KERNEL_READ_REQUEST ReadRequest;

		ReadRequest.ProcessId = ProcessId;
		ReadRequest.Address = ReadAddress;
		ReadRequest.pBuff = &Buffer;
		ReadRequest.size = Size;
		DWORD Bytes;
		/*std::cout << "ProcessId   " << std::hex << ReadRequest.ProcessId << std::endl;
		std::cout << "Address hex:   " << std::hex << ReadRequest.Address << std::endl;
		std::cout << "pBuff hex:   " << std::hex << ReadRequest.pBuff << std::endl;
		std::cout << "size decimal:   " << std::dec << ReadRequest.size << std::endl;*/

		//для чтения памяти на ОС Window_8 и выше необходимо вызвывать функцию DeviceIoControl со следующими параметрами:
		//if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
		// 
		//Но здесь, для чтения памяти, используется вызов функции DeviceIoControl с другими параметрами(что некорректно).
		//Нестандартные параметры функции используются потому что для корретной работы функции на Window_7
		//необходимо создать собственный обработчик исключений, который остсутствует в ОС Window_7 
		//пример для Window_7: if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), 0, 0, &Bytes, NULL))

		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), 0, 0, &Bytes, NULL))
		{
			return Buffer;
		}
		return Buffer;
	}

	template <typename type>
	bool WriteVirtualMemory(ULONG ProcessId, ULONGLONG WriteAddress, type WriteWalue, SIZE_T Size)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		DWORD Bytes;
		KERNEL_WRITE_REQUEST WriteRequest;
		WriteRequest.ProcessId = ProcessId;
		WriteRequest.Address = WriteAddress;
		WriteRequest.pBuff = &WriteWalue;
		WriteRequest.size = Size;

		if (DeviceIoControl(hDriver, IO_WRITE_REQUEST, &WriteRequest, sizeof(WriteRequest), 0, 0, &Bytes, NULL))
		{
			return true;
		}
		return false;
	}
};