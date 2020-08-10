
// MFC_DICDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFC_DIC.h"
#include "MFC_DICDlg.h"
#include "afxdialogex.h"
#include "Calibration.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include<math.h>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace cv;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Mat intrinsic = Mat(3, 3, CV_32FC1);
Mat distCoeffs;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCDICDlg 对话框



CMFCDICDlg::CMFCDICDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_DIC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDICDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Pic1, m_hWnd1);
	DDX_Control(pDX, IDC_STATIC_Pic2, m_hWnd2);
	DDX_Control(pDX, IDC_CALIBRATION, m_Static_Figure);
}

BEGIN_MESSAGE_MAP(CMFCDICDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CALIBRATION, &CMFCDICDlg::OnBnClickedCalibration)
	ON_BN_CLICKED(IDC_PIXEL, &CMFCDICDlg::OnBnClickedPixel)
	ON_BN_CLICKED(IDC_MATCHTEMPLATE, &CMFCDICDlg::OnBnClickedMatchtemplate)
	ON_BN_CLICKED(IDC_DISPLACEMENT, &CMFCDICDlg::OnBnClickedDisplacement)
END_MESSAGE_MAP()


// CMFCDICDlg 消息处理程序

BOOL CMFCDICDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	namedWindow("view1", WINDOW_AUTOSIZE);
	HWND hWnd1 = (HWND)cvGetWindowHandle("view1");
	HWND hParent1 = ::GetParent(hWnd1);
	::SetParent(hWnd1, GetDlgItem(IDC_STATIC_Pic1)->m_hWnd);
	::ShowWindow(hParent1, SW_HIDE);

	namedWindow("view2", WINDOW_AUTOSIZE);
	HWND hWnd2 = (HWND)cvGetWindowHandle("view2");
	HWND hParent2 = ::GetParent(hWnd2);
	::SetParent(hWnd2, GetDlgItem(IDC_STATIC_Pic2)->m_hWnd);
	::ShowWindow(hParent2, SW_HIDE);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCDICDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDICDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDICDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//相机标定程序
void CMFCDICDlg::OnBnClickedCalibration()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<String> files;
	glob("F:/calibration", files);// 读取文件夹

	// 定义变量
	vector<vector<Point2f>> imagePoints;
	vector<vector<Point3f>> objectPoints;
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.001);
	int numCornersHor = 7;// 定义横纵方向的角点个数
	int numCornersVer = 7;
	int numSquares = 50;
	vector<Point3f> obj;
	for (int i = 0; i < numCornersHor; i++)
		for (int j = 0; j < numCornersVer; j++)
			obj.push_back(Point3f((float)j * numSquares, (float)i * numSquares, 0));
	// 发现棋盘格与绘制
	Size s;
	for (int i = 0; i < files.size(); i++) {
		printf("image file : %s \n", files[i].c_str());
		Mat image = imread(files[i]);// 读取图片
		s = image.size();
		Mat gray;
		cvtColor(image, gray, COLOR_BGR2GRAY);
		vector<Point2f> corners;
		bool ret = findChessboardCorners(gray, Size(7, 7), corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);
		if (ret) {
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), criteria);
			imagePoints.push_back(corners);
			objectPoints.push_back(obj);
		}
	}
	// 相机校正
	vector<Mat> rvecs;
	vector<Mat> tvecs;
	intrinsic.ptr<float>(0)[0] = 1;
	intrinsic.ptr<float>(1)[1] = 1;
	calibrateCamera(objectPoints, imagePoints, s, intrinsic, distCoeffs, rvecs, tvecs);
	MessageBox(TEXT("相机标定完成"));
	Calibration dlg;
	dlg.m_fx = floor(intrinsic.at<double>(0, 0)* 10000.000f+0.5)/ 10000.000f;
	dlg.m_fy = floor(intrinsic.at<double>(1, 1) * 10000.000f + 0.5) / 10000.000f;
	dlg.m_u0 = floor(intrinsic.at<double>(0, 2) * 10000.000f + 0.5) / 10000.000f;
	dlg.m_v0 = floor(intrinsic.at<double>(1, 2) * 10000.000f + 0.5) / 10000.000f;
	dlg.m_k1 = floor(distCoeffs.at<double>(0, 0) * 10000.000f + 0.5) / 10000.000f;
	dlg.m_k2 = floor(distCoeffs.at<double>(0, 1) * 10000.000f + 0.5) / 10000.000f;
	dlg.m_k3 = floor(distCoeffs.at<double>(0, 2) * 10000.000f + 0.5) / 10000.000f;
	dlg.m_p1 = floor(distCoeffs.at<double>(0, 3) * 10000.000f + 0.5) / 10000.000f;
	dlg.m_p2 = floor(distCoeffs.at<double>(0, 4) * 10000.000f + 0.5) / 10000.000f;
	dlg.DoModal();
}


void CMFCDICDlg::OnBnClickedUndisort()
{
	// TODO: 在此添加控件通知处理程序代码
}

double k;
void CMFCDICDlg::OnBnClickedPixel()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<vector<Point2f>> imagePoints;
	vector<vector<Point3f>> objectPoints;
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.001);
	int numCornersHor = 7;
	int numCornersVer = 7;
	int numSquares = 50;
	vector<Point3f> obj;
	for (int i = 0; i < numCornersHor; i++)
		for (int j = 0; j < numCornersVer; j++)
			obj.push_back(Point3f((float)j * numSquares, (float)i * numSquares, 0));
	// 发现棋盘格与绘制
	Size s;
	Mat dst = imread("F:/vertical/1.png");
	Mat image;
	undistort(dst, image, intrinsic, distCoeffs);
	s = image.size();
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	vector<Point2f> corners;
	bool ret = findChessboardCorners(gray, Size(7, 7), corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);// 查找交点
	if (ret) {
		cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), criteria);
		imagePoints.push_back(corners);
		objectPoints.push_back(obj);
	}
	//计算像素当量
	double t = 0;
	double* p = new double[6];
	for (size_t i = 0; i < 6; i++) {
		p[i] = corners[i + 1].y - corners[i].y;// 计算相邻角点间的y坐标方向的像素距离
	}
	int sum = 0;
	for (int i = 0; i < 6; i++) {
		sum += p[i];
	}
	double ave = sum / 6;// 计算前六个角点距离的平均值
	const double ChessDis = 8.8;// 定义角点间的实际距离
	k = ChessDis /ave;// 计算像素当量
	CString str1;
	str1.Format(TEXT("像素当量为：%lf"),k);
	MessageBox(str1);
}

int x;
void CMFCDICDlg::OnBnClickedMatchtemplate()
{
	// TODO: 在此添加控件通知处理程序代码
	Mat orginal, final;
	Mat image1 = imread("F:/image/demo 01.png");
	undistort(image1, orginal, intrinsic, distCoeffs);
	Mat image2 = imread("F:/image/demo 60.png");
	undistort(image2, final, intrinsic, distCoeffs);
	Mat temp = imread("F:/template/1.png");
	int width1 = orginal.cols - temp.cols + 1;
	int height1 = orginal.rows - temp.rows + 1;
	Mat result1(width1, height1, CV_32FC1);
	matchTemplate(orginal, temp, result1, CV_TM_SQDIFF, Mat());
	normalize(result1, result1, 0, 1, NORM_MINMAX, -1, Mat());
	Point minLoc1;
	Point maxLoc1;
	double min1, max1;
	Mat copy1;
	orginal.copyTo(copy1);
	minMaxLoc(result1, &min1, &max1, &minLoc1, &maxLoc1, Mat());
	rectangle(copy1, Rect(minLoc1.x, minLoc1.y, temp.cols, temp.rows), Scalar(0, 0, 255), 8, 8);

	Mat imagedst1;	//以下操作获取图形控件尺寸并以此改变图片尺寸	
	CRect rect1;
	GetDlgItem(IDC_STATIC_Pic1)->GetClientRect(&rect1);
	Rect dst1(rect1.left, rect1.top, rect1.right, rect1.bottom);
	resize(copy1, imagedst1, cv::Size(rect1.Width(), rect1.Height()));
	imshow("view1", imagedst1);

	int width2 = final.cols - temp.cols + 1;
	int height2 = final.rows - temp.rows + 1;
	Mat result2(width2, height2, CV_32FC1);
	matchTemplate(final, temp, result2, CV_TM_SQDIFF, Mat());
	normalize(result2, result2, 0, 1, NORM_MINMAX, -1, Mat());
	Point minLoc2;
	Point maxLoc2;
	double min2, max2;
	Mat copy2;
	final.copyTo(copy2);
	minMaxLoc(result2, &min2, &max2, &minLoc2, &maxLoc2, Mat());
	rectangle(copy2, Rect(minLoc2.x, minLoc2.y, temp.cols, temp.rows), Scalar(0, 0, 255), 8, 8);

	Mat imagedst2;	//以下操作获取图形控件尺寸并以此改变图片尺寸	
	CRect rect2;
	GetDlgItem(IDC_STATIC_Pic2)->GetClientRect(&rect2);
	Rect dst2(rect2.left, rect2.top, rect2.right, rect2.bottom);
	resize(copy2, imagedst2, cv::Size(rect2.Width(), rect2.Height()));
	imshow("view2", imagedst2);
	x = minLoc2.y - minLoc1.y;

}

void CMFCDICDlg::OnBnClickedDisplacement()
{
	// TODO: 在此添加控件通知处理程序代码
	float distance = x * k;
	CString str1;
	str1.Format(TEXT("输出位移为：%f"), distance);
	MessageBox(str1);
	//// TODO:  在此添加控件通知处理程序代码


	CWnd* pwnd = GetDlgItem(IDC_STATIC);
	CDC* pdc = pwnd->GetDC();
	pwnd->Invalidate();
	pwnd->UpdateWindow();


	pdc->Rectangle(0, 0, 320, 320);


	CPen* ppenRed = new CPen;


	ppenRed->CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CGdiObject* pOldpen = pdc->SelectObject(ppenRed);
	pdc->MoveTo(20, 20);
	pdc->LineTo(20, 300);
	pdc->LineTo(300, 300);


	CString str;
	str.Format(_T("0"));
	pdc->SetTextColor(RGB(0, 255, 0));
	pdc->TextOutW(20, 303, str);
	str.Format(_T("50"));
	pdc->SetTextColor(RGB(0, 255, 0));
	pdc->TextOutW(70, 303, str);
	str.Format(_T("100"));
	pdc->SetTextColor(RGB(0, 255, 0));
	pdc->TextOutW(120, 303, str);
	str.Format(_T("150"));
	pdc->SetTextColor(RGB(0, 255, 0));
	pdc->TextOutW(170, 303, str);
	str.Format(_T("200"));
	pdc->SetTextColor(RGB(0, 255, 0));
	pdc->TextOutW(220, 303, str);
	str.Format(_T("250"));
	pdc->SetTextColor(RGB(0, 255, 0));
	pdc->TextOutW(270, 303, str);


	for (int i = 10; i < 300; i += 5) {
		if ((i & 1) == 0) {
			pdc->MoveTo(i + 10, 300);
			pdc->LineTo(i + 10, 303);
		}
		else {
			pdc->MoveTo(i + 10, 300);
			pdc->LineTo(i + 10, 302);
		}
	}
}
