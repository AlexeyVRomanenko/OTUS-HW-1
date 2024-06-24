#include <iostream>

#include <editor/editor.h>
#include <editor/IApp.h>
#include <editor/IDocument.h>
#include <editor/shapes/IShapes.h>
#include <editor/shapes/IPoint.h>
#include <editor/shapes/ILine.h>
#include <editor/shapes/ICircle.h>
#include <editor/shapes/IRectangle.h>

/**
 * \brief MY DOXY Doc
 * OTUS HW-05
 */

int main(int, char**)
{
	editor::IAppPtr app = editor::CreateApp();

	editor::IDocumentPtr doc = app->CreateNewDocument();
	doc->SaveAs(fs::temp_directory_path() / L"tmp_doc.tmp");

	editor::IShapes* shapes = doc->GetShapes();
	editor::IPointPtr point = shapes->CreatePoint({ 0, 0 });
	editor::ILinePtr line = shapes->CreateLine({ 0, 0 }, { 1, 1 });
	editor::ICirclePtr circle = shapes->CreateCircle({ 0, 0 }, 0.5);
	editor::IRectanglePtr rect = shapes->CreateRectangle({ 1.5, 1.6 }, { 2.7, 2.8 });
	shapes->Delete(point->GetTag());
	shapes->Delete(circle->GetTag());

	doc->Save();

	return 0;
}