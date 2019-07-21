#include "stdafx.h"
#include "MathVector.h"
#include <cmath>

int CMathVector::m_cnt = 0;

CMathVector::CMathVector(double X, double Y, double Limit) : x(X), y(Y), m_limit(Limit)
{
	m_cnt++;
}

CMathVector::~CMathVector()
{
	m_cnt--;
}

CMathVector CMathVector::operator+(const CMathVector& T) const
{	
	return CMathVector(x + T.x, y + T.y);
}

CMathVector CMathVector::operator-(const CMathVector& T) const
{	
	return CMathVector(x - T.x, y - T.y);
}

CMathVector CMathVector::operator*(const CMathVector& T) const
{
	return CMathVector(x * T.x, y * T.y);
}

CMathVector CMathVector::operator/(const CMathVector& T) const
{
	return CMathVector(x / T.x, y / T.y);
}

CMathVector CMathVector::operator*(double f) const
{	
	return CMathVector(x * f, y * f);
}

CMathVector CMathVector::operator/(double f) const
{	
	return CMathVector(x / f, y / f);
}

CMathVector CMathVector::operator+=(const CMathVector& T)
{		
	return CMathVector(x += T.x, y += T.y);
}

CMathVector CMathVector::operator-=(const CMathVector& T)
{	
	return CMathVector(x -= T.x, y -= T.y);
}

CMathVector CMathVector::operator*=(const CMathVector& T)
{
	return CMathVector(x *= T.x, y *= T.y);
}

CMathVector CMathVector::operator/=(const CMathVector& T)
{
	return CMathVector(x /= T.x, y /= T.y);
}

CMathVector CMathVector::operator*=(double f)
{	
	return CMathVector(x *= f, y *= f);
}

CMathVector CMathVector::operator/=(double f)
{	
	return CMathVector(x /= f, y /= f);
}

CMathVector CMathVector::add(CMathVector T1,  CMathVector T2)
{	
	return T1 + T2;
}

CMathVector CMathVector::sub(CMathVector T1, CMathVector T2) 
{
	return T1 - T2;
}

CMathVector CMathVector::mult(CMathVector T1, CMathVector T2)
{
	return T1 * T2;
}

CMathVector CMathVector::div(CMathVector T1, CMathVector T2)
{
	return T1 / T2;
}

double CMathVector::DotVector(const CMathVector& T) const
{
	return (x * T.x) + (y * T.y);
}

/*
CMathVector CMathVector::CrossVector(const CMathVector& T) const
{
	
}
*/

double CMathVector::AngleVector() const
{
	// -y 윈도우 좌표계는 y가 꺼꾸로
	double theta = atan2(-y, x);	
	double deg = theta * 180 / pi;

	if (deg < 0)
		deg += 360;
	
	return deg;
}

double CMathVector::AngleBetweenVector(const CMathVector& T) const
{
	CMathVector v1(x, y, m_limit);
	v1.Normalize();
	CMathVector v2 = T;
	v2.Normalize();

	double theta = v1.DotVector(v2);
	theta = acos(theta);
	double deg = theta * 180.0 / pi;
	return deg;

	//return acos(DotVector(T) / GetMagnitude() * T.GetMagnitude())*180.0 / pi;

}

double CMathVector::GetMagnitude() const
{
	return sqrt(x*x + y*y);
}

void CMathVector::Normalize()
{
	// 벡터 정규화 (방향 유지, 크기 1로)
	// 피타고라스 정의, 벡터 빗변(크기)구하기 
	double mag = sqrt(x*x + y*y);

	if (mag > 0)
	{
		x /= mag;
		y /= mag;
	}	
}

void CMathVector::Limit(double limit)
{
	// copysign(x, y)
	// y의 부호를 x의 부호로 사용
	// double c = copysign(3.14, -3.2)
	// c == -3.14

	m_limit = limit;

	if (abs(x) > m_limit)
		x = copysign(limit, x);

	if (abs(y) > m_limit)
		y = copysign(limit, y);	
}
