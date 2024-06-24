#pragma once
#include <editor/shapes/IShape.h>

class Shape :
	public virtual editor::IShape
{
public:
	virtual editor::shape_tag GetTag() const override;

protected:
	Shape(editor::shape_tag);

	static editor::shape_tag CreateNewTag();

private:
	const editor::shape_tag m_tag;
};