#include <base/math.h>
#include "rect.h"

namespace tu
{
	
namespace gui
{

CRect::CRect() :
	x(0),
	y(0),
	w(0),
	h(0)
{
	
}

CRect::CRect(int X, int Y, int W, int H) :
	x(X),
	y(Y),
	w(W),
	h(H)
{
	
}

void CRect::CenterIn(const CRect& Rect)
{
	x = Rect.x + Rect.w/2 - w/2;
	y = Rect.y + Rect.h/2 - h/2;
}

bool CRect::IsInside(int X, int Y) const
{
	return (X >= x && X <= x + w && Y >= y && Y <= y + h);
}

void CRect::RemoveMargin(int Margin)
{
	x += Margin;
	y += Margin;
	w -= 2*Margin;
	h -= 2*Margin;
}

void CRect::BSMinimum(int W, int H)
{
	minw = W;
	minh = H;
	BSInfiniteWidth();
	BSInfiniteHeight();
}

void CRect::BSNoConstraint()
{
	BSMinimum(0,0);
}

void CRect::BSFixed(int W, int H)
{
	minw = W;
	minh = H;
	maxw = W;
	maxh = H;
}

void CRect::BSInfiniteWidth()
{
	maxw = -1;
}

void CRect::BSInfiniteHeight()
{
	maxh = -1;
}

void CRect::BSAddSpacing(int X, int Y)
{
	minw += X;
	minh += Y;
	if(maxw != -1)
		maxw += X;
	if(maxh != -1)
		maxh += Y;
}

void CRect::BSAddMargin(int M)
{
	minw += 2*M;
	minh += 2*M;
	if(maxw != -1)
		maxw += 2*M;
	if(maxh != -1)
		maxh += 2*M;
}

void CRect::BSHorizontalAdd(const CRect& Rect)
{
	minw += Rect.minw;
	minh = max(minh, Rect.minh);
	
	if(maxw == -1 || Rect.maxw == -1)
		maxw = -1;
	else
		maxw += Rect.maxw;	
	
	if(maxh == -1 || Rect.maxh == -1)
		maxh = -1;
	else
		maxh = max(maxh, Rect.maxh);
}

void CRect::BSVerticalAdd(const CRect& Rect)
{
	minw = max(minw, Rect.minw);
	minh += Rect.minh;
	
	if(maxw == -1 || Rect.maxw == -1)
		maxw = -1;
	else
		maxw = max(maxw, Rect.maxw);
	
	if(maxh == -1 || Rect.maxh == -1)
		maxh = -1;
	else
		maxh += Rect.maxh;
}

void CRect::DRUpdatePosition(const CRect& BoundingRect, const CRect BoundingSize)
{
	x = BoundingRect.x;
	y = BoundingRect.y;
	
	if(BoundingRect.w < BoundingSize.minw)
		w = BoundingSize.minw;
	else if(BoundingSize.maxw < 0)
		w = BoundingRect.w;
	else
		w = min(BoundingRect.w, BoundingSize.maxw);
	
	if(BoundingRect.h < BoundingSize.minh)
		h = BoundingSize.minh;
	else if(BoundingSize.maxh < 0)
		h = BoundingRect.h;
	else
		h = min(BoundingRect.h, BoundingSize.maxh);
}
}

}
