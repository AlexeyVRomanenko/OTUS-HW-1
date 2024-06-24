#pragma once
#include <editor/shapes/ICircle.h>
#include "shape.h"

class Circle :
	public editor::ICircle,
	public Shape
{
public:
	static CirclePtr Instance(const editor::point2d& center, double radius);

private:
	Circle(const editor::point2d& center, double radius);

	const editor::point2d m_center;
	const double m_radius = 0;
};