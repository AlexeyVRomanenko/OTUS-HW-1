#include <iostream>

#include <editor/editor.h>
#include <editor/IApp.h>
#include <editor/IDocument.h>

/**
 * \brief MY DOXY Doc
 * OTUS HW-05
 */

int main(int, char**)
{
	editor::IAppPtr app = editor::CreateApp();

	editor::IDocumentPtr doc = app->CreateNewDocument();
	doc->SaveAs(fs::temp_directory_path() / L"tmp_doc.tmp");

	doc->GetShapes()


	return 0;
}