#pragma once
#include <editor/shapes/ILine.h>
#include "shape.h"

class Line :
	public editor::ILine,
	public Shape
{
public:
	static LinePtr Instance(const editor::point2d& p1, const editor::point2d& p2);

private:
	Line(const editor::point2d& p1, const editor::point2d& p2);

	const editor::point2d m_p1;
	const editor::point2d m_p2;
};