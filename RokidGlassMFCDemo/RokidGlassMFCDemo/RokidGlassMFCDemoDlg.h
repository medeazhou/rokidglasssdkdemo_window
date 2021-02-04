
// RokidGlassMFCDemoDlg.h: 头文件
//

#pragma once

#include <glass.h>
#include <iostream>
#include <fstream>

#define WM_RokidGlassMessage (WM_USER + 100)

// CRokidGlassMFCDemoDlg 对话框
class CRokidGlassMFCDemoDlg : public CDialogEx
{
// 构造
public:
	CRokidGlassMFCDemoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROKIDGLASSMFCDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	GlassEvent EventCallback;
	GlassHandle glass;
	int audioFrameSize;
	FILE* wavFP;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRokidGlassMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
};
