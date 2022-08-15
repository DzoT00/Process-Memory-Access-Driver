# Process-Memory-Access-Driver
Simple KMDF kernel mode driver for handling read/write memory requests into arbitrary processes.


### Background

Kernel based anti-cheat drivers (EAC, BattleEye) block or monitor requests for
interfacing with the memory from the game process. The simplest way to bypass anti-cheat protections from the kernel is to use your own kernel mode driver.

This repository contains a training example of such a driver.
The solution consists of three projects:
1. PMADriverControl is a user mode application that interacts with the kernel mode driver.
2. ProcessMemoryAccessDriver - the kernel mode driver itself.
3. Target_MFC_Application is an example of a target application whose address space is accessed.
This application displays the addresses and values of the target variables in its GUI.

### Building

1. Install [Visual Studio](https://www.visualstudio.com/).
2. Install the [Windows Driver Kit](https://docs.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk).
3. select the target OS to compile in the project settings ProcessMemoryAccessDriver->Driver settings->General->Target OS Version.
3. Set the solution configuration to `x64` and `Release` and build the solution.

I compile the driver for Window_7, to use the driver on operating systems starting from Window_8 and above,
you need to change the target platform for compiling to your version of the OS in the ProcessMemoryAccessDriver project settings.

### Usage

For practical use you will need a driver signing certificate. For development
purposes you can enable [test-signing mode](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/the-testsigning-boot-configuration-option).

You will need to either install and run the driver as a service with `CreateService` or use a function like `NtLoadDriver`,
but I use [OSRLOADER](https://www.osronline.com/article.cfm%5Earticle=157.htm)
Once the driver is loaded, you can open a handle to the driver:

```cpp
#include "driver_config.h"
#include "driver_codes.h"

HANDLE driver = CreateFileW(
    DRIVER_DEVICE_PATH, 
    GENERIC_READ | GENERIC_WRITE, 
    FILE_SHARE_READ | FILE_SHARE_WRITE, 
    0, 
    OPEN_EXISTING, 
    0, 0);
```

To issue read or write requests, you send the driver a control code:
```cpp
IO_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_SPECIAL_ACCESS)
```
Еhis is implemented through:
1. Obtaining handle of driver:
```cpp
devicehandle = CreateFile(L"\\\\.\\ProcMemAccessDrv", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
```
2. Creating a KernelInterface class object to access the driver through the methods of that class:
```cpp
KernelInterface Driver = KernelInterface(devicehandle);
```
3. Accessing the "ReadVirtualMemory" method of the "Driver" object of the "KernelInterface" class
```cpp
uint32_t integer_variable_a = Driver.ReadVirtualMemory<uint32_t>(ProccesId, address_int_a, sizeof(uint32_t));
float float_variable_b = Driver.ReadVirtualMemory<float>(ProccesId, address_float_b, sizeof(float));
byte byte_variable_c = Driver.ReadVirtualMemory<byte>(ProccesId, address_byte_c, sizeof(byte));
```
The target and source parameters should be reversed if issuing a write request.

Memory read method implemented in the "KernelInterface" class:
```cpp
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

		//to read memory on OS Window_8 and above, you must call the DeviceIoControl function with the following parameters:
		//if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
		// 
		//But here, to read memory, a call to the DeviceIoControl function with other parameters is used (which is incorrect).
		//Non-standard parameters of the function are used because for corrective operation of the function on the Window_7
		//You must create your own exception handler that is not present in the OS Window_7
		//example for Window_7: if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), 0, 0, &Bytes, NULL))

		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), 0, 0, &Bytes, NULL))
		{
			return Buffer;
		}
		return Buffer;
 }
```
I compile the driver for Window_7, to use the driver on operating systems starting from Window_8 and above,
you need to change the target platform for compiling to your version of the OS in the ProcessMemoryAccessDriver project settings
and use the DeviceIoControl subroutine call with the parameters corresponding to your OS version!
