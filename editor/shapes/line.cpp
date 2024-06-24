#include "pch.h"
#include "line.h"

LinePtr Line::Instance(const editor::point2d& p1, const editor::point2d& p2)
{
	return LinePtr{ new Line(p1, p1) };
}

Line::Line(const editor::point2d& p1, const editor::point2d& p2) :
	Shape(Shape::CreateNewTag()),
	m_p1(p1),
	m_p2(p2)
{
}