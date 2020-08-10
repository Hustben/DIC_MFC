#pragma once


// Calibration 对话框

class Calibration : public CDialogEx
{
	DECLARE_DYNAMIC(Calibration)

public:
	Calibration(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Calibration();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIBRATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_fx;
	double m_fy;
	double m_u0;
	double m_v0;
	double m_k1;
	double m_k2;
	double m_k3;
	double m_p1;
	double m_p2;
public:
	virtual BOOL OnInitDialog();
};
