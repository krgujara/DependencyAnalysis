///////////////////////////////////////////////////////////////
// Test.cpp - Creates Test stub for finding dependency analysis// 
// and finding the strongly connectecd components            //
//                                                           //
// Language:    Visual C++ 2015                              //
// Platform:    Windows 10 Macbook                           //
// Author:      Komal Gujarathi, Syracuse University         //
//              (315) 744 6116, krgujara@syr.edu             //
///////////////////////////////////////////////////////////////

#include "Test.h"
#include "../DepAnal/DepAnal.h"
#include "../Parser/ActionsAndRules.h"
#include "../Analyzer/Executive.h"
#include "../NoSqlDb/NoSqlDb.h"
#include "../StrongComponents/StrongComponent.h"
#include <algorithm>
using namespace GraphLib;
TestStub::TestStub()
{
}

TestStub::~TestStub()
{
}
void displayDB(NoSqlDb<std::string> db);

void requirement1();
void requirement2();
void requirement3();
void requirement4();
void requirement5();
void requirement6(std::unordered_map<std::string, std::set<std::string>> dep);
void requirement7();
void requirement8();
void requirement9();
void Tarjan1();
typedef Vertex<std::string, std::string> vertex;
void strongconnect1( vertex*vPtr);
Graph<std::string, std::string> depGraph;
void persistSCC(std::unordered_map<int, std::vector<std::string>> scc);
std::unordered_map<int, std::vector<std::string>> scc;

#ifdef TESTSTUB
int main(int argc, char *argv[]) {

	CodeAnalysisExecutive exec;
	using DepStore = std::unordered_map<std::string, std::set<std::string>>;
	try {
		bool succeeded = exec.ProcessCommandLine(argc, argv);
		if (!succeeded)
		{
			return 1;
		}
		exec.getSourceFiles();
		exec.processSourceCode(true);

		requirement1();
		requirement2();
		requirement3();
		requirement4();
		TypeTable tt;
		tt.createTypeTable();
		std::cout << "\nType tabe Created\n";
		Store typeTable = tt.getTypeTable();

		requirement5();

		DepAnal danal;
		FileMap files = exec.getFileMap();
		danal.tokenizeForDependencies(files, typeTable);
		danal.displayDependencies();
		DepStore dependencies = danal.getDependencies();

		requirement6(dependencies);

		

		requirement7();
		NoSqlDb<StrData> db;
		for (auto file : dependencies)
		{
			Element<StrData> elem;
			elem.name = file.first;
			elem.category = "Parent";
			elem.description = "This is Parent File";
			elem.timeDate = "3/7/2017 7:00";
			elem.data = "File data";
			for (auto dependentFile : file.second)
			{
				elem.addChildToElement(db, dependentFile);
			}
			db.add(elem.name, elem);
		}
		displayDB(db);
		std::cout << "\n. Persisting the data to XML file at Location "<<exec.getXmlPath();
		//db.toXML("../XML Data/data.xml");
		db.toXML(exec.getXmlPath());
		requirement8();
		requirement9();
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
void displayDB(NoSqlDb<std::string> db)
{
	//Displaying the database elements

	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";

	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";
}
void requirement1()
{
	std::cout << "Demonstrating Project #2: DEPENDENCY ANALYSIS\n\n";
	std::cout << "Requirement 1 : Shall be implemented in Visual Studio 2015 and its C++ windows console\n ";
	std::cout << "\n=================================\n";
}
void requirement2()
{
	std::cout << "Requirement 2 : Shall use C++ standard library's streams for IO and new delete for heap based memory management\n ";
	std::cout << "\n=================================\n";
}

void requirement3()
{
	std::cout << "Requirement 3 : Shall provide C++ packages as described in Purpose section. Single Repository Principle enforced\n ";
	std::cout << "\n=================================\n";

}
void requirement4()
{
	std::cout << "Requirement 4 : Shall provide TypeAnalysis package that identifies all types\n ";
	std::cout << "\n=================================\n";

}
void requirement5()
{
	std::cout << "\n=================================\n";
	std::cout << "Requirement 5 : Shall provide DependencyAnalysis package that identifies all dependencies between files in a specified file collection\n ";
	std::cout << "\n=================================\n";


}
void requirement6(std::unordered_map<std::string, std::set<std::string>> dependencies)
{
	std::cout << "\n=================================\n";
	std::cout << "Requirement 6 : Shall find Strong Components in the dependency graph defined by relationships rvaluated in the previous requirement\n ";
	std::cout << "\n=================================\n";
	try {

		typedef Graph<std::string, std::string> graph;
		typedef Vertex<std::string, std::string> vertex;
		typedef Display<std::string, std::string> display;

		for (auto item : dependencies)
		{
			//std::cout << item.first;
			std::string val = item.first;

			vertex parentVertex = depGraph.findVertexByName1(val);
			//std::cout << "parentVertex: " << parentVertex.index << "   "<< parentVertex.get_v();
			size_t id = parentVertex.id();
			if (id == -1)
			{
				//Node doesnot exist
				vertex newV(val);
				parentVertex = newV;
				depGraph.addVertex(newV);
			}

			for (auto file : item.second)
			{
				std::string val1 = file;
				vertex childVertex = depGraph.findVertexByName1(val1);
				size_t id = childVertex.id();
				if (id == -1) {
					vertex newV1(val1);
					childVertex = newV1;
					depGraph.addVertex(newV1);
				}
				std::string edgeName = item.first + "to" + file;
				depGraph.addEdge(edgeName, parentVertex, childVertex);
			}
		}
		//display::show(depGraph);
		Tarjan1();
	}
	catch (std::exception e) {
		std::cout << "Exception : " << e.what();
	}
}

//-------<Tarjan algorithm is used to find the strongly connected components>
void Tarjan1()
{
	for (size_t i = 0; i < depGraph.size(); i++)
	{
		if (depGraph[i].index == -1)
		{
			strongconnect1(&depGraph[i]);
		}
	}
	persistSCC(scc);
}
void strongconnect1(vertex *v)
{
	static int index = 0;
	static std::stack<vertex *> S;
	vertex *w;
	static int scont = 0;
	static int scccount = 0;
	v->index = index;
	v->lowlink = index;
	index = index + 1;
	S.push(v);
	v->onStack = true;
	//consider the successor of v
	for (size_t i = 0; i < v->size(); ++i){
		vertex::Edge edge = (*v)[i];
		w = &depGraph[edge.first];
		if (w->index == -1){
			//successor w has not yet been visited, recurse on it
			strongconnect1(w);
			if (v->lowlink > w->lowlink)
			{
				v->lowlink = w->lowlink;
			}
		}
		else if (w->onStack){
			//successor w is in the stack and hence in the current SSC
			if (v->lowlink > w->lowlink)
			{
				v->lowlink = w->lowlink;
			}
		}
	}
	//if v is a root node, pop the stack and generate an SSC
	if (v->lowlink == v->index) {
		std::vector<vertex*> strongcompo;
		std::cout << "\nNew Strongly connected component";
		do{
			w = S.top();
			S.pop();
			w->onStack = false;
			strongcompo.push_back(w);
			scc[scccount].push_back(w->get_v());
		} while (w->id() != v->id());
		scccount++;
		std::cout << "\n\nStrong Component Number "<<scont++<<": \n";
		for (auto vt : strongcompo) {
			std::cout << vt->get_v() << "\n";
		}
		std::cout << "\n";
	}
}

void persistSCC(std::unordered_map<int, std::vector<std::string>> scc)
{
	NoSqlDb<std::string> dbSCC;

	int i = 0;
	for (std::unordered_map<int, std::vector<std::string>>::iterator it = scc.begin(); it != scc.end(); ++it)
	{
		Element<StrData> elem;
		elem.name = std::to_string(it->first);
		elem.category = "Parent";
		elem.description = "This is Parent File";
		elem.timeDate = "3/7/2017 7:00";
		elem.data = "File data";
		for (auto strongCompo : it->second)
		{
			elem.addChildToElement(dbSCC, strongCompo);
		}

		dbSCC.add(elem.name, elem);
		//i++;
	}
	displayDB(dbSCC);
	std::cout << "\n. Persisting the data to XML file at Location ../XML Data/SCC.xml";
	//db.toXML("../XML Data/data.xml");
	dbSCC.toXML("../XML Data/SCC.xml");
}

void requirement7()
{
	std::cout << "\n=================================\n";

	std::cout << "Requirement 7 : Should write the analysis results, in XML format, to a specified file\n ";
	std::cout << "\n=================================\n";

}
void requirement8()
{
	std::cout << "\n=================================\n";

	std::cout << "Requirement 8 : Shall process command line accepting: \n1. Path to directory tree containing files to analyze\n 2. List of File patterns to match for selection of files to analyze\n3. Specification of XML file, supplying a default if no specification is provided";
	std::cout << "\n=================================\n";
	std::cout << "Implemented..";
	//--------------------------
	std::cout << "\n=================================\n";

}
void requirement9()
{
	std::cout << "Requirement 9 : Shall include an automated unit test suite that demonstrates you meet all the requirements of this project";
	std::cout << "=================================\n";
	std::cout << "Implemented";

}

