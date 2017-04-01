
// HexagonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Hexagon.h"
#include "HexagonDlg.h"

#define M_PI       3.14159265358979323846

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// CHexagonDlg dialog




CHexagonDlg::CHexagonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHexagonDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHexagonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHexagonDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CHexagonDlg message handlers

BOOL CHexagonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	DrawAndSave();
	//DrawAllHexagonsFloat();
	//DrawAllHexagonsUInt();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHexagonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();

		using namespace Gdiplus;
		CPaintDC dc(this);
		Graphics graphics(dc.GetSafeHdc());
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		PointF pts[3];
		pts[0].X = 0.0f;
		pts[0].Y = 0.0f;
		pts[1].X = -5.0f;
		pts[1].Y = 8.6f;
		pts[2].X = 5.0f;
		pts[2].Y = 8.6f;

		CRect rect;
		GetClientRect(&rect);
		graphics.TranslateTransform(10.0f, 10.0f);
		//graphics.TranslateTransform(rect.Width()/2.0f, rect.Height()/2.0f);


		//Pen pen(Color(255,0,0), 1);
		//graphics.DrawPolygon(&pen, pts, 3);
		SolidBrush brush(Color(255,0,0));
		graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

		graphics.RotateTransform(60.0f);
		//graphics.DrawPolygon(&pen, pts, 3);
		graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

		graphics.RotateTransform(60.0f);
		//graphics.DrawPolygon(&pen, pts, 3);
		graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

		graphics.RotateTransform(60.0f);
		//graphics.DrawPolygon(&pen, pts, 3);
		graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

		graphics.RotateTransform(60.0f);
		//graphics.DrawPolygon(&pen, pts, 3);
		graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

		graphics.RotateTransform(60.0f);
		//graphics.DrawPolygon(&pen, pts, 3);
		graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHexagonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CHexagonDlg::DrawAndSave()
{
	using namespace Gdiplus;
	Bitmap bmp(22, 22, PixelFormat32bppARGB);
	Graphics graphics(&bmp);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	PointF pts[3];
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -5.0f;
	pts[1].Y = 8.66f;
	pts[2].X = 5.0f;
	pts[2].Y = 8.66f;

	CRect rect;
	GetClientRect(&rect);
	graphics.TranslateTransform(10.0f, 10.0f);
	//graphics.TranslateTransform(rect.Width()/2.0f, rect.Height()/2.0f);


	//Pen pen(Color(255,0,0), 1);
	//graphics.DrawPolygon(&pen, pts, 3);
	SolidBrush brush(Color(255,0,0));
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	Gdiplus::Status status = bmp.Save(L"d:\\Data\\Pictures\\hex.png", &pngClsid, NULL);
	return status == Gdiplus::Ok ? true : false;
}

bool CHexagonDlg::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	using namespace Gdiplus;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return false;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return false;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return true;  // Success
		}    
	}

	free(pImageCodecInfo);
	return false;  // Failure
}

Gdiplus::Bitmap* CHexagonDlg::Draw(Gdiplus::PointF pts[3])
{
	using namespace Gdiplus;
	Bitmap* bmp = new Bitmap(22, 22, PixelFormat32bppARGB);
	Graphics graphics(bmp);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	CRect rect;
	GetClientRect(&rect);
	graphics.TranslateTransform(10.0f, 10.0f);
	//graphics.TranslateTransform(rect.Width()/2.0f, rect.Height()/2.0f);


	//Pen pen(Color(255,0,0), 1);
	//graphics.DrawPolygon(&pen, pts, 3);
	SolidBrush brush(Color(255,0,0));
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	graphics.RotateTransform(60.0f);
	//graphics.DrawPolygon(&pen, pts, 3);
	graphics.FillPolygon(&brush, pts, 3, Gdiplus::FillModeAlternate);

	return bmp;
}

void CHexagonDlg::DrawAllHexagonsFloat()
{
	using namespace Gdiplus;
	Bitmap* canvas = new Bitmap(22, 22, PixelFormat32bppARGB);

	PointF pts[3];
	// radius = 10
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -5.0f;
	pts[1].Y = 8.66f;
	pts[2].X = 5.0f;
	pts[2].Y = 8.66f;

	Bitmap* pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 10.0f);
	delete pBmp;
	pBmp = NULL;

	// radius = 9
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -4.5f;
	pts[1].Y = 7.79f;
	pts[2].X = 4.5f;
	pts[2].Y = 7.79f;

	pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 9.0f);
	delete pBmp;
	pBmp = NULL;

	// radius = 8
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -4.0f;
	pts[1].Y = 6.93f;
	pts[2].X = 4.0f;
	pts[2].Y = 6.93f;

	pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 8.0f);
	delete pBmp;
	pBmp = NULL;

	// radius = 7
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -3.5f;
	pts[1].Y = 6.06f;
	pts[2].X = 3.5f;
	pts[2].Y = 6.06f;

	pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 7.0f);
	delete pBmp;
	pBmp = NULL;

	// radius = 6
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -3.0f;
	pts[1].Y = 5.2f;
	pts[2].X = 3.0f;
	pts[2].Y = 5.2f;

	pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 6.0f);
	delete pBmp;
	pBmp = NULL;

	// radius = 5
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -2.5f;
	pts[1].Y = 4.33f;
	pts[2].X = 2.5f;
	pts[2].Y = 4.33f;

	pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 5.0f);
	delete pBmp;
	pBmp = NULL;

	// radius = 4
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -2.0f;
	pts[1].Y = 3.46f;
	pts[2].X = 2.0f;
	pts[2].Y = 3.46f;

	pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 4.0f);
	delete pBmp;
	pBmp = NULL;

	// radius = 3
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -1.5f;
	pts[1].Y = 2.60f;
	pts[2].X = 1.5f;
	pts[2].Y = 2.60f;

	pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 3.0f);
	delete pBmp;
	pBmp = NULL;

	// radius = 2
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -1.0f;
	pts[1].Y = 1.73f;
	pts[2].X = 1.0f;
	pts[2].Y = 1.73f;

	pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 2.0f);
	delete pBmp;
	pBmp = NULL;

	// radius = 1
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -0.5f;
	pts[1].Y = 0.87f;
	pts[2].X = 0.5f;
	pts[2].Y = 0.87f;

	pBmp = Draw(pts);
	DrawCanvasFloat(canvas, pBmp, 1.0f);
	delete pBmp;
	pBmp = NULL;

	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	Gdiplus::Status status = canvas->Save(L"D:\\Data\\Pictures\\hex1.png", &pngClsid, NULL);
	//return status == Gdiplus::Ok ? true : false;

	delete canvas;
	canvas = NULL;

}

void CHexagonDlg::DrawAllHexagonsUInt()
{
	using namespace Gdiplus;
	Bitmap* canvas = new Bitmap(22, 22, PixelFormat32bppARGB);

	PointF pts[3];
	// radius = 10
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -5.0f;
	pts[1].Y = 8.66f;
	pts[2].X = 5.0f;
	pts[2].Y = 8.66f;

	Bitmap* pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 10);
	delete pBmp;
	pBmp = NULL;

	// radius = 9
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -4.5f;
	pts[1].Y = 7.79f;
	pts[2].X = 4.5f;
	pts[2].Y = 7.79f;

	pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 9);
	delete pBmp;
	pBmp = NULL;

	// radius = 8
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -4.0f;
	pts[1].Y = 6.93f;
	pts[2].X = 4.0f;
	pts[2].Y = 6.93f;

	pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 8);
	delete pBmp;
	pBmp = NULL;

	// radius = 7
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -3.5f;
	pts[1].Y = 6.06f;
	pts[2].X = 3.5f;
	pts[2].Y = 6.06f;

	pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 7);
	delete pBmp;
	pBmp = NULL;

	// radius = 6
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -3.0f;
	pts[1].Y = 5.2f;
	pts[2].X = 3.0f;
	pts[2].Y = 5.2f;

	pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 6);
	delete pBmp;
	pBmp = NULL;

	// radius = 5
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -2.5f;
	pts[1].Y = 4.33f;
	pts[2].X = 2.5f;
	pts[2].Y = 4.33f;

	pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 5);
	delete pBmp;
	pBmp = NULL;

	// radius = 4
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -2.0f;
	pts[1].Y = 3.46f;
	pts[2].X = 2.0f;
	pts[2].Y = 3.46f;

	pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 4);
	delete pBmp;
	pBmp = NULL;

	// radius = 3
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -1.5f;
	pts[1].Y = 2.60f;
	pts[2].X = 1.5f;
	pts[2].Y = 2.60f;

	pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 3);
	delete pBmp;
	pBmp = NULL;

	// radius = 2
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -1.0f;
	pts[1].Y = 1.73f;
	pts[2].X = 1.0f;
	pts[2].Y = 1.73f;

	pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 2);
	delete pBmp;
	pBmp = NULL;

	// radius = 1
	pts[0].X = 0.0f;
	pts[0].Y = 0.0f;
	pts[1].X = -0.5f;
	pts[1].Y = 0.87f;
	pts[2].X = 0.5f;
	pts[2].Y = 0.87f;

	pBmp = Draw(pts);
	DrawCanvasUInt(canvas, pBmp, 1);
	delete pBmp;
	pBmp = NULL;

	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	Gdiplus::Status status = canvas->Save(L"D:\\Data\\Pictures\\hexUInt.png", &pngClsid, NULL);
	//return status == Gdiplus::Ok ? true : false;

	delete canvas;
	canvas = NULL;

}

void CHexagonDlg::DrawCanvasFloat(Gdiplus::Bitmap* canvas, Gdiplus::Bitmap* pMask, float val)
{
	if(canvas==NULL || pMask==NULL)
		return;

	union floatUni
	{
		float f;
		UINT u;
	};

	floatUni uni;
	uni.f = val;

	UINT* pixelsMask = NULL;
	UINT* pixelsCanvas = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataMask;
	Rect rect(0, 0, pMask->GetWidth(), pMask->GetHeight() );
	BitmapData bitmapDataCanvas;
	Rect rect2(0, 0, canvas->GetWidth(), canvas->GetHeight() );

	pMask->LockBits(
		&rect,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataMask );

	canvas->LockBits(
		&rect2,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataCanvas );

	pixelsMask = (UINT*)bitmapDataMask.Scan0;
	pixelsCanvas = (UINT*)bitmapDataCanvas.Scan0;

	if( !pixelsMask || !pixelsCanvas)
		return;

	UINT col = 0;
	int stride = bitmapDataMask.Stride >> 2;
	for(UINT row = 0; row < bitmapDataMask.Height; ++row)
	{
		for(col = 0; col < bitmapDataMask.Width; ++col)
		{
			UINT index = row * stride + col;
			BYTE nAlpha = 0;

			nAlpha = (pixelsMask[index] & 0xff0000)>>16;

			if(nAlpha>0)
			{
				pixelsCanvas[index] = uni.u;
			}
		}
	}

	pMask->UnlockBits(&bitmapDataMask);
	canvas->UnlockBits(&bitmapDataCanvas);

}

void CHexagonDlg::DrawCanvasUInt(Gdiplus::Bitmap* canvas, Gdiplus::Bitmap* pMask, UINT val)
{
	if(canvas==NULL || pMask==NULL)
		return;

	UINT* pixelsMask = NULL;
	UINT* pixelsCanvas = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataMask;
	Rect rect(0, 0, pMask->GetWidth(), pMask->GetHeight() );
	BitmapData bitmapDataCanvas;
	Rect rect2(0, 0, canvas->GetWidth(), canvas->GetHeight() );

	pMask->LockBits(
		&rect,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataMask );

	canvas->LockBits(
		&rect2,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataCanvas );

	pixelsMask = (UINT*)bitmapDataMask.Scan0;
	pixelsCanvas = (UINT*)bitmapDataCanvas.Scan0;

	if( !pixelsMask || !pixelsCanvas)
		return;

	UINT col = 0;
	int stride = bitmapDataMask.Stride >> 2;
	for(UINT row = 0; row < bitmapDataMask.Height; ++row)
	{
		for(col = 0; col < bitmapDataMask.Width; ++col)
		{
			UINT index = row * stride + col;
			BYTE nAlpha = 0;

			nAlpha = (pixelsMask[index] & 0xff0000)>>16;

			if(nAlpha>0)
			{
				pixelsCanvas[index] = 0xff000000 | val;
			}
		}
	}

	pMask->UnlockBits(&bitmapDataMask);
	canvas->UnlockBits(&bitmapDataCanvas);

}
