#pragma once
#include "Ship.h"
static const double pi = 3.141592;

class CMathVector
{
public:
	CMathVector(double X = 0, double Y = 0, double Limit = 9999);
	~CMathVector();

public:
	double x, y;
private:
	double m_limit;
	static int m_cnt;

public:
	CMathVector operator+(const CMathVector& T) const;
	CMathVector operator-(const CMathVector& T) const;
	CMathVector operator*(const CMathVector& T) const;
	CMathVector operator/(const CMathVector& T) const;
	CMathVector operator*(double f) const;
	CMathVector operator/(double f) const;

	CMathVector operator+=(const CMathVector& T);
	CMathVector operator-=(const CMathVector& T);
	CMathVector operator*=(const CMathVector& T);
	CMathVector operator/=(const CMathVector& T);
	CMathVector operator*=(double f);
	CMathVector operator/=(double f);

	static CMathVector add(CMathVector T1, CMathVector T2);
	static CMathVector sub(CMathVector T1, CMathVector T2);
	static CMathVector mult(CMathVector T1, CMathVector T2);
	static CMathVector div(CMathVector T1, CMathVector T2);

	double DotVector(const CMathVector& T) const;
	CMathVector CrossVector(const CMathVector& T) const;
	double AngleVector() const;
	double AngleBetweenVector(const CMathVector& T) const;

	double GetMagnitude() const;
	void Normalize();
	void Limit(double limit);

	inline static int GetCount() { return m_cnt; }

};

