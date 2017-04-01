
// HexCoverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HexCover.h"
#include "HexCoverDlg.h"
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


// CHexCoverDlg dialog




CHexCoverDlg::CHexCoverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHexCoverDlg::IDD, pParent)
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

CHexCoverDlg::~CHexCoverDlg()
{
	delete m_pSrcBitmap1;
	m_pSrcBitmap1 = NULL;
	delete m_pSrcBitmap2;
	m_pSrcBitmap2 = NULL;
	delete m_pCanvas;
	m_pCanvas = NULL;

	delete m_pDistanceMask;
	m_pDistanceMask = NULL;
	delete m_pRedMask;
	m_pRedMask = NULL;
	delete m_pSmallDistanceMask;
	m_pSmallDistanceMask = NULL;
	delete m_pSmallRedMask;
	m_pSmallRedMask = NULL;

	timeEndPeriod(m_wTimerRes);
}

void CHexCoverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHexCoverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CHexCoverDlg message handlers

BOOL CHexCoverDlg::OnInitDialog()
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
	// Load image 1
	LoadSrcImage1();
	// Load image 2(should be the same dimension as image 2)
	LoadSrcImage2();
	LoadSmallDistImage();
	LoadSmallRedMaskImage();

	// generate the destination image
	if(m_pSrcBitmap1!=NULL)
		m_pCanvas = new Gdiplus::Bitmap(m_pSrcBitmap1->GetWidth(),m_pSrcBitmap1->GetHeight(), PixelFormat32bppARGB);

	// generate the mask for each pixel, its distance from its hexagon center.
	GenDistanceMask();

	// generate the mask for each pixel for which hexagon does it belongs to.
	GenArrayIndexMask();

	m_numPixels = 0;
	m_nSpeed = 2;
	m_fSpeed = 2.0f;

	// Expand the client size according to the dimension of the first embedded image.
	//================================================================================
	CRect rect;
	GetClientRect(&rect);
	CRect rectWin;
	GetWindowRect(&rectWin);

	int extraWidth = rectWin.Width() - rect.Width();
	int extraHeight = rectWin.Height() - rect.Height();

	SetWindowPos(NULL, 0, 0, extraWidth+m_pSrcBitmap1->GetWidth(), extraHeight+m_pSrcBitmap1->GetHeight(), 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHexCoverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHexCoverDlg::OnPaint()
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
HCURSOR CHexCoverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHexCoverDlg::Draw()
{
	if(!m_pSrcBitmap1||!m_pSrcBitmap2||!m_pCanvas)
		return;

	DWORD allStartTime = timeGetTime();

	using namespace Gdiplus;
	CPaintDC dc(this);
	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	// If the circle is fully expanded, draw the full image, instead of computing which pixel should be visible
	if(m_numPixels==0)
	{
		if(m_firstImage)
			graphics.DrawImage(m_pSrcBitmap1,0,0,m_pSrcBitmap1->GetWidth(),m_pSrcBitmap1->GetHeight());
		else
			graphics.DrawImage(m_pSrcBitmap2,0,0,m_pSrcBitmap2->GetWidth(),m_pSrcBitmap2->GetHeight());

		return;
	}

	UINT* pixelsCanvas = NULL;
	UINT* pixelsImage1 = NULL;
	UINT* pixelsImage2 = NULL;
	UINT* pixelsRedMask = NULL;
	UINT* pixelsDistanceMask = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataCanvas;
	Rect rect(0, 0, m_pCanvas->GetWidth(), m_pCanvas->GetHeight() );
	BitmapData bitmapDataImage1;
	Rect rect2(0, 0, m_pSrcBitmap1->GetWidth(), m_pSrcBitmap1->GetHeight() );
	BitmapData bitmapDataImage2;
	Rect rect3(0, 0, m_pSrcBitmap2->GetWidth(), m_pSrcBitmap2->GetHeight() );
	BitmapData bitmapDataRedMask;
	Rect rect4(0, 0, m_pRedMask->GetWidth(), m_pRedMask->GetHeight() );
	BitmapData bitmapDataDistanceMask;
	Rect rect5(0, 0, m_pDistanceMask->GetWidth(), m_pDistanceMask->GetHeight() );

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

	m_pRedMask->LockBits(
		&rect4,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataRedMask );

	m_pDistanceMask->LockBits(
		&rect5,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataDistanceMask );

	pixelsCanvas = (UINT*)bitmapDataCanvas.Scan0;
	pixelsRedMask = (UINT*)bitmapDataRedMask.Scan0;
	pixelsDistanceMask = (UINT*)bitmapDataDistanceMask.Scan0;

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

	if( !pixelsCanvas || !pixelsImage1 || !pixelsImage2 || !pixelsRedMask || !pixelsDistanceMask )
		return;

	UINT col = 0;
	int stride = bitmapDataCanvas.Stride >> 2;

	float fPointX = m_fPointX; // x coordinates of the circle center
	float fPointY = m_fPointY; // y coordinates of the circle center

	// vector of 4 radius of the circle center
	//F32vec4 vecRadius(fRadius);
	UINT nRadius=m_numPixels;


	ComputeCenterDist();
	DWORD startTime = timeGetTime();
	for(UINT row = 0; row < bitmapDataCanvas.Height; row+=1)
	{
		for(col = 0; col < bitmapDataCanvas.Width; col+=1)
		{
			// calculate the index of destination pixel array
			int index = row * stride + col;

			// Calculate this pixel is belonging to which hexagon index
			// by taking out the alpha channel (0x00ffffff) from its index mask.
			int ArrIndex = 0xffffff & pixelsRedMask[index];

			// Calculate the distance of the pixel from its hexagon center
			// by taking out the alpha channel (0x00ffffff) from its distance mask.
			int distFromHexCenter = 0xffffff & pixelsDistanceMask[index];

			// Calculate the distance of the hexagon center from the mouse point
			int hexDistFromCircleCenter = m_vecHexCenter[ArrIndex].dist;

			// Calculate the difference of circle radius and distance of the 
			// hexagon center from the mouse point
			int result = nRadius - hexDistFromCircleCenter;

			UINT origPixel = pixelsCanvas[index];
			if(result<distFromHexCenter) // if difference is less 
			{ // than pixel distance from hexagon center, draw the original pixel
				pixelsCanvas[index] = origPixel;
			}
			else if(result>=distFromHexCenter) // if difference is equal or more 
			{ // than pixel distance from hexagon center, draw the beneath pixel
				pixelsCanvas[index] = pixelsImage2[index];
			}
		}
	}

	DWORD endTime = timeGetTime();

	m_pSrcBitmap1->UnlockBits(&bitmapDataImage1);
	m_pSrcBitmap2->UnlockBits(&bitmapDataImage2);
	m_pCanvas->UnlockBits(&bitmapDataCanvas);
	m_pRedMask->UnlockBits(&bitmapDataRedMask);
	m_pDistanceMask->UnlockBits(&bitmapDataDistanceMask);

	DWORD allEndTime = timeGetTime();
	char buf[50];
	sprintf(buf, "All time:%d, Loop time:%d\n", allEndTime-allStartTime, endTime-startTime);
	OutputDebugStringA(buf);

	graphics.DrawImage(m_pCanvas,0,0,m_pCanvas->GetWidth(),m_pCanvas->GetHeight());
}

float CHexCoverDlg::ComputeDistance(float fPointX1, float fPointY1, float fPointX2, float fPointY2)
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

float CHexCoverDlg::MaxDistance(float a, float b, float c, float d)
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

void CHexCoverDlg::LoadSrcImage1()
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

void CHexCoverDlg::LoadSrcImage2()
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


void CHexCoverDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{
		if(m_numPixels>=m_totalLength)
		{
			KillTimer(1);
			m_firstImage = !m_firstImage;
			m_numPixels = 0;
			m_nSpeed = 2;
			m_fSpeed = 2.0f;
			return;
		}

		//m_fSpeed = m_fSpeed * 1.05f; // acceleration.
		//m_nSpeed = (int)(m_fSpeed);
		m_numPixels+=m_nSpeed;

		char buf[50];
		sprintf(buf, "m_nSpeed: %d, m_fSpeed: %f", m_nSpeed, m_fSpeed);
		OutputDebugStringA(buf);

		Invalidate(false);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CHexCoverDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
}


void CHexCoverDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_numPixels!=0)
		return;

	CRect rect;
	GetClientRect(&rect);

	m_fPointX = (float)(point.x);
	m_fPointY = (float)(point.y);

	// Calculate the distance of the mouse point from the 4 corners of the client area.
	float a = ComputeDistance(0.0f, 0.0f, m_fPointX, m_fPointY);
	float b = ComputeDistance(0.0f, (float)(rect.bottom), m_fPointX, m_fPointY);
	float c = ComputeDistance((float)(rect.right), 0.0f, m_fPointX, m_fPointY);
	float d = ComputeDistance((float)(rect.right), (float)(rect.bottom), m_fPointX, m_fPointY);

	m_totalLength = (int)(MaxDistance(a, b, c, d));
	m_totalLength += 10;
	m_numPixels = 0;

	SetTimer(1, 30, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}

// Generate the distance mask by blitting m_pSmallDistanceMask repeatedly until it fills the whole m_pDistanceMask.
// m_pSmallDistanceMask is pre-generated by hexagon mask generator project in another solution (not in this solution).
// The magic numbers have to do with the pre-generated hexagon which has a height 18 pixel and radius of 20 pixel
// Pre-generated hexagon is centered in a square image of 22 pixels (extra 2 pixels are allowances)
void CHexCoverDlg::GenDistanceMask()
{
	using namespace Gdiplus;
	m_pDistanceMask = new Bitmap(m_pSrcBitmap1->GetWidth(), m_pSrcBitmap1->GetHeight(), PixelFormat32bppARGB);
	Graphics graph(m_pDistanceMask);

	int widthStep = 21+10;
	int width=-10;
	int bmpWidth = (int)(m_pSrcBitmap1->GetWidth());
	bool widthOdd = true;
	int heightStepOdd = 21;
	int heightStepEven = 21-10;
	int heightStep = 9;
	int height=-10;
	int bmpHeight = (int)(m_pSrcBitmap1->GetHeight());
	bool heightOdd = true;
	while(height<bmpHeight)
	{
		while(width<bmpWidth)
		{
			graph.DrawImage(m_pSmallDistanceMask, width,height,0,0,21,21,UnitPixel);
			width += widthStep;
		}

		heightOdd = !heightOdd;
		height += heightStep;

		if(!heightOdd)
			width = 5;
		else
			width = -10;
	}
}

// Generate the index mask by writing the index into m_pSmallRedMask and blitting m_pSmallRedMask 
// repeatedly until it fills the whole m_pRedMask.
// m_pSmallRedMask is pre-generated by hexagon mask generator project in another solution (not in this solution).
// The magic numbers have to do with the pre-generated hexagon which has a height 18 pixel and radius of 20 pixel
// Pre-generated hexagon is centered in a square image of 22 pixels (extra 2 pixels are allowances)
void CHexCoverDlg::GenArrayIndexMask()
{
	using namespace Gdiplus;
	m_pRedMask = new Bitmap(m_pSrcBitmap1->GetWidth(), m_pSrcBitmap1->GetHeight(), PixelFormat32bppARGB);
	Gdiplus::Bitmap* destImage = new Bitmap(m_pSmallRedMask->GetWidth(), m_pSmallRedMask->GetHeight(), PixelFormat32bppARGB);
	Graphics graph(m_pRedMask);

	int widthStep = 21+10;
	int width=-10;
	int bmpWidth = (int)(m_pSrcBitmap1->GetWidth());
	bool widthOdd = true;
	int heightStepOdd = 21;
	int heightStepEven = 21-10;
	int heightStep = 9;
	int height=-10;
	int bmpHeight = (int)(m_pSrcBitmap1->GetHeight());
	bool heightOdd = true;
	UINT cnt=0;
	m_vecHexCenter.clear();
	while(height<bmpHeight)
	{
		while(width<bmpWidth)
		{
			SetValueToImage(m_pSmallRedMask, destImage, cnt);
			graph.DrawImage(destImage, width,height,0,0,21,21,UnitPixel);
			HexCenter center;
			center.dist=10000;
			center.x = width + 11;
			center.y = height + 11;
			m_vecHexCenter.push_back(center);
			width += widthStep;
			++cnt;
		}

		heightOdd = !heightOdd;
		height += heightStep;

		if(!heightOdd)
			width = 5;
		else
			width = -10;
	}

	delete destImage;
}

// Write the vector index number into the image if the mask pixel is red but noted the alpha channel is set to 0xff
// which needs to be removed later.
void CHexCoverDlg::SetValueToImage(Gdiplus::Bitmap* smallRedMask, Gdiplus::Bitmap* destImage, UINT val)
{
	if(destImage==NULL || smallRedMask==NULL)
		return;

	UINT* pixelsMask = NULL;
	UINT* pixelsDest = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataMask;
	Rect rect(0, 0, smallRedMask->GetWidth(), smallRedMask->GetHeight() );
	BitmapData bitmapDataDest;
	Rect rect2(0, 0, destImage->GetWidth(), destImage->GetHeight() );

	smallRedMask->LockBits(
		&rect,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataMask );

	destImage->LockBits(
		&rect2,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataDest );

	pixelsMask = (UINT*)bitmapDataMask.Scan0;
	pixelsDest = (UINT*)bitmapDataDest.Scan0;

	if( !pixelsMask || !pixelsDest)
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
				pixelsDest[index] = 0xff000000 | val;
			}
		}
	}

	smallRedMask->UnlockBits(&bitmapDataMask);
	destImage->UnlockBits(&bitmapDataDest);
}


void CHexCoverDlg::LoadSmallDistImage()
{
	HINSTANCE hResInstance = AfxGetResourceHandle( );

	HRSRC res = FindResource(hResInstance,
		MAKEINTRESOURCE(IDB_BITMAPSMALLDIST),L"BINARY");
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
		m_pSmallDistanceMask = new Bitmap(pStream,false);

		pStream->Release();
	}
}

void CHexCoverDlg::LoadSmallRedMaskImage()
{
	HINSTANCE hResInstance = AfxGetResourceHandle( );

	HRSRC res = FindResource(hResInstance,
		MAKEINTRESOURCE(IDB_BITMAPSMALLREDMASK),L"BINARY");
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
		m_pSmallRedMask = new Bitmap(pStream,false);

		pStream->Release();
	}
}

// compute the radius using pythagorean theorem
void CHexCoverDlg::ComputeCenterDist()
{
	for(size_t i=0; i<m_vecHexCenter.size(); ++i)
	{
		float fx = m_vecHexCenter[i].x - m_fPointX;
		float fy = m_vecHexCenter[i].y - m_fPointY;

		fx = fx * fx;
		fy = fy * fy;

		m_vecHexCenter[i].dist = (int)(std::sqrt(fx + fy));
	}
}