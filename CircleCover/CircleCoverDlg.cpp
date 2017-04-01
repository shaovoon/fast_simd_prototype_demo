
// CircleCoverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CircleCover.h"
#include "CircleCoverDlg.h"
#include "afxdialogex.h"
#include <cmath>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCircleCoverDlg dialog




CCircleCoverDlg::CCircleCoverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCircleCoverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	TIMECAPS tc;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
	{
		OutputDebugStringW(L"Error in timeGetDevCaps");
	}

	m_wTimerRes = 1;//min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(m_wTimerRes); 
}

void CCircleCoverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircleCoverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CCircleCoverDlg::~CCircleCoverDlg()
{
	delete m_pSrcBitmap1;
	m_pSrcBitmap1 = NULL;
	delete m_pSrcBitmap2;
	m_pSrcBitmap2 = NULL;
	delete m_pCanvas;
	m_pCanvas = NULL;

	timeEndPeriod(m_wTimerRes);
}
// CCircleCoverDlg message handlers

BOOL CCircleCoverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	LoadSrcImage1();
	LoadSrcImage2();

	if(m_pSrcBitmap1!=NULL)
		m_pCanvas = new Gdiplus::Bitmap(m_pSrcBitmap1->GetWidth(),m_pSrcBitmap1->GetHeight(), PixelFormat32bppARGB);

	m_numPixels = 0;
	m_nSpeed = 10;

	CRect rect;
	GetClientRect(&rect);
	CRect rectWin;
	GetWindowRect(&rectWin);

	int extraWidth = rectWin.Width() - rect.Width();
	int extraHeight = rectWin.Height() - rect.Height();

	SetWindowPos(NULL, 0, 0, extraWidth+m_pSrcBitmap1->GetWidth(), extraHeight+m_pSrcBitmap1->GetHeight(), 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCircleCoverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCircleCoverDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		Draw();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCircleCoverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCircleCoverDlg::Draw()
{
	if(!m_pSrcBitmap1||!m_pSrcBitmap2||!m_pCanvas)
		return;

	DWORD allStartTime = timeGetTime();

	using namespace Gdiplus;
	CPaintDC dc(this);
	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	if(m_numPixels==0)
	{
		if(m_firstImage)
			graphics.DrawImage(m_pSrcBitmap1,0,0,m_pSrcBitmap1->GetWidth(),m_pSrcBitmap1->GetHeight());
		else
			graphics.DrawImage(m_pSrcBitmap2,0,0,m_pSrcBitmap2->GetWidth(),m_pSrcBitmap2->GetHeight());

		return;
	}

	//graphics.DrawImage(m_pSrcBitmap,0,0,m_pSrcBitmap->GetWidth(),m_pSrcBitmap->GetHeight());

	UINT* pixelsCanvas = NULL;
	UINT* pixelsImage1 = NULL;
	UINT* pixelsImage2 = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataCanvas;
	Rect rect(0, 0, m_pCanvas->GetWidth(), m_pCanvas->GetHeight() );
	BitmapData bitmapDataImage1;
	Rect rect2(0, 0, m_pSrcBitmap1->GetWidth(), m_pSrcBitmap1->GetHeight() );
	BitmapData bitmapDataImage2;
	Rect rect3(0, 0, m_pSrcBitmap2->GetWidth(), m_pSrcBitmap2->GetHeight() );

	m_pSrcBitmap1->LockBits(
		&rect2,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataImage1 );

	m_pSrcBitmap2->LockBits(
		&rect3,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataImage2 );

	m_pCanvas->LockBits(
		&rect,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataCanvas );

	pixelsCanvas = (UINT*)bitmapDataCanvas.Scan0;

	if(m_firstImage)
	{
		pixelsImage1 = (UINT*)bitmapDataImage1.Scan0;
		pixelsImage2 = (UINT*)bitmapDataImage2.Scan0;
	}
	else
	{
		pixelsImage1 = (UINT*)bitmapDataImage2.Scan0;
		pixelsImage2 = (UINT*)bitmapDataImage1.Scan0;
	}

	if( !pixelsCanvas || !pixelsImage1 || !pixelsImage2 )
		return;

	UINT col = 0;
	int stride = bitmapDataCanvas.Stride >> 2;

	float fPointX = m_fPointX; // x coordinates of the circle center
	float fPointY = m_fPointY; // y coordinates of the circle center

	// vector of 4 x coordinates of the circle center
	F32vec4 vecPointX(fPointX);
	// vector of 4 y coordinates of the circle center
	F32vec4 vecPointY(fPointY);
	// vector of 4 radius of the circle center
	//F32vec4 vecRadius(fRadius);
	F32vec4 vecRadius((float)(m_numPixels));

	float fy=0.0f;
	float fx=0.0f;

	UINT color = 0xff00ff00;
	DWORD startTime = timeGetTime();
	for(UINT row = 0, fy=0.0f; row < bitmapDataCanvas.Height; row+=1, fy+=1.0f)
	{
		for(col = 0, fx=0.0f; col < bitmapDataCanvas.Width; col+=4, fx+=4.0f)
		{
			// calculate the index of destination pixel array
			int index = row * stride + col;

			// vector of X coordinates of the 4 pixels, it is inverse of of little endian
			F32vec4 vecX(fx+3.0f, fx+2.0f, fx+1.0f, fx+0.0f);
			// vector of Y coordinates of the 4 pixels
			F32vec4 vecY((float)(fy));

			// Subtract center X from the pixel X coordinates
			vecX -= vecPointX;
			// Subtract center Y from the pixel Y coordinates
			vecY -= vecPointY;

			// compute the square of X, that is X * X = X to power of 2
			vecX = vecX * vecX;
			// compute the square of Y, that is Y * Y = Y to power of 2
			vecY = vecY * vecY;

			// Add up the X square and Y square
			F32vec4 vecHypSq = vecX + vecY;

			// Find the hypotenuse by computing square root
			F32vec4 vecHyp = sqrt(vecHypSq);

			// Generate the mask for condition of hypotenuse < radius
			F32vec4 mask = cmple(vecHyp, vecRadius);

			// all 4 pixel in mask vector, falls within the width
			if(col+3<bitmapDataCanvas.Width)
			{
				UINT origPixel = pixelsCanvas[index+0];
				pixelsCanvas[index+0] = pixelsImage2[index+0] & (__m128(mask)).m128_u32[0];
				pixelsCanvas[index+0] |= origPixel & ~((__m128(mask)).m128_u32[0]);

				origPixel = pixelsCanvas[index+1];
				pixelsCanvas[index+1] = pixelsImage2[index+1] & (__m128(mask)).m128_u32[1];
				pixelsCanvas[index+1] |= origPixel & ~((__m128(mask)).m128_u32[1]);

				origPixel = pixelsCanvas[index+2];
				pixelsCanvas[index+2] = pixelsImage2[index+2] & (__m128(mask)).m128_u32[2];
				pixelsCanvas[index+2] |= origPixel & ~((__m128(mask)).m128_u32[2]);

				origPixel = pixelsCanvas[index+3];
				pixelsCanvas[index+3] = pixelsImage2[index+3] & (__m128(mask)).m128_u32[3];
				pixelsCanvas[index+3] |= origPixel & ~((__m128(mask)).m128_u32[3]);
			}
			else // all 4 pixel in mask vector do not falls within the width: have to test 1 by 1.
			{
				UINT origPixel = pixelsCanvas[index+0];
				pixelsCanvas[index+0] = pixelsImage2[index+0] & (__m128(mask)).m128_u32[0];
				pixelsCanvas[index+0] |= origPixel & ~((__m128(mask)).m128_u32[0]);

				if(col+1<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+1];

					pixelsCanvas[index+1] = pixelsImage2[index+1] & (__m128(mask)).m128_u32[1];
					pixelsCanvas[index+1] |= origPixel & ~((__m128(mask)).m128_u32[1]);
				}

				if(col+2<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+2];

					pixelsCanvas[index+2] = pixelsImage2[index+2] & (__m128(mask)).m128_u32[2];
					pixelsCanvas[index+2] |= origPixel & ~((__m128(mask)).m128_u32[2]);
				}

				if(col+3<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+3];

					pixelsCanvas[index+3] = pixelsImage2[index+3] & (__m128(mask)).m128_u32[3];
					pixelsCanvas[index+3] |= origPixel & ~((__m128(mask)).m128_u32[3]);
				}
			}
		}
	}

	DWORD endTime = timeGetTime();

	m_pSrcBitmap1->UnlockBits(&bitmapDataImage1);
	m_pSrcBitmap2->UnlockBits(&bitmapDataImage2);
	m_pCanvas->UnlockBits(&bitmapDataCanvas);

	DWORD allEndTime = timeGetTime();
	char buf[50];
	sprintf(buf, "All time:%d, Loop time:%d\n", allEndTime-allStartTime, endTime-startTime);
	OutputDebugStringA(buf);

	graphics.DrawImage(m_pCanvas,0,0,m_pCanvas->GetWidth(),m_pCanvas->GetHeight());
}

float CCircleCoverDlg::ComputeDistance(float fPointX1, float fPointY1, float fPointX2, float fPointY2)
{
	// Subtract center X from the pixel X coordinates
	float X = fPointX2 - fPointX1;
	// Subtract center Y from the pixel Y coordinates
	float Y = fPointY2 - fPointY1;

	// compute the square of X, that is X * X = X to power of 2
	X = X * X;
	// compute the square of Y, that is Y * Y = Y to power of 2
	Y = Y * Y;

	// Add up the X square and Y square
	float hypSq = X + Y;

	// Find the hypotenuse by computing square root
	float hyp = std::sqrt(hypSq);

	return hyp;
}

float CCircleCoverDlg::MaxDistance(float a, float b, float c, float d)
{
	if(a>=b&&a>=c&&a>=d)
		return a;
	if(b>=a&&b>=c&&b>=d)
		return b;
	if(c>=a&&c>=b&&c>=d)
		return c;
	if(d>=a&&d>=b&&d>=c)
		return d;

	return a;
}

void CCircleCoverDlg::LoadSrcImage1()
{
	HINSTANCE hResInstance = AfxGetResourceHandle( );

	HRSRC res = FindResource(hResInstance,
		MAKEINTRESOURCE(IDB_BITMAP1),L"BINARY");
	if (res) 
	{
		HGLOBAL mem = LoadResource(hResInstance, res);
		void *data = LockResource(mem);
		size_t len = SizeofResource(hResInstance, res);

		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
		LPVOID pvData = GlobalLock( hGlobal );
		memcpy(pvData,data,len);
		GlobalUnlock(hGlobal);

		LPSTREAM pStream = NULL;
		HRESULT hr = CreateStreamOnHGlobal( hGlobal, TRUE, &pStream );

		using namespace Gdiplus;
		m_pSrcBitmap1 = new Bitmap(pStream,false);

		pStream->Release();
	}
}

void CCircleCoverDlg::LoadSrcImage2()
{
	HINSTANCE hResInstance = AfxGetResourceHandle( );

	HRSRC res = FindResource(hResInstance,
		MAKEINTRESOURCE(IDB_BITMAP2),L"BINARY");
	if (res) 
	{
		HGLOBAL mem = LoadResource(hResInstance, res);
		void *data = LockResource(mem);
		size_t len = SizeofResource(hResInstance, res);

		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
		LPVOID pvData = GlobalLock( hGlobal );
		memcpy(pvData,data,len);
		GlobalUnlock(hGlobal);

		LPSTREAM pStream = NULL;
		HRESULT hr = CreateStreamOnHGlobal( hGlobal, TRUE, &pStream );

		using namespace Gdiplus;
		m_pSrcBitmap2 = new Bitmap(pStream,false);

		pStream->Release();
	}
}

void CCircleCoverDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{
		if(m_numPixels>=m_totalLength)
		{
			KillTimer(1);
			m_firstImage = !m_firstImage;
			m_numPixels = 0;
			m_nSpeed = 10;
			return;
		}

		m_nSpeed = (int)(m_nSpeed * 1.1f);
		m_numPixels+=m_nSpeed;
		Invalidate(false);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CCircleCoverDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
}


void CCircleCoverDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_numPixels!=0)
		return;

	CRect rect;
	GetClientRect(&rect);

	m_fPointX = (float)(point.x);
	m_fPointY = (float)(point.y);

	float a = ComputeDistance(0.0f, 0.0f, m_fPointX, m_fPointY);
	float b = ComputeDistance(0.0f, (float)(rect.bottom), m_fPointX, m_fPointY);
	float c = ComputeDistance((float)(rect.right), 0.0f, m_fPointX, m_fPointY);
	float d = ComputeDistance((float)(rect.right), (float)(rect.bottom), m_fPointX, m_fPointY);

	m_totalLength = (int)(MaxDistance(a, b, c, d));
	m_numPixels = 0;

	SetTimer(1, 30, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}
