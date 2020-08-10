
// MFC_DICDlg.h: 头文件
//
#pragma once
#include<opencv2/opencv.hpp>


// CMFCDICDlg 对话框
class CMFCDICDlg : public CDialogEx
{
// 构造
public:
	CMFCDICDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_DIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCalibration();

	afx_msg void OnBnClickedUndisort();
	afx_msg void OnBnClickedPixel();
	afx_msg void OnBnClickedMatchtemplate();
private:
	CStatic m_hWnd1;
	CStatic m_hWnd2;
public:
	afx_msg void OnBnClickedDisplacement();
private:
	CButton m_Static_Figure;
};
