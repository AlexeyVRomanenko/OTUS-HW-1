#include "pch.h"
#include "point.h"

PointPtr Point::Instance(const editor::point2d& center)
{
	return PointPtr{ new Point(center) };
}

Point::Point(const editor::point2d& center) :
	Shape(Shape::CreateNewTag()),
	m_center(center)
{
}