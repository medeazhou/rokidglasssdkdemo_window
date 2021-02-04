
// RokidGlassMFCDemoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RokidGlassMFCDemo.h"
#include "RokidGlassMFCDemoDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#include <dsound.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

/** 回调函数类型 */
typedef enum ROKIDGLASS_MESSAGE_TYPE {
	ROKIDGLASS_ROTATION,
	ROKIDGLASS_GAMEROTATION,
	ROKIDGLASS_RAWSENSOR,
	ROKIDGLASS_KEY,
	ROKIDGLASS_TOUCH,
	ROKIDGLASS_PROXIMITYSENSOR,
	ROKIDGLASS_LIGHTSENSOR,
	ROKIDGLASS_CAMERA,
	ROKIDGLASS_AUDIO
};

class CRokidGlassData
{
	// 构造
public:
	CRokidGlassData(int type, float* data, int dataLen, unsigned long timeStamp, int senseId, int senseStatus);
	CRokidGlassData(int type, int keyCode, BOOLEAN status, int lightStrength, int eventType, int slideLength, int reserval);
	CRokidGlassData(int type, BYTE* pBuf, int bufLen, VideoFormat* format);
	CRokidGlassData(int type, BYTE* pBuf, int bufLen);
	// 标准构造函数
	~CRokidGlassData();

	int type;
	float* data;
	int dataLen;
	unsigned long timeStamp;
	int senseId;
	int senseStatus;

	int keyCode;
	BOOLEAN status;

	int eventType;
	int slideLength;

	int lightStrength;

	BYTE* pBuf;
	int bufLen;
	VideoFormat format;
};


CRokidGlassData::CRokidGlassData(int type, float* data, int dataLen, unsigned long timeStamp, int senseId, int senseStatus)
{
	this->type = type;
	this->data = new float[dataLen];
	this->dataLen = dataLen;
	memcpy(this->data, data, sizeof(float) * dataLen);
	this->timeStamp = timeStamp;
	this->senseId = senseId;
	this->senseStatus = senseStatus;

}

CRokidGlassData::CRokidGlassData(int type, int keyCode, BOOLEAN status, int lightStrength, int eventType, int slideLength, int reserval)
{
	this->type = type;
	this->keyCode = keyCode;
	this->status = status;
	this->lightStrength = lightStrength;
	this->eventType = eventType;
	this->slideLength = slideLength;
}


CRokidGlassData::CRokidGlassData(int type, BYTE* pBuf, int bufLen, VideoFormat* format)
{
	this->type = type;
	this->bufLen = bufLen;
	this->pBuf = new BYTE[bufLen];
	memcpy(this->pBuf, pBuf, bufLen);
	this->format.type = format->type;
	this->format.width = format->width;
	this->format.height = format->height;
}

CRokidGlassData::CRokidGlassData(int type, BYTE* pBuf, int bufLen) {
	this->type = type;
	this->bufLen = bufLen;
	this->pBuf = new BYTE[bufLen];
	memcpy(this->pBuf, pBuf, bufLen);
}


CRokidGlassData::~CRokidGlassData()
{
	if (this->data) {
		delete this->data;
	}

	if (this->pBuf) {
		delete this->pBuf;
	}
}

#pragma comment(lib, "RokidGlassSDK.lib")

//Rokid Glass callback 代理
void onRotationVectorEvent(unsigned long timeStamp, float* quaternion) {
	CRokidGlassData data(ROKIDGLASS_ROTATION, quaternion, 4, timeStamp, 0, 0);
	SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_RokidGlassMessage, (WPARAM)&data, 0);
}

void onGameRotationVectorEvent(unsigned long timeStamp, float* quaternion) {
	CRokidGlassData data(ROKIDGLASS_GAMEROTATION, quaternion, 4, timeStamp, 0, 0);
	SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_RokidGlassMessage, (WPARAM)&data, 0);
}

void onRawSensorEvent(unsigned long timeStamp, int sensorID, float* data, int status) {
	CRokidGlassData glassData(ROKIDGLASS_RAWSENSOR, data, 3, timeStamp, sensorID, status);
	SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_RokidGlassMessage, (WPARAM)&glassData, 0);
}

void onKeyEvent(int keyCode, bool status) {
	CRokidGlassData data(ROKIDGLASS_KEY, keyCode, status, 0, 0, 0, 0);
	SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_RokidGlassMessage, (WPARAM)&data, 0);
}

void onTouchEvent(int eventType, int len) {
	CRokidGlassData data(ROKIDGLASS_TOUCH, 0, TRUE, 0, eventType, len, 0);
	SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_RokidGlassMessage, (WPARAM)&data, 0);
}

void onProximitySensorEvent(bool state) {
	CRokidGlassData data(ROKIDGLASS_PROXIMITYSENSOR, 0, state, 0, 0, 0, 0);
	SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_RokidGlassMessage, (WPARAM)&data, 0);
}

void onLightSensorEvent(int value) {
	CRokidGlassData data(ROKIDGLASS_LIGHTSENSOR, 0, 0, value, 0, 0, 0);
	SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_RokidGlassMessage, (WPARAM)&data, 0);
}

void onCaptureEvent(double timestamp, VideoFormat* format, BYTE* pBuf, long size) {

	CRokidGlassData data(ROKIDGLASS_CAMERA, pBuf, size, format);
	SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_RokidGlassMessage, (WPARAM)&data, 0);
}

void onAudioEvent(BYTE* pBuf, long frames) {

	CRokidGlassData data(ROKIDGLASS_AUDIO, pBuf, frames);
	SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_RokidGlassMessage, (WPARAM)&data, 0);
}


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


// CRokidGlassMFCDemoDlg 对话框



CRokidGlassMFCDemoDlg::CRokidGlassMFCDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROKIDGLASSMFCDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	glass = 0;
}

void CRokidGlassMFCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRokidGlassMFCDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_RokidGlassMessage, OnRokidGlassMessage)
	ON_BN_CLICKED(IDC_START, &CRokidGlassMFCDemoDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CRokidGlassMFCDemoDlg::OnBnClickedStop)
END_MESSAGE_MAP()

LRESULT CRokidGlassMFCDemoDlg::OnRokidGlassMessage(WPARAM wParam, LPARAM lParam)
{
	CRokidGlassData* data = (CRokidGlassData*)wParam;

	switch (data->type) {
	case ROKIDGLASS_ROTATION: {
		CString str;
		str.Format(L"%f", data->data[0]);
		SetDlgItemText(IDC_RO_1, str);

		str.Format(L"%f", data->data[1]);
		SetDlgItemText(IDC_RO_2, str);

		str.Format(L"%f", data->data[2]);
		SetDlgItemText(IDC_RO_3, str);

		str.Format(L"%f", data->data[3]);
		SetDlgItemText(IDC_RO_4, str);
		break;
	}
	case ROKIDGLASS_GAMEROTATION: {
		CString str;
		str.Format(L"%f", data->data[0]);
		SetDlgItemText(IDC_GAME_1, str);

		str.Format(L"%f", data->data[1]);
		SetDlgItemText(IDC_GAME_2, str);

		str.Format(L"%f", data->data[2]);
		SetDlgItemText(IDC_GAME_3, str);

		str.Format(L"%f", data->data[3]);
		SetDlgItemText(IDC_GAME_4, str);
		break;
	}
	case ROKIDGLASS_RAWSENSOR: {
		if (data->senseId == ACCELEROMETER) {
			CString str;
			str.Format(L"%f", data->data[0]);
			SetDlgItemText(IDC_AC_1, str);

			str.Format(L"%f", data->data[1]);
			SetDlgItemText(IDC_AC_2, str);

			str.Format(L"%f", data->data[2]);
			SetDlgItemText(IDC_AC_3, str);

			SetDlgItemInt(IDC_AC_STATUS, data->senseStatus);
		}
		else if (data->senseId == MAGNETIC) {
			CString str;
			str.Format(L"%f", data->data[0]);
			SetDlgItemText(IDC_MA_1, str);

			str.Format(L"%f", data->data[1]);
			SetDlgItemText(IDC_MA_2, str);

			str.Format(L"%f", data->data[2]);
			SetDlgItemText(IDC_MA_3, str);

			SetDlgItemInt(IDC_MA_STATUS, data->senseStatus);
		}
		else if (data->senseId == GYROSCOPE) {
			CString str;
			str.Format(L"%f", data->data[0]);
			SetDlgItemText(IDC_GY_1, str);

			str.Format(L"%f", data->data[1]);
			SetDlgItemText(IDC_GY_2, str);

			str.Format(L"%f", data->data[2]);
			SetDlgItemText(IDC_GY_3, str);

			SetDlgItemInt(IDC_GY_STATUS, data->senseStatus);
		}

		break;
	}
	case ROKIDGLASS_KEY: {
		if (data->keyCode == BACK_KEY) {
			SetDlgItemText(IDC_KEYCODE, L"返回键");

		}
		else if (data->keyCode == POWER_KEY) {
			SetDlgItemText(IDC_KEYCODE, L"熄屏键");
		}

		if (data->status)
			SetDlgItemText(IDC_KEY_STATUS, L"Pressed");
		else
			SetDlgItemText(IDC_KEY_STATUS, L"Released");
		break;
	}
	case ROKIDGLASS_TOUCH: {
		switch (data->eventType) {
		case IDLE:
			SetDlgItemText(IDC_TOUCH_TYPE, L"TOUCH IDLE");
			break;
		case PRESS:
			SetDlgItemText(IDC_TOUCH_TYPE, L"TOUCH Pressed");
			break;
		case SHORT_PRESS:
			SetDlgItemText(IDC_TOUCH_TYPE, L"TOUCH short Pressed");
			break;
		case LONG_PRESS:
			SetDlgItemText(IDC_TOUCH_TYPE, L"TOUCH Long Pressed");
			break;
		case FORWARD_SLIDE:
			SetDlgItemText(IDC_TOUCH_TYPE, L"TOUCH 向前滑动");
			break;
		case BACKWARD_SLIDE:
			SetDlgItemText(IDC_TOUCH_TYPE, L"TOUCH 向后滑动");
			break;
		case REPORTED:
			SetDlgItemText(IDC_TOUCH_TYPE, L"TOUCH REPORTED");
			break;
		}

		SetDlgItemInt(IDC_TOUCH_TYPE_LEN, data->slideLength, true);
		break;
	}
	case ROKIDGLASS_PROXIMITYSENSOR:

		if (data->status == TRUE) {
			SetDlgItemText(IDC_PRO_STATUS, L"戴上");
		}
		else {
			SetDlgItemText(IDC_PRO_STATUS, L"未戴上");
		}
		break;
	case ROKIDGLASS_LIGHTSENSOR:
		SetDlgItemInt(IDC_LIGHT_STRENG, data->lightStrength);
		break;
	case ROKIDGLASS_CAMERA: {
		
		if (data->format.type == JPEG) {
			
			HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, data->bufLen);
			void* pData = GlobalLock(hGlobal);
			memcpy(pData, data->pBuf, data->bufLen);
			GlobalUnlock(hGlobal);

			IStream* pStream = NULL;
			if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK) {
				CImage image;
				if (SUCCEEDED(image.Load(pStream))) {
					int width = image.GetWidth();
					int height = image.GetHeight();
					CRect rect;

					GetDlgItem(IDC_PICTURE)->GetWindowRect(&rect);//将窗口矩形选中到picture控件上   
					ScreenToClient(&rect);//将客户区选中到Picture控件表示的矩形区域内   
					GetDlgItem(IDC_PICTURE)->MoveWindow(rect.left, rect.top, width, height, TRUE);//将窗口移动到Picture控件表示的矩形区域  

					CWnd* pWnd = GetDlgItem(IDC_PICTURE);
					//获得picture控件所在的矩形区域
					pWnd->GetClientRect(&rect);
					//获得picture控件的DC
					CDC* pDC = pWnd->GetDC();
					//将图片画到Picture控件表示的矩形区域
					image.Draw(pDC->m_hDC, 0, 0);
					//image.StretchBlt(pDC->m_hDC, 0, 0, width /3, height /3, 0, 0, width, height, SRCCOPY);
					//释放picture控件的DC
					ReleaseDC(pDC);
				}
			}

			pStream->Release();

			GlobalFree(hGlobal);
			
		}
		else if (data->format.type == RGB) {
			//TODO: later will to implement.
			//do nothing
		}
		else if (data->format.type == YUV) {
			//TODO: later will to implement.
			//do nothing
		}
		else {
			//TODO: later will to implement.
			//do nothing
		}

		break;
	}
	case ROKIDGLASS_AUDIO:
		//TODO: do nothing.
		if(wavFP) {
			fwrite(data->pBuf, 1, data->bufLen * GlassGetAudioFrameSize(glass), wavFP);
			fflush(wavFP);
		}

		SetDlgItemText(IDC_MIC, L"正在录音中...，文件保存在运行目录中的mic.raw");

		break;
	default:
		break;
	}


	//delete data;
	return 0;
}

// CRokidGlassMFCDemoDlg 消息处理程序

BOOL CRokidGlassMFCDemoDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRokidGlassMFCDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if ((nID & 0xFFF0) == SC_CLOSE) {
			if (glass) {
				GlassStopCapture(glass);
				GlassClose(glass);
				glass == 0;
			}
		}

		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRokidGlassMFCDemoDlg::OnPaint()
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
HCURSOR CRokidGlassMFCDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRokidGlassMFCDemoDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码

	EventCallback.onRotationVectorEvent = onRotationVectorEvent;
	EventCallback.onGameRotationVectorEvent = onGameRotationVectorEvent;
	EventCallback.onRawSensorEvent = onRawSensorEvent;
	EventCallback.onKeyEvent = onKeyEvent;
	EventCallback.onTouchEvent = onTouchEvent;
	EventCallback.onProximitySensorEvent = onProximitySensorEvent;
	EventCallback.onLightSensorEvent = onLightSensorEvent;
	EventCallback.onCaptureEvent = onCaptureEvent;
	EventCallback.onAudioEvent = onAudioEvent;

	glass = GlassInitial(&EventCallback);

	if (!glass) {
		TRACE(traceAppMsg, 0, "Unable Initial glass!\n");
		return;
	}

	//TODO: here need a thread to open device.
	while (true) {
		TRACE(traceAppMsg, 0, "try to open glass...\n");
		if (GlassOpen(glass) == true)
			break;
		Sleep(1000);
	}

	int firwareVersion = GlassGetFirmwareVersion(glass);
	CString snStr(GlassGetSerialNumber(glass));

	SetDlgItemInt(IDC_FIRM_VERSION, firwareVersion, TRUE);
	SetDlgItemText(IDC_DeviceSN, snStr);

	GlassStartCapture(glass);
	this->audioFrameSize = GlassGetAudioFrameSize(glass);

	if (!wavFP) {
		fopen_s(&wavFP, "mic.raw", "wb");
	}
	GlassStartAudio(glass);
}


void CRokidGlassMFCDemoDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (glass) {
		GlassStopCapture(glass);
		GlassClose(glass);
		if (wavFP) {
			fclose(wavFP);
		}
		glass == 0;
	}
}
