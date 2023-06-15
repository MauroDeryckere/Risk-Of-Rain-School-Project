#include "base.h"
#include "structs.h"

//-----------------------------------------------------------------
// Window Constructors
//-----------------------------------------------------------------
Window::Window( const std::string& title , float width , float height , bool isVSyncOn )
	:title{ title }
	,width{ width }
	,height{ height }
	,isVSyncOn{ isVSyncOn }
{
}

//-----------------------------------------------------------------
// Point2f Constructors
//-----------------------------------------------------------------
Point2f::Point2f( )
	:Point2f{ 0.0f, 0.0f }
{
}
Point2f::Point2f( float x, float y )
	:x{ x }, y{ y }
{
}

//-----------------------------------------------------------------
// Rectf Constructors
//-----------------------------------------------------------------
Rectf::Rectf( )
	:Rectf{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

Rectf::Rectf( float left, float bottom, float width, float height )
	:left{ left }
	,bottom{ bottom }
	,width{ width }
	,height{ height }
{
}

//-----------------------------------------------------------------
// Color4f Constructors
//-----------------------------------------------------------------
Color4f::Color4f( )
	:Color4f{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

Color4f::Color4f( float r, float g, float b, float a )
	:r{ r }
	,g{ g }
	,b{ b }
	,a{ a }
{
}

//-----------------------------------------------------------------
// Circlef Constructors
//-----------------------------------------------------------------
Circlef::Circlef( )
	:Circlef{ 0.0f, 0.0f, 0.0f }
{
}

Circlef::Circlef( float centerX, float centerY, float radius )
	:Circlef{ Point2f{ centerX, centerY }, radius }
{
}

Circlef::Circlef( const Point2f& center, float radius )
	:center{ center }
	,radius{ radius }
{
}

//-----------------------------------------------------------------
// Ellipsef Constructors
//-----------------------------------------------------------------
Ellipsef::Ellipsef( )
	:Ellipsef{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}


Ellipsef::Ellipsef( const Point2f& center, float radiusX, float radiusY )
	: center{ center }
	, radiusX{ radiusX }
	, radiusY{ radiusY }
{
}

Ellipsef::Ellipsef( float centerX, float centerY, float radiusX, float radiusY )
	: Ellipsef{ Point2f{ centerX, centerY }, radiusX, radiusY }
{
}

//-----------------------------------------------------------------
// Bezier Constructors
//-----------------------------------------------------------------
LinearBezier::LinearBezier():
	LinearBezier{ Point2f{0.f,0.f}, Point2f{0.f,0.f} }
{
}

LinearBezier::LinearBezier(const Point2f& p0, const Point2f& p1):
	p0{p0},
	p1{p1}
{
}

QuadraticBezier::QuadraticBezier():
	QuadraticBezier{ Point2f{0.f,0.f}, Point2f{0.f,0.f}, Point2f{0.f,0.f} }
{
}

QuadraticBezier::QuadraticBezier(const Point2f& p0, const Point2f& p1, const Point2f& p2) :
	p0{ p0 },
	p1{ p1 },
	p2{ p2 }
{
}

CubicBezier::CubicBezier():
	CubicBezier{ Point2f{0.f,0.f}, Point2f{0.f,0.f}, Point2f{0.f,0.f}, Point2f{0.f,0.f} }
{
}

CubicBezier::CubicBezier(const Point2f& p0, const Point2f& p1, const Point2f& p2, const Point2f& p3):
	p0{ p0 },
	p1{ p1 },
	p2{ p2 },
	p3{ p3 }
{
}
