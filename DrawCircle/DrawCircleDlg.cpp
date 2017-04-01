
// DrawCircleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawCircle.h"
#include "DrawCircleDlg.h"
#include "afxdialogex.h"
#include <cmath>
#include <MMSystem.h>
#include <immintrin.h>

#pragma comment(lib, "winmm.lib")

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// CDrawCircleDlg dialog

union unionUintFloat
{
	UINT n;
	float f;
};


CDrawCircleDlg::CDrawCircleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDrawCircleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	TIMECAPS tc;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
	{
		// Error; application can't continue.
	}

	m_wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(m_wTimerRes); 

}

CDrawCircleDlg::~CDrawCircleDlg()
{
	delete m_pSrcBitmap;
	m_pSrcBitmap = NULL;

	timeEndPeriod(m_wTimerRes);
}


void CDrawCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrawCircleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CDrawCircleDlg message handlers

BOOL CDrawCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	LoadSrcImage();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
/*
void CDrawCircleDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}
*/

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDrawCircleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Draw Circle without optimization
/*
void CDrawCircleDlg::OnPaint()
{
	if(!m_pSrcBitmap)
		return;

	using namespace Gdiplus;
	CPaintDC dc(this);
	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	UINT* pixelsCanvas = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataCanvas;
	Rect rect(0, 0, m_pSrcBitmap->GetWidth(), m_pSrcBitmap->GetHeight() );

	m_pSrcBitmap->LockBits(
		&rect,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataCanvas );

	pixelsCanvas = (UINT*)bitmapDataCanvas.Scan0;

	if( !pixelsCanvas )
		return;

	UINT col = 0;
	int stride = bitmapDataCanvas.Stride >> 2;

	float fPointX = 50.0f; // x coordinates of the circle center
	float fPointY = 50.0f; // y coordinates of the circle center
	float fRadius = 40.0f; // radius of the circle center

	float fy=0.0f;
	float fx=0.0f;

	UINT color = 0xff00ff00;
	DWORD startTime = timeGetTime();
	for(UINT row = 0, fy=0.0f; row < bitmapDataCanvas.Height; row+=1, fy+=1.0f)
	{
		for(col = 0, fx=0.0f; col < bitmapDataCanvas.Width; col+=1, fx+=1.0f)
		{
			// calculate the index of destination pixel array
			int index = row * stride + col;

			// Subtract center X from the pixel X coordinates
			float X = fx - fPointX;
			// Subtract center Y from the pixel Y coordinates
			float Y = fy - fPointY;

			// compute the square of X, that is X * X = X to power of 2
			X = X * X;
			// compute the square of Y, that is Y * Y = Y to power of 2
			Y = Y * Y;

			// Add up the X square and Y square
			float hypSq = X + Y;

			// Find the hypotenuse by computing square root
			float hyp = std::sqrt(hypSq);

			UINT origPixel = pixelsCanvas[index];
			if(hyp<=fRadius)
			{
				pixelsCanvas[index] = color;
			}
			else
			{
				pixelsCanvas[index] = origPixel;
			}
		}
	}
	DWORD endTime = timeGetTime();
	char buf[50];
	sprintf(buf, "%d\n", endTime-startTime);
	OutputDebugStringA(buf);

	m_pSrcBitmap->UnlockBits(&bitmapDataCanvas);

	graphics.DrawImage(m_pSrcBitmap,0,0,m_pSrcBitmap->GetWidth(),m_pSrcBitmap->GetHeight());
}
*/

/*
// Draw Circle with SIMD intrinsic
void CDrawCircleDlg::OnPaint()
{
	if(!m_pSrcBitmap)
		return;

	using namespace Gdiplus;
	CPaintDC dc(this);
	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	//graphics.DrawImage(m_pSrcBitmap,0,0,m_pSrcBitmap->GetWidth(),m_pSrcBitmap->GetHeight());

	UINT* pixelsCanvas = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataCanvas;
	Rect rect(0, 0, m_pSrcBitmap->GetWidth(), m_pSrcBitmap->GetHeight() );

	m_pSrcBitmap->LockBits(
		&rect,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataCanvas );

	pixelsCanvas = (UINT*)bitmapDataCanvas.Scan0;

	if( !pixelsCanvas )
		return;

	UINT col = 0;
	int stride = bitmapDataCanvas.Stride >> 2;

	float fPointX = 120.0f; // x coordinates of the circle center
	float fPointY = 120.0f; // y coordinates of the circle center
	float fRadius = 110.0f; // radius of the circle center

	// vector of 4 x coordinates of the circle center
	__m128 vecPointX = _mm_set_ps1(fPointX);
	// vector of 4 y coordinates of the circle center
	__m128 vecPointY = _mm_set_ps1(fPointY);
	// vector of 4 radius of the circle center
	__m128 vecRadius = _mm_set_ps1(fRadius);

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
			__m128 vecX= _mm_set_ps(fx+3.0f, fx+2.0f, fx+1.0f, fx+0.0f);
			// vector of Y coordinates of the 4 pixels
			__m128 vecY = _mm_set_ps1(fy);

			// Subtract center X from the pixel X coordinates
			vecX = _mm_sub_ps(vecX, vecPointX);

			// Subtract center Y from the pixel Y coordinates
			vecY = _mm_sub_ps(vecY, vecPointY);

			// compute the square of X, that is X * X = X to power of 2
			vecX = _mm_mul_ps(vecX, vecX);
			// compute the square of Y, that is Y * Y = Y to power of 2
			vecY = _mm_mul_ps(vecY, vecY);

			// Add up the X square and Y square
			__m128 vecHypSq = _mm_add_ps(vecX, vecY);

			// Find the hypotenuse by computing square root
			__m128 vecHyp = _mm_sqrt_ps(vecHypSq);

			// Generate the mask for condition of hypotenuse < radius
			__m128 mask = _mm_cmple_ps(vecHyp, vecRadius);

			// all 4 pixel in mask vector, falls within the width
			if(col+3<bitmapDataCanvas.Width)
			{
				UINT origPixel = pixelsCanvas[index+0];
				pixelsCanvas[index+0] = color & mask.m128_u32[0];
				pixelsCanvas[index+0] |= origPixel & ~mask.m128_u32[0];

				origPixel = pixelsCanvas[index+1];
				pixelsCanvas[index+1] = color & mask.m128_u32[1];
				pixelsCanvas[index+1] |= origPixel & ~mask.m128_u32[1];

				origPixel = pixelsCanvas[index+2];
				pixelsCanvas[index+2] = color & mask.m128_u32[2];
				pixelsCanvas[index+2] |= origPixel & ~mask.m128_u32[2];

				origPixel = pixelsCanvas[index+3];
				pixelsCanvas[index+3] = color & mask.m128_u32[3];
				pixelsCanvas[index+3] |= origPixel & ~mask.m128_u32[3];
			}
			else // all 4 pixel in mask vector do not falls within the width: have to test 1 by 1.
			{
				UINT origPixel = pixelsCanvas[index+0];
				pixelsCanvas[index+0] = color & mask.m128_u32[0];
				pixelsCanvas[index+0] |= origPixel & ~mask.m128_u32[0];

				if(col+1<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+1];

					pixelsCanvas[index+1] = color & mask.m128_u32[1];
					pixelsCanvas[index+1] |= origPixel & ~mask.m128_u32[1];
				}

				if(col+2<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+2];

					pixelsCanvas[index+2] = color & mask.m128_u32[2];
					pixelsCanvas[index+2] |= origPixel & ~mask.m128_u32[2];
				}

				if(col+3<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+3];

					pixelsCanvas[index+3] = color & mask.m128_u32[3];
					pixelsCanvas[index+3] |= origPixel & ~mask.m128_u32[3];
				}
			}
		}
	}

	DWORD endTime = timeGetTime();
	char buf[50];
	sprintf(buf, "%d\n", endTime-startTime);
	OutputDebugStringA(buf);
	m_pSrcBitmap->UnlockBits(&bitmapDataCanvas);

	graphics.DrawImage(m_pSrcBitmap,0,0,m_pSrcBitmap->GetWidth(),m_pSrcBitmap->GetHeight());
}
*/

// Draw Circle with AVX intrinsic
void CDrawCircleDlg::OnPaint()
{
	if(!m_pSrcBitmap)
		return;

	using namespace Gdiplus;
	CPaintDC dc(this);
	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	//graphics.DrawImage(m_pSrcBitmap,0,0,m_pSrcBitmap->GetWidth(),m_pSrcBitmap->GetHeight());

	UINT* pixelsCanvas = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataCanvas;
	Rect rect(0, 0, m_pSrcBitmap->GetWidth(), m_pSrcBitmap->GetHeight() );

	m_pSrcBitmap->LockBits(
		&rect,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataCanvas );

	pixelsCanvas = (UINT*)bitmapDataCanvas.Scan0;

	if( !pixelsCanvas )
		return;

	UINT col = 0;
	int stride = bitmapDataCanvas.Stride >> 2;

	float fPointX = 120.0f; // x coordinates of the circle center
	float fPointY = 120.0f; // y coordinates of the circle center
	float fRadius = 110.0f; // radius of the circle center

	// vector of 4 x coordinates of the circle center
	__m256 vecPointX = _mm256_set1_ps(fPointX);
	// vector of 4 y coordinates of the circle center
	__m256 vecPointY = _mm256_set1_ps(fPointY);
	// vector of 4 radius of the circle center
	__m256 vecRadius = _mm256_set1_ps(fRadius);

	float fy=0.0f;
	float fx=0.0f;

	UINT color = 0xff00ff00;
	DWORD startTime = timeGetTime();
	unionUintFloat tmp;
	for(UINT row = 0, fy=0.0f; row < bitmapDataCanvas.Height; row+=1, fy+=1.0f)
	{
		for(col = 0, fx=0.0f; col < bitmapDataCanvas.Width; col+=8, fx+=8.0f)
		{
			// calculate the index of destination pixel array
			int index = row * stride + col;

			// vector of X coordinates of the 4 pixels, it is inverse of of little endian
			__m256 vecX= _mm256_set_ps(fx+7.0f, fx+6.0f, fx+5.0f, fx+4.0f, fx+3.0f, fx+2.0f, fx+1.0f, fx+0.0f);
			// vector of Y coordinates of the 4 pixels
			__m256 vecY = _mm256_set1_ps(fy);

			// Subtract center X from the pixel X coordinates
			vecX = _mm256_sub_ps(vecX, vecPointX);

			// Subtract center Y from the pixel Y coordinates
			vecY = _mm256_sub_ps(vecY, vecPointY);

			// compute the square of X, that is X * X = X to power of 2
			vecX = _mm256_mul_ps(vecX, vecX);
			// compute the square of Y, that is Y * Y = Y to power of 2
			vecY = _mm256_mul_ps(vecY, vecY);

			// Add up the X square and Y square
			__m256 vecHypSq = _mm256_add_ps(vecX, vecY);

			// Find the hypotenuse by computing square root
			__m256 vecHyp = _mm256_sqrt_ps(vecHypSq);

			// Generate the mask for condition of hypotenuse < radius
			__m256 mask = _mm256_cmp_ps(vecHyp, vecRadius, _CMP_LE_OS);

			// all 4 pixel in mask vector, falls within the width
			if(col+7<bitmapDataCanvas.Width)
			{
				UINT origPixel = pixelsCanvas[index+0];
				tmp.f = mask.m256_f32[0];
				pixelsCanvas[index+0] = color & tmp.n;
				pixelsCanvas[index+0] |= origPixel & ~tmp.n;

				origPixel = pixelsCanvas[index+1];
				tmp.f = mask.m256_f32[1];
				pixelsCanvas[index+1] = color & tmp.n;
				pixelsCanvas[index+1] |= origPixel & ~tmp.n;

				origPixel = pixelsCanvas[index+2];
				tmp.f = mask.m256_f32[2];
				pixelsCanvas[index+2] = color & tmp.n;
				pixelsCanvas[index+2] |= origPixel & ~tmp.n;

				origPixel = pixelsCanvas[index+3];
				tmp.f = mask.m256_f32[3];
				pixelsCanvas[index+3] = color & tmp.n;
				pixelsCanvas[index+3] |= origPixel & ~tmp.n;

				origPixel = pixelsCanvas[index+4];
				tmp.f = mask.m256_f32[4];
				pixelsCanvas[index+4] = color & tmp.n;
				pixelsCanvas[index+4] |= origPixel & ~tmp.n;

				origPixel = pixelsCanvas[index+5];
				tmp.f = mask.m256_f32[5];
				pixelsCanvas[index+5] = color & tmp.n;
				pixelsCanvas[index+5] |= origPixel & ~tmp.n;

				origPixel = pixelsCanvas[index+6];
				tmp.f = mask.m256_f32[6];
				pixelsCanvas[index+6] = color & tmp.n;
				pixelsCanvas[index+6] |= origPixel & ~tmp.n;

				origPixel = pixelsCanvas[index+7];
				tmp.f = mask.m256_f32[7];
				pixelsCanvas[index+7] = color & tmp.n;
				pixelsCanvas[index+7] |= origPixel & ~tmp.n;
			}
			else // all 4 pixel in mask vector do not falls within the width: have to test 1 by 1.
			{
				UINT origPixel = pixelsCanvas[index+0];
				tmp.f = mask.m256_f32[0];

				pixelsCanvas[index+0] = color & tmp.n;
				pixelsCanvas[index+0] |= origPixel & ~tmp.n;

				if(col+1<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+1];
					tmp.f = mask.m256_f32[1];

					pixelsCanvas[index+1] = color & tmp.n;
					pixelsCanvas[index+1] |= origPixel & ~tmp.n;
				}

				if(col+2<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+2];
					tmp.f = mask.m256_f32[2];

					pixelsCanvas[index+2] = color & tmp.n;
					pixelsCanvas[index+2] |= origPixel & ~tmp.n;
				}

				if(col+3<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+3];
					tmp.f = mask.m256_f32[3];

					pixelsCanvas[index+3] = color & tmp.n;
					pixelsCanvas[index+3] |= origPixel & ~tmp.n;
				}

				if(col+4<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+4];
					tmp.f = mask.m256_f32[4];

					pixelsCanvas[index+4] = color & tmp.n;
					pixelsCanvas[index+4] |= origPixel & ~tmp.n;
				}

				if(col+5<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+5];
					tmp.f = mask.m256_f32[5];

					pixelsCanvas[index+5] = color & tmp.n;
					pixelsCanvas[index+5] |= origPixel & ~tmp.n;
				}

				if(col+6<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+6];
					tmp.f = mask.m256_f32[6];

					pixelsCanvas[index+6] = color & tmp.n;
					pixelsCanvas[index+6] |= origPixel & ~tmp.n;
				}

				if(col+7<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+7];
					tmp.f = mask.m256_f32[7];

					pixelsCanvas[index+7] = color & tmp.n;
					pixelsCanvas[index+7] |= origPixel & ~tmp.n;
				}
			}
		}
	}

	DWORD endTime = timeGetTime();
	char buf[50];
	sprintf(buf, "%d\n", endTime-startTime);
	OutputDebugStringA(buf);
	m_pSrcBitmap->UnlockBits(&bitmapDataCanvas);

	graphics.DrawImage(m_pSrcBitmap,0,0,m_pSrcBitmap->GetWidth(),m_pSrcBitmap->GetHeight());
}


/*
// Draw Circle with SIMD class
void CDrawCircleDlg::OnPaint()
{
	if(!m_pSrcBitmap)
		return;

	DWORD allStartTime = timeGetTime();

	using namespace Gdiplus;
	CPaintDC dc(this);
	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	//graphics.DrawImage(m_pSrcBitmap,0,0,m_pSrcBitmap->GetWidth(),m_pSrcBitmap->GetHeight());

	UINT* pixelsCanvas = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataCanvas;
	Rect rect(0, 0, m_pSrcBitmap->GetWidth(), m_pSrcBitmap->GetHeight() );

	m_pSrcBitmap->LockBits(
		&rect,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataCanvas );

	pixelsCanvas = (UINT*)bitmapDataCanvas.Scan0;

	if( !pixelsCanvas )
		return;

	UINT col = 0;
	int stride = bitmapDataCanvas.Stride >> 2;

	float fPointX = 50.0f; // x coordinates of the circle center
	float fPointY = 50.0f; // y coordinates of the circle center
	float fRadius = 40.0f; // radius of the circle center

	// vector of 4 x coordinates of the circle center
	F32vec4 vecPointX(fPointX);
	// vector of 4 y coordinates of the circle center
	F32vec4 vecPointY(fPointY);
	// vector of 4 radius of the circle center
	F32vec4 vecRadius(fRadius);

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
				pixelsCanvas[index+0] = color & (__m128(mask)).m128_u32[0];
				pixelsCanvas[index+0] |= origPixel & ~((__m128(mask)).m128_u32[0]);

				origPixel = pixelsCanvas[index+1];
				pixelsCanvas[index+1] = color & (__m128(mask)).m128_u32[1];
				pixelsCanvas[index+1] |= origPixel & ~((__m128(mask)).m128_u32[1]);

				origPixel = pixelsCanvas[index+2];
				pixelsCanvas[index+2] = color & (__m128(mask)).m128_u32[2];
				pixelsCanvas[index+2] |= origPixel & ~((__m128(mask)).m128_u32[2]);

				origPixel = pixelsCanvas[index+3];
				pixelsCanvas[index+3] = color & (__m128(mask)).m128_u32[3];
				pixelsCanvas[index+3] |= origPixel & ~((__m128(mask)).m128_u32[3]);
			}
			else // all 4 pixel in mask vector do not falls within the width: have to test 1 by 1.
			{
				UINT origPixel = pixelsCanvas[index+0];
				pixelsCanvas[index+0] = color & (__m128(mask)).m128_u32[0];
				pixelsCanvas[index+0] |= origPixel & ~((__m128(mask)).m128_u32[0]);

				if(col+1<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+1];

					pixelsCanvas[index+1] = color & (__m128(mask)).m128_u32[1];
					pixelsCanvas[index+1] |= origPixel & ~((__m128(mask)).m128_u32[1]);
				}

				if(col+2<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+2];

					pixelsCanvas[index+2] = color & (__m128(mask)).m128_u32[2];
					pixelsCanvas[index+2] |= origPixel & ~((__m128(mask)).m128_u32[2]);
				}

				if(col+3<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+3];

					pixelsCanvas[index+3] = color & (__m128(mask)).m128_u32[3];
					pixelsCanvas[index+3] |= origPixel & ~((__m128(mask)).m128_u32[3]);
				}
			}
		}
	}

	DWORD endTime = timeGetTime();

	m_pSrcBitmap->UnlockBits(&bitmapDataCanvas);

	DWORD allEndTime = timeGetTime();
	char buf[50];
	sprintf(buf, "All time:%d, Loop time:%d\n", allEndTime-allStartTime, endTime-startTime);
	OutputDebugStringA(buf);

	graphics.DrawImage(m_pSrcBitmap,0,0,m_pSrcBitmap->GetWidth(),m_pSrcBitmap->GetHeight());
}
*/

// Draw Circle with gradient
/*
void CDrawCircleDlg::OnPaint()
{
	if(!m_pSrcBitmap)
		return;

	using namespace Gdiplus;
	CPaintDC dc(this);
	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	UINT* pixelsCanvas = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataCanvas;
	Rect rect(0, 0, m_pSrcBitmap->GetWidth(), m_pSrcBitmap->GetHeight() );

	m_pSrcBitmap->LockBits(
		&rect,
		ImageLockModeWrite,
		PixelFormat32bppARGB,
		&bitmapDataCanvas );

	pixelsCanvas = (UINT*)bitmapDataCanvas.Scan0;

	if( !pixelsCanvas )
		return;

	UINT col = 0;
	int stride = bitmapDataCanvas.Stride >> 2;

	float fPointX = 50.0f; // x coordinates of the circle center
	float fPointY = 50.0f; // Y coordinates of the circle center
	float fRadius = 40.0f; // radius of the circle center

	// vector of 4 x coordinates of the circle center
	F32vec4 vecPointX(fPointX, fPointX, fPointX, fPointX);
	// vector of 4 x coordinates of the circle center
	F32vec4 vecPointY(fPointY, fPointY, fPointY, fPointY);
	// vector of 4 radius of the circle center
	F32vec4 vecRadius(fRadius, fRadius, fRadius, fRadius);

	float fy=0.0f;
	float fx=0.0f;

	UINT color = 0xff0000ff;
	UINT color2 = 0xff00ffff;

	UINT* arrGrad = GenGrad(color, color2, 40);
	
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
			F32vec4 vecY(fy, fy, fy, fy);

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

				pixelsCanvas[index+0] = arrGrad[ (int)((__m128(vecHyp)).m128_f32[0]) ] & (__m128(mask)).m128_u32[0];
				pixelsCanvas[index+0] |= origPixel & ~((__m128(mask)).m128_u32[0]);

				origPixel = pixelsCanvas[index+1];
				pixelsCanvas[index+1] = arrGrad[ (int)((__m128(vecHyp)).m128_f32[1]) ] & (__m128(mask)).m128_u32[1];
				pixelsCanvas[index+1] |= origPixel & ~((__m128(mask)).m128_u32[1]);

				origPixel = pixelsCanvas[index+2];
				pixelsCanvas[index+2] = arrGrad[ (int)((__m128(vecHyp)).m128_f32[2]) ] & (__m128(mask)).m128_u32[2];
				pixelsCanvas[index+2] |= origPixel & ~((__m128(mask)).m128_u32[2]);

				origPixel = pixelsCanvas[index+3];
				pixelsCanvas[index+3] = arrGrad[ (int)((__m128(vecHyp)).m128_f32[3]) ] & (__m128(mask)).m128_u32[3];
				pixelsCanvas[index+3] |= origPixel & ~((__m128(mask)).m128_u32[3]);
			}
			else // all 4 pixel in mask vector do not falls within the width: have to test 1 by 1.
			{
				UINT origPixel = pixelsCanvas[index+0];

				pixelsCanvas[index+0] = arrGrad[ (int)((__m128(vecHyp)).m128_f32[0]) ] & (__m128(mask)).m128_u32[0];
				pixelsCanvas[index+0] |= origPixel & ~((__m128(mask)).m128_u32[0]);

				if(col+1<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+1];

					pixelsCanvas[index+1] = arrGrad[ (int)((__m128(vecHyp)).m128_f32[1]) ] & (__m128(mask)).m128_u32[1];
					pixelsCanvas[index+1] |= origPixel & ~((__m128(mask)).m128_u32[1]);
				}

				if(col+2<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+2];

					pixelsCanvas[index+2] = arrGrad[ (int)((__m128(vecHyp)).m128_f32[2]) ] & (__m128(mask)).m128_u32[2];
					pixelsCanvas[index+2] |= origPixel & ~((__m128(mask)).m128_u32[2]);
				}

				if(col+3<bitmapDataCanvas.Width)
				{
					origPixel = pixelsCanvas[index+3];

					pixelsCanvas[index+3] = arrGrad[ (int)((__m128(vecHyp)).m128_f32[3]) ] & (__m128(mask)).m128_u32[3];
					pixelsCanvas[index+3] |= origPixel & ~((__m128(mask)).m128_u32[3]);
				}
			}
		}
	}

	DWORD endTime = timeGetTime();
	char buf[50];
	sprintf(buf, "%d\n", endTime-startTime);
	OutputDebugStringA(buf);

	delete [] arrGrad;

	m_pSrcBitmap->UnlockBits(&bitmapDataCanvas);

	graphics.DrawImage(m_pSrcBitmap,0,0,m_pSrcBitmap->GetWidth(),m_pSrcBitmap->GetHeight());
}
*/

UINT* CDrawCircleDlg::GenGrad(UINT a, UINT b, UINT len)
{
	int r1=(a&0xff0000)>>16,g1=(a&0xff00)>>8,b1=(a&0xff); //Any start color
	int r2=(b&0xff0000)>>16,g2=(b&0xff00)>>8,b2=(b&0xff); //Any start color

	UINT* arr = new UINT[len+1];

	for(UINT i=0;i<len+1;i++)
	{ 
		int r,g,b;
		r = r1 + (i * (r2-r1) / len);
		g = g1 + (i * (g2-g1) / len);
		b = b1 + (i * (b2-b1) / len);
		
		arr[i] = 0xff000000 | r << 16 | g << 8 | b;
	}

	return arr;
}

float CDrawCircleDlg::ComputeDistance(float fPointX1, float fPointY1, float fPointX2, float fPointY2)
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

int CDrawCircleDlg::MaxDistance(float a, float b, float c, float d)
{
	if(a>=b&&a>=c&&a>=d)
		return 0;
	if(b>=a&&b>=c&&b>=d)
		return 1;
	if(c>=a&&c>=b&&c>=d)
		return 2;
	if(d>=a&&d>=b&&d>=c)
		return 3;

	return -1;
}

void CDrawCircleDlg::LoadSrcImage()
{
	using namespace Gdiplus;
	CRect rect;
	GetClientRect(&rect);
	m_pSrcBitmap = new Bitmap(rect.Width(), rect.Height(), PixelFormat32bppARGB);

	Graphics graph((Image*)(m_pSrcBitmap));

	SolidBrush brush(Color(255,255,255));
	graph.FillRectangle(&brush, 0, 0, rect.Width(), rect.Height());
}

/*
void CDrawCircleDlg::LoadSrcImage()
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
		m_pSrcBitmap = new Bitmap(pStream,false);

		pStream->Release();
	}
}
*/