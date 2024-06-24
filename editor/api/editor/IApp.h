#pragma once
#include <editor/IObj.h>

namespace editor
{
	class IApp :
		protected IObj
	{
	public:
		virtual IDocumentPtr CreateNewDocument() = 0;
		virtual IDocumentPtr ImportDocument(const std::filesystem::path& file) = 0;
	};
}