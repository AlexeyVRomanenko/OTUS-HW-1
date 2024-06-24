#include "pch.h"
#include "document.h"
#include "shapes.h"

DocumentPtr Document::Instance()
{
	return DocumentPtr{ new Document() };
}

bool Document::Save() const
{
	if (m_file.empty())
		return false;

	return true;
}

bool Document::SaveAs(const fs::path& file) const
{
	m_file = file;
	return true;
}

bool Document::LoadFromFile(const fs::path& file)
{
	m_file = file;
	return true;
}

editor::IShapes* Document::GetShapes() const
{
	if (!m_shapes)
		m_shapes = Shapes::Instance();

	return m_shapes.get();
}