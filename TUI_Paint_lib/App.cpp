#include <App.h>

int main(int argc, char* argv[]) try
{
	return App::main(argc, argv);
}
catch (TUIPaint::XError const& e)
{
	cerr << "TUIPaint::consoleException: " << e.msg() << std::endl;
}
catch (std::exception const& e)
{
	cerr << "std::exception: " << e.what() << endl;
}
catch (...)
{
	cerr << "Error: an exception has been caught...\n";
	return EXIT_FAILURE;
}

int App::main(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
		thisApp->_args.push_back(argv[i]);
	return thisApp->Execute();
}

App* App::thisApp = nullptr;

App::App()
{
	if (thisApp)
		throw std::exception("Error: App already initialized");
	thisApp = this;
}

int App::Execute() {
	cout << "App console framework, (c) us, 2017\n";
	return EXIT_SUCCESS;
}

