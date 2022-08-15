
// Target_MFC_Application.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTargetMFCApplicationApp:
// Сведения о реализации этого класса: Target_MFC_Application.cpp
//

class CTargetMFCApplicationApp : public CWinApp
{
public:
	CTargetMFCApplicationApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTargetMFCApplicationApp theApp;
