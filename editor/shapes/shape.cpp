#include "pch.h"
#include "shape.h"

Shape::Shape(editor::shape_tag tag) :
	m_tag(tag)
{
}

editor::shape_tag Shape::GetTag() const
{
	return m_tag;
}

editor::shape_tag Shape::CreateNewTag()
{
	editor::shape_tag tag = {};
	return ++tag;
}