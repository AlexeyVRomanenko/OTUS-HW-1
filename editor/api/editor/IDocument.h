#pragma once
#include <editor/IObj.h>

namespace editor
{
	class IDocument :
		protected IObj
	{
	public:
		virtual bool Save() const = 0;
		virtual bool SaveAs(const std::filesystem::path& file) const = 0;

		virtual IShapes* GetShapes() const = 0;
	};
}