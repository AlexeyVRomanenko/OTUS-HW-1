#pragma once
#include <editor/shapes/IRectangle.h>
#include "shape.h"

class Rectangle :
	public editor::IRectangle,
	public Shape
{
public:
	static RectanglePtr Instance(const editor::point2d& p1, const editor::point2d& p2);

private:
	Rectangle(const editor::point2d& p1, const editor::point2d& p2);

	const editor::point2d m_p1;
	const editor::point2d m_p2;
};