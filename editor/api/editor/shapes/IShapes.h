#pragma once
#include <editor/IObj.h>
#include <editor/shapes/IShape.h>

namespace editor
{
	class IShapes :
		public IObj
	{
	public:
		virtual IPointPtr CreatePoint(const point2d& center) = 0;
		virtual ILinePtr CreateLine(const point2d& p1, const point2d& p2) = 0;
		virtual ICirclePtr CreateCircle(const point2d& center, double radius) = 0;
		virtual IRectanglePtr CreateRectangle(const point2d& p1, const point2d& p2) = 0;

		virtual bool Delete(const shape_tag) = 0;
		virtual size_t GetCount() const = 0;
	};
}