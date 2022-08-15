
// PMADriverControl.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы

// CPMADriverControlApp:
// Сведения о реализации этого класса: PMADriverControl.cpp
//

class CPMADriverControlApp : public CWinApp
{
public:
	CPMADriverControlApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CPMADriverControlApp theApp;
