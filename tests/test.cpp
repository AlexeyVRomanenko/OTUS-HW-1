#define BOOST_TEST_MODULE test
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include <editor/editor.h>
#include <editor/IApp.h>
#include <editor/IDocument.h>
#include <editor/shapes/IShapes.h>
#include <editor/shapes/IPoint.h>
#include <editor/shapes/ILine.h>
#include <editor/shapes/ICircle.h>
#include <editor/shapes/IRectangle.h>

///////////////////////////////////////////////////////////

struct cout_redirect
{
	cout_redirect(std::streambuf* new_buffer)
		: old(std::cout.rdbuf(new_buffer))
	{
	}

	~cout_redirect()
	{
		std::cout.rdbuf(old);
	}

private:
	std::streambuf* old = nullptr;
};

void BOOST_CHECK_COUT(const std::function<void()>& fn, const char* ref_str)
{
	boost::test_tools::output_test_stream output;
	{
		cout_redirect guard(output.rdbuf());
		fn();
	}
	BOOST_CHECK(output.is_equal(ref_str));
}

///////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(Test_1)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);

	editor::IAppPtr app = editor::CreateApp();
	BOOST_CHECK(app);

	editor::IDocumentPtr doc = app->CreateNewDocument();
	BOOST_CHECK(doc);
	BOOST_CHECK(doc->SaveAs(fs::temp_directory_path() / L"tmp_doc.tmp"));
	BOOST_CHECK(app->GetDocumentsCount() == 1);

	editor::IShapes* shapes = doc->GetShapes();
	BOOST_CHECK(shapes);
	editor::IPointPtr point = shapes->CreatePoint({ 3, 4 });
	BOOST_CHECK(point);
	editor::ILinePtr line = shapes->CreateLine({ 0, 0 }, { 1, 1 });
	BOOST_CHECK(line);
	editor::ICirclePtr circle = shapes->CreateCircle({ 0, 0 }, 0.5);
	BOOST_CHECK(circle);
	editor::IRectanglePtr rect = shapes->CreateRectangle({ 1.5, 1.6 }, { 2.7, 2.8 });
	BOOST_CHECK(rect);
	BOOST_CHECK(shapes->GetCount() == 4);

	shapes->Delete(point->GetTag());
	BOOST_CHECK(shapes->GetCount() == 3);
	shapes->Delete(circle->GetTag());
	BOOST_CHECK(shapes->GetCount() == 2);

	BOOST_CHECK(doc->Save());
}