#ifndef TU_CLIENT_GUI_RECT_H
#define TU_CLIENT_GUI_RECT_H

#include <base/vmath.h>

namespace tu
{
	
namespace gui
{

class CRect
{
public:
	union { int x,minx,minw,x0; };
	union { int y,miny,minh,y0; };
	union { int w,maxx,maxw,x1; };
	union { int h,maxy,maxh,y1; };
	
public:
	CRect();
	CRect(int X, int Y, int W, int H);
	
	void CenterIn(const CRect& Rect);
	bool IsInside(int X, int Y) const;
	
	void RemoveMargin(int Padding);
	void AddMargin(int Padding);
	
	void BSNoConstraint();
	void BSMinimum(int W, int H);
	void BSFixed(int W, int H);
	void BSInfiniteWidth();
	void BSInfiniteHeight();
	void BSAddSpacing(int X, int Y);
	void BSAddMargin(int M);
	void BSHorizontalAdd(const CRect& Rect);
	void BSVerticalAdd(const CRect& Rect);
	void BSInnerAdd(const CRect& Rect);
	
	void DRUpdatePosition(const CRect& BoundingRect, const CRect BoundingSize);
};

}

}

#endif
