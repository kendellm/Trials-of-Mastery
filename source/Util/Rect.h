/*
	File:		Rect.h
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1307
	Purpose:	Rect struct contain information for a rectangle
*/

#ifndef RECT_H
#define RECT_H

#define WIND32_LEAN_AND_MEAN
#include <Windows.h>

struct Rect
{
	Rect(void);
	Rect(int left, int top, int right, int bottom);
//	Rect(int left, int top, int width, int height);

	~Rect(void);

	int left;
	int top;
	int right;
	int bottom;
	int width;
	int height;

	bool IsActive();
	RECT GetRECT();
};

#endif