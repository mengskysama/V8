#include "stdafx.h"
#include "Basic.h"
#include <wingdi.h>

#pragma comment(lib,"msimg32.lib")

namespace FFTWav
{
	/************************************************************************/
	/* CSystem                                                              */
	/************************************************************************/
	inline void set_low(jlong* value, jint low)
	{
		*value &= (jlong)0xffffffff << 32;
		*value |= (jlong)(julong)(juint)low;
	}

	inline void set_high(jlong* value, jint high)
	{
		*value &= (jlong)(julong)(juint)0xffffffff;
		*value |= (jlong)high << 32;
	}

	jlong as_long(LARGE_INTEGER x) {
		jlong result = 0; 
		set_high(&result, x.HighPart);
		set_low(&result,  x.LowPart);
		return result;
	}

	LARGE_INTEGER liFrequency = {0};
	BOOL gSupportPerformanceFrequency = QueryPerformanceFrequency(&liFrequency);
	jlong CSystem::frequency = as_long(liFrequency);
	int CSystem::ready = 1;

/////////////////////////////////////////////////////////////////////////////////////////////
	BOOL DrawGradient(HDC hdc, CONST RECT* pRect, CONST DWORD* cl, int Num, DWORD dwMode)
	{
		int Width;
		int Height;
		TRIVERTEX *pvert;
		GRADIENT_RECT    *pgRect;

		if (cl == NULL || Num < 1 || pRect == NULL || dwMode == GRADIENT_FILL_TRIANGLE)
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			return TRUE;
		}

		if (Num == 1)
		{
			HBRUSH hBrush = CreateSolidBrush(cl[0]);
			SelectObject(hdc, hBrush);
			FillRect(hdc, pRect, hBrush);
			DeleteObject(hBrush);
			return TRUE;
		}

		pvert = new TRIVERTEX[Num * 2 - 2];
		pgRect = new GRADIENT_RECT[Num];

		Width = pRect->right - pRect->left;
		Height = pRect->bottom - pRect->top;
		for (int i = 0; i < Num - 1; i++)
		{
			if (dwMode == GRADIENT_FILL_RECT_V)
			{
				pvert[i * 2].x = pRect->left;
				pvert[i * 2].y = pRect->top + Height / (Num - 1) * i;

				pvert[i * 2 + 1].x = pRect->right;
				pvert[i * 2 + 1].y = pRect->top + Height / (Num - 1) * (i + 1);
			}
			else if (dwMode == GRADIENT_FILL_RECT_H)
			{
				pvert[i * 2].x = pRect->left + Width / (Num - 1) * i;
				pvert[i * 2].y = pRect->top;

				pvert[i * 2 + 1].x = pRect->left + Width / (Num - 1) * (i + 1);
				pvert[i * 2 + 1].y = pRect->bottom;
			}

			pvert[i * 2] .Red    = (WORD)GetRValue((cl[i])) << 8;
			pvert[i * 2] .Green  = (WORD)GetGValue((cl[i])) << 8;
			pvert[i * 2] .Blue   = (WORD)GetBValue((cl[i])) << 8;
			pvert[i * 2] .Alpha  = 0x0000;

			pvert[i * 2 + 1] .Red    = (WORD)GetRValue((cl[i + 1])) << 8;
			pvert[i * 2 + 1] .Green  = (WORD)GetGValue((cl[i + 1])) << 8;
			pvert[i * 2 + 1] .Blue   = (WORD)GetBValue((cl[i + 1])) << 8;
			pvert[i * 2 + 1] .Alpha  = 0x0000;

			pgRect[i].UpperLeft  = i * 2;
			pgRect[i].LowerRight = i * 2 + 1;
		}

		BOOL bRet = ::GradientFill(hdc, pvert, Num * 2, pgRect, Num - 1, dwMode);

		delete []pvert;
		delete []pgRect;

		return bRet;
	}
}