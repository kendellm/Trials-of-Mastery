/*
File:		Rect.cpp
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	Rect struct contain information for a rectangle
*/


#include "Rect.h"


Rect::Rect(void)
{
	left		= -1;
	top			= -1;
	right		= -1;
	bottom		= -1;

	width		= -1;
	height		= -1;
}


Rect::Rect(int nLeft, int nTop, int nRight, int nBottom)
{
	left = nLeft;
	top = nTop;
	right = nRight;
	bottom = nBottom;

	width = nRight - nLeft;
	height = nBottom - nTop;
}

//Rect::Rect(int nLeft, int nTop, int nWidth, int nHeight)
//{
//	left = nLeft;
//	top = nTop;
//	right = nLeft + nWidth;
//	bottom = nTop + nHeight;
//
//	width = nWidth;
//	height = nHeight;
//}



Rect::~Rect(void)
{
}

bool Rect::IsActive()
{
	if (left == -1 && top == -1 && right == -1 && bottom == -1)
		return false;
	else
		return true;
}

RECT Rect::GetRECT()
{
	 RECT r = { left, top, right, bottom }; 
	 return r;
}



