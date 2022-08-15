//Events.h : declaration the event handler called when the executable image is run

#pragma once
#include <ntifs.h>
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo);