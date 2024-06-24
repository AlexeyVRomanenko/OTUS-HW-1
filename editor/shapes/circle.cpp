#include "pch.h"
#include "circle.h"

CirclePtr Circle::Instance(const editor::point2d& center, double radius)
{
	return CirclePtr{ new Circle(center, radius) };
}

Circle::Circle(const editor::point2d& center, double radius) :
	Shape(Shape::CreateNewTag()),
	m_center(center),
	m_radius(radius)
{
}