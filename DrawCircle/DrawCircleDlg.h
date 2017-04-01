
// DrawCircleDlg.h : header file
//

#pragma once
#include <dvec.h>
#include <fvec.h>


// CDrawCircleDlg dialog
class CDrawCircleDlg : public CDialogEx
{
// Construction
public:
	CDrawCircleDlg(CWnd* pParent = NULL);	// standard constructor
	~CDrawCircleDlg();
private:
	void LoadSrcImage();
	UINT* GenGrad(UINT a, UINT b, UINT len);

	float ComputeDistance(float fPointX1, float fPointY1, float fPointX2, float fPointY2);

	int MaxDistance(float a, float b, float c, float d);

	Gdiplus::Bitmap* m_pSrcBitmap;

	UINT     m_wTimerRes;

public:
// Dialog Data
	enum { IDD = IDD_DRAWCIRCLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
