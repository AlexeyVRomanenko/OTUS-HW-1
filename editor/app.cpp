#include "pch.h"
#include "app.h"
#include "document.h"

AppPtr App::Instance()
{
	return AppPtr{ new App() };
}

editor::IDocumentPtr App::CreateNewDocument()
{
	DocumentPtr doc = Document::Instance();
	m_documents.push_back(doc);
	return doc;
}

editor::IDocumentPtr App::ImportDocument(const fs::path& file)
{
	DocumentPtr doc = Document::Instance();
	if (!doc->LoadFromFile(file))
		return {};

	m_documents.push_back(doc);

	return doc;
}

size_t App::GetDocumentsCount() const
{
	return m_documents.size();
}