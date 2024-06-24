#pragma once
#include <editor/shapes/IPoint.h>
#include "shape.h"

class Point :
	public editor::IPoint,
	public Shape
{
public:
	static PointPtr Instance(const editor::point2d& center);

private:
	Point(const editor::point2d& center);

	const editor::point2d m_center;
};