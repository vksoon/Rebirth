#pragma once

#include "Prerequisites.h"

RB_NAMESPACE_BEGIN

class RArea
{
public:
	RArea() = default;

	RArea(const Vec2i& UL, const Vec2i& LR)
	{
		Set(UL.x, UL.y, LR.x, LR.y);
	}

	RArea(int32_t X1, int32_t Y1, int32_t X2, int32_t Y2)
	{
		Set(X1, Y1, X2, Y2);
	}

	void Set(int32_t X1, int32_t Y1, int32_t X2, int32_t Y2)
	{
		if (X1 <= X2)
		{
			x1 = X1;
			x2 = X2;
		}
		else
		{
			x1 = X2;
			x2 = X1;
		}

		if (Y1 <= Y2)
		{
			y1 = Y1;
			y2 = Y2;
		}
		else
		{
			y1 = Y2;
			y2 = Y1;
		}
	}

	void Offset(const Vec2i& o)
	{
		x1 += o.x;
		x2 += o.x;
		y1 += o.y;
		y2 += o.y;
	}

	RArea GetOffset(const Vec2i& offset) const
	{
		return RArea(x1 + offset.x, y1 + offset.y, x2 + offset.x, y2 + offset.y);
	}

	int32		GetWidth() const { return x2 - x1; }
	int32		GetHeight() const { return y2 - y1; }
	Vec2i		GetSize() const { return Vec2i(x2 - x1, y2 - y1); }
	Vec2		GetCenter() const { return Vec2((x1 + x2) / 2.0f, (y1 + y2) / 2.0f); }
	int32		CalcArea() const { return GetWidth() * GetHeight(); }

	int32		GetX1() const { return x1; }
	void		SetX1(int32 aX1) { x1 = aX1; }
	int32		GetY1() const { return y1; }
	void		SetY1(int32 aY1) { y1 = aY1; }
	int32		GetX2() const { return x2; }
	void		SetX2(int32 aX2) { x2 = aX2; }
	int32		GetY2() const { return y2; }
	void		SetY2(int32 aY2) { y2 = aY2; }
	Vec2i		GetUL() const { return Vec2i(x1, y1); }
	Vec2i		GetLR() const { return Vec2i(x2, y2); }

	void Include(const Vec2i& point)
	{
		if (x1 > point.x) x1 = point.x;
		if (x2 < point.x) x2 = point.x;
		if (y1 > point.y) y1 = point.y;
		if (y2 < point.y) y2 = point.y;
	}

	void Include(const Array<Vec2i>& points)
	{
		for (size_t s = 0; s < points.size(); ++s)
			Include(points[s]);
	}

	void Include(const RArea& area)
	{
		Include(Vec2i(area.x1, area.y1));
		Include(Vec2i(area.x2, area.y2));
	}

	bool Contains(const Vec2i& offset) const
	{
		return ((offset.x >= x1) && (offset.x < x2) && (offset.y >= y1) && (offset.y < y2));
	}

	bool			operator==(const RArea& rhs) const { return ((x1 == rhs.x1) && (y1 == rhs.y1) && (x2 == rhs.x2) && (y2 == rhs.y2)); }
	bool			operator!=(const RArea& rhs) const { return !(*this == rhs); }

	const RArea		operator+(const Vec2i& o) const { return this->GetOffset(o); }
	const RArea		operator-(const Vec2i& o) const { return this->GetOffset(Vec2i(-o.x, -o.y)); }

	const RArea		operator+(const RArea& rhs) const { return RArea(x1 + rhs.x1, y1 + rhs.y1, x2 + rhs.x2, y2 + rhs.y2); }
	const RArea		operator-(const RArea& rhs) const { return RArea(x1 - rhs.x1, y1 - rhs.y1, x2 - rhs.x2, y2 - rhs.y2); }

	RArea& operator+=(const Vec2i& o) { Offset(o); return *this; }
	RArea& operator-=(const Vec2i& o) { Offset(Vec2i(-o.x, -o.y)); return *this; }

	static RArea Zero() { return RArea(0, 0, 0, 0); }

	int32 x1, y1, x2, y2;
};

RB_NAMESPACE_END