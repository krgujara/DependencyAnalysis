// DepAnal

#include "DepAnal.h"

//----< helper: is text a substring of str? >--------------------
//
//namespace DepAnalysis {
	bool contains_str(const std::string& str, const std::string& text)
	{
		if (str.find(text) < str.length())
			return true;
		return false;
	}

	void DepAnal::tokenizeForDependencies(FileMap files, std::unordered_map<Type, std::vector<TypeInfo>> typeTable)
	{
		std::vector<std::string> vec;

		for (auto item : files)
		{
			if (contains_str(item.first, "*.cpp") || contains_str(item.first, "*.h"))
			{
				for (auto file : item.second) {
					std::ifstream in(file);
					if (!in.good()) continue;
					Scanner::Toker toker;
					toker.returnComments(false);
					toker.attach(&in);
					std::cout << "\nCurrently processing :\n " << file;
					do
					{
						std::string tok = toker.getTok();
						if (tok == "\n")
							continue;
						//	tok = "newline";
						//std::cout << "\n -- " << tok;
						vec.push_back(tok);


					} while (in.good());
					analyzeDependencies(vec, file, typeTable);
				}
			}
		}

	}

	void DepAnal::displayDependencies()
	{
		std::cout << "----Dependency Analysis has : " << depAnalStore.size() << std::endl;

		for (auto item : depAnalStore)
		{
			std::cout << item.first << " depends on ";
			for (std::set<std::string>::iterator it = item.second.begin(); it != item.second.end(); ++it)
			{
				std::cout << "\n" << *it;
			}
		}
	}

	std::unordered_map<std::string, std::set<std::string>> DepAnal::getDependencies()
	{
		return depAnalStore;
	}

	void DepAnal::analyzeDependencies(std::vector<std::string> vec, std::string file, std::unordered_map<Type, std::vector<TypeInfo>>
		
		
		
		
		typetable)
	{
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			//std::cout << "\n" << *it;

			//A file cannot be dependent on itself
			for (std::pair<Type, std::vector<TypeInfo>> item : typetable)
			{

				for(std::vector<TypeInfo>::iterator it1 = item.second.begin(); it1 != item.second.end();++it1 )
				if (it1 -> _filename != file)
				{
					if (*it == item.first)
					{
						depAnalStore[file].insert(it1->_filename);
					}
				}
			}
		}
	}

//}

#ifdef DEPANAL_TEST


int main(int argc, char* argv[])
{
	using namespace CodeAnalysis;
	//using namespace DepAnalysis;

	//std::cout << "Starting to execute";

	CodeAnalysisExecutive exec;

	try {
		bool succeeded = exec.ProcessCommandLine(argc, argv);
		if (!succeeded)
		{
			return 1;
		}
		exec.getSourceFiles();
		exec.processSourceCode(true);

		TypeTable ta;
		ta.createTypeTable();
		std::unordered_map<Type, std::vector<TypeInfo>> typeTable = ta.getTypeTable();
		DepAnal da;
		//da.doDepAnal();

		FileMap files = exec.getFileMap();

		da.tokenizeForDependencies(files,typeTable);

		da.displayDependencies();

		std::unordered_map<std::string, std::set<std::string>> dependencies = da.getDependencies();

	}
	catch (std::exception& except)
	{
		exec.flushLogger();
		std::cout << "\n\n  caught exception in Executive::main: " + std::string(except.what()) + "\n\n";
		exec.stopLogger();
		return 1;
	}
	return 0;
}
#endif