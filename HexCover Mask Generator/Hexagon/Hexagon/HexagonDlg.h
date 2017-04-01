
// HexagonDlg.h : header file
//

#pragma once


// CHexagonDlg dialog
class CHexagonDlg : public CDialog
{
// Construction
public:
	CHexagonDlg(CWnd* pParent = NULL);	// standard constructor
private:
	bool GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	bool DrawAndSave();
	Gdiplus::Bitmap* Draw(Gdiplus::PointF pts[3]);

	void DrawAllHexagonsFloat();

	void DrawCanvasFloat(Gdiplus::Bitmap* canvas, Gdiplus::Bitmap* pMask, float val);

	void DrawAllHexagonsUInt();

	void DrawCanvasUInt(Gdiplus::Bitmap* canvas, Gdiplus::Bitmap* pMask, UINT val);

// Dialog Data
	enum { IDD = IDD_HEXAGON_DIALOG };

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
