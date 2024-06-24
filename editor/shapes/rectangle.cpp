#include "pch.h"
#include "rectangle.h"

RectanglePtr Rectangle::Instance(const editor::point2d& p1, const editor::point2d& p2)
{
	return RectanglePtr{ new Rectangle(p1, p1) };
}

Rectangle::Rectangle(const editor::point2d& p1, const editor::point2d& p2) :
	Shape(Shape::CreateNewTag()),
	m_p1(p1),
	m_p2(p2)
{
}