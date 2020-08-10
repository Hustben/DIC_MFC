// Calibration.cpp: 实现文件
//

#include "pch.h"
#include "MFC_DIC.h"
#include "Calibration.h"
#include "afxdialogex.h"
#include "MFC_DICDlg.h"
#include<opencv2/opencv.hpp>

using namespace cv;


// Calibration 对话框

IMPLEMENT_DYNAMIC(Calibration, CDialogEx)

Calibration::Calibration(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALIBRATION, pParent)
	, m_fx(0)
	, m_fy(0)
	, m_u0(0)
	, m_v0(0)
	, m_k1(0)
	, m_k2(0)
	, m_k3(0)
	, m_p1(0)
	, m_p2(0)
{

}

Calibration::~Calibration()
{
}

void Calibration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fx);
	DDX_Text(pDX, IDC_EDIT2, m_fy);
	DDX_Text(pDX, IDC_EDIT3, m_u0);
	DDX_Text(pDX, IDC_EDIT4, m_v0);
	DDX_Text(pDX, IDC_EDIT5, m_k1);
	DDX_Text(pDX, IDC_EDIT6, m_k2);
	DDX_Text(pDX, IDC_EDIT7, m_k3);
	DDX_Text(pDX, IDC_EDIT8, m_p1);
	DDX_Text(pDX, IDC_EDIT9, m_p2);
}


BEGIN_MESSAGE_MAP(Calibration, CDialogEx)
END_MESSAGE_MAP()


// Calibration 消息处理程序


BOOL Calibration::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
