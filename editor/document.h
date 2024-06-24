#pragma once
#include <editor/IDocument.h>

class Document :
	public editor::IDocument
{
public:
	static DocumentPtr Instance();

	//IDocument
	virtual bool Save() const override;
	virtual bool SaveAs(const fs::path& file) const override;
	virtual editor::IShapes* GetShapes() const override;

	bool LoadFromFile(const fs::path&);

private:
	Document() = default;

	mutable fs::path m_file;
	mutable ShapesPtr m_shapes;
};