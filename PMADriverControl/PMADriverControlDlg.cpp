
// PMADriverControlDlg.cpp: файл реализации
//This file contains event handlers for pressing GUI buttons.
//Event handlers implement the receipt of the driver HANDLE,
//and the creation of an interface for interacting with the driver through the handler

#include "pch.h"
#include "framework.h"
#include "PMADriverControl.h"
#include "PMADriverControlDlg.h"
#include "afxdialogex.h"
#include "kernelinterface.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CPMADriverControlDlg



CPMADriverControlDlg::CPMADriverControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PMADRIVERCONTROL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPMADriverControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPMADriverControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPMADriverControlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPMADriverControlDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CPMADriverControlDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CPMADriverControlDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// Обработчики сообщений CPMADriverControlDlg

BOOL CPMADriverControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CPMADriverControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CPMADriverControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CPMADriverControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CString output;
HWND ec;
HANDLE devicehandle = NULL;
ULONGLONG Address;
ULONGLONG address_int_a;
ULONGLONG address_float_b;
ULONGLONG address_byte_c;
ULONG ProccesId;
//KernelInterface Driver;//стоит сделать объект глобальным?

void CPMADriverControlDlg::OnBnClickedButton1()//OpenDevice
{
	//obtaining handle of driver
	devicehandle = CreateFile(L"\\\\.\\ProcMemAccessDrv", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if(devicehandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"not valid value", 0, 0);
		return;
	}
	MessageBox(L"valid value", 0, 0);

	//creating a KernelInterface class object to access the driver through the methods of that class
	KernelInterface Driver = KernelInterface(devicehandle);//using a new constructor
	ProccesId = Driver.GetProcessId();
	Address = Driver.GetClientAddress();
	address_int_a = Address + 0x704C;
	address_float_b = Address + 0x7048;
	address_byte_c = Address + 0x7044;

	ec = ::GetDlgItem(AfxGetMainWnd()->m_hWnd, IDC_EDIT1);
	output.Format(_T("Base addres of target application: %p \r\naddress variable 'a': %p \r\naddress variable 'b': %p \r\naddress variable 'c': %p \r\nProccesId: %d"),
				Address,
				address_int_a,
				address_float_b,
				address_byte_c,
				ProccesId);

		::SetWindowText(ec, output);
}

/*
    std::cout << "Address of MSFT " << Address << std::endl;
    std::cout << "ProccesId of MSFT hex:   " << std::hex << ProccesId << std::endl;
    std::cout << "Address of MSFT hex:   " << std::hex << Address << std::endl;
    std::cout << "Address2 of MSFT hex:   " << std::hex << Address_int << std::endl;*/


void CPMADriverControlDlg::OnBnClickedButton2()//CloseDevice
{
	if (devicehandle != INVALID_HANDLE_VALUE)
		CloseHandle(devicehandle);
}

void CPMADriverControlDlg::OnBnClickedButton6()//ReadData
{
	//::GetDlgItem(AfxGetMainWnd()->m_hWnd
	//обязательно ли создавать обьекты класса каждый раз?
	KernelInterface Driver = devicehandle;

	uint32_t integer_variable_a = Driver.ReadVirtualMemory<uint32_t>(ProccesId, address_int_a, sizeof(uint32_t));
	float float_variable_b = Driver.ReadVirtualMemory<float>(ProccesId, address_float_b, sizeof(float));
	byte byte_variable_c = Driver.ReadVirtualMemory<byte>(ProccesId, address_byte_c, sizeof(byte));
	ec = ::GetDlgItem(AfxGetMainWnd()->m_hWnd, IDC_EDIT1);
	char cc = byte_variable_c;
	output.Format(_T("integer variable 'a' %d address variable 'a': %p \r\nfloat variable 'b' %f address variable 'b': %p \r\nbyte variable 'c': %x address variable 'c': %p \r\nProccesId: %d"),
		integer_variable_a, address_int_a,
		float_variable_b, address_float_b,
		byte_variable_c, address_byte_c,
		ProccesId);

	::SetWindowText(ec, output);

}


void CPMADriverControlDlg::OnBnClickedButton4()//WriteData
{
	//обязательно ли создавать обьекты класса каждый раз?
	KernelInterface Driver = devicehandle;
	uint32_t A = 9;
	float B = 2.f;
	byte C = 0xe8;

	Driver.WriteVirtualMemory(ProccesId, address_int_a, A, sizeof(uint32_t));
	Driver.WriteVirtualMemory(ProccesId, address_float_b, B, sizeof(float));
	Driver.WriteVirtualMemory(ProccesId, address_byte_c, C, sizeof(byte));
}
