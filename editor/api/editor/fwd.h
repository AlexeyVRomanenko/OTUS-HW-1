#pragma once

namespace editor
{
	using IAppPtr = std::shared_ptr<class IApp>;
	using IDocumentPtr = std::shared_ptr<class IDocument>;

	class IShapes;
	using ICirclePtr = std::shared_ptr<class ICircle>;
	using ILinePtr = std::shared_ptr<class ILine>;
	using IPointPtr = std::shared_ptr<class IPoint>;
	using IRectanglePtr = std::shared_ptr<class IRectangle>;

	struct point2d;
}