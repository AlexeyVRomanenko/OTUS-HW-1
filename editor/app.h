#pragma once
#include <editor/IApp.h>

class App :
	public editor::IApp
{
public:
	static AppPtr Instance();

	//IApp
	virtual editor::IDocumentPtr CreateNewDocument() override;
	virtual editor::IDocumentPtr ImportDocument(const fs::path& file) override;

private:
	App() = default;

	std::list<DocumentPtr> m_documents;
};