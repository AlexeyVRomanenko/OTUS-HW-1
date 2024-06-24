#include "pch.h"
#include "shapes.h"
#include "point.h"
#include "line.h"
#include "circle.h"
#include "rectangle.h"

ShapesPtr Shapes::Instance()
{
	return ShapesPtr{ new Shapes };
}

editor::IPointPtr Shapes::CreatePoint(const editor::point2d& center)
{
	auto obj = Point::Instance(center);
	m_shapes.insert({ obj->GetTag(), obj });
	return obj;
}

editor::ILinePtr Shapes::CreateLine(const editor::point2d& p1, const editor::point2d& p2)
{
	auto obj = Line::Instance(p1, p2);
	m_shapes.insert({ obj->GetTag(), obj });
	return obj;
}

editor::ICirclePtr Shapes::CreateCircle(const editor::point2d& center, double radius)
{
	auto obj = Circle::Instance(center, radius);
	m_shapes.insert({ obj->GetTag(), obj });
	return obj;
}

editor::IRectanglePtr Shapes::CreateRectangle(const editor::point2d& p1, const editor::point2d& p2)
{
	auto obj = Rectangle::Instance(p1, p2);
	m_shapes.insert({ obj->GetTag(), obj });
	return obj;
}

bool Shapes::Delete(const editor::shape_tag tag)
{
	return m_shapes.erase(tag);
}

size_t Shapes::GetCount() const
{
	return m_shapes.size();
}