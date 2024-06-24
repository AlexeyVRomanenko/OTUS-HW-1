#pragma once
#include <editor/shapes/IShapes.h>
#include "shape.h"

class Shapes :
	public editor::IShapes
{
public:
	static ShapesPtr Instance();

	//IShapes
	virtual editor::IPointPtr CreatePoint(const editor::point2d& center) override;
	virtual editor::ILinePtr CreateLine(const editor::point2d& p1, const editor::point2d& p2) override;
	virtual editor::ICirclePtr CreateCircle(const editor::point2d& center, double radius) override;
	virtual editor::IRectanglePtr CreateRectangle(const editor::point2d& p1, const editor::point2d& p2) override;
	virtual bool Delete(const editor::shape_tag) override;
	virtual size_t GetCount() const override;

private:
	Shapes() = default;

	using SHAPES = std::unordered_map<editor::shape_tag, ShapePtr>;
	SHAPES m_shapes;
};