
// CircleCoverDlg.h : header file
//

#pragma once
#include <dvec.h>
#include <fvec.h>


// CCircleCoverDlg dialog
class CCircleCoverDlg : public CDialogEx
{
// Construction
public:
	CCircleCoverDlg(CWnd* pParent = NULL);	// standard constructor
	~CCircleCoverDlg();

private:
	void LoadSrcImage1();
	void LoadSrcImage2();

	float ComputeDistance(float fPointX1, float fPointY1, float fPointX2, float fPointY2);

	float MaxDistance(float a, float b, float c, float d);

	void Draw();

	Gdiplus::Bitmap* m_pSrcBitmap1;

	Gdiplus::Bitmap* m_pSrcBitmap2;

	Gdiplus::Bitmap* m_pCanvas;

	int m_numPixels;

	int m_totalLength;

	bool m_firstImage;

	float m_fPointX;
	
	float m_fPointY;

	UINT  m_wTimerRes;

	int m_nSpeed;



// Dialog Data
	enum { IDD = IDD_CIRCLECOVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
