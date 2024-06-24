#pragma once
#include <editor/IObj.h>

namespace editor
{
	using shape_tag = unsigned int;

	class IShape :
		public IObj
	{
	public:
		virtual shape_tag GetTag() const = 0;
	};
}