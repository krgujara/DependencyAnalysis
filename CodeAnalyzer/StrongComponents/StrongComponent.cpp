///////////////////////////////////////////////////////////////
// StrongComponent.cpp                                       //
//This package is used to analyze the strong componets from the
//graph created for the Depency analysis results we get after//
//depency analysis                                           //
// Language:    Visual C++ 2015                              //
// Platform:    Windows 10 Macbook                           //
// Author:      Komal Gujarathi, Syracuse University         //
//              (315) 744 6116, krgujara@syr.edu             //
///////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <unordered_map>
#include<set>
#include <stack>
#include "StrongComponent.h"
#include <algorithm>
using namespace GraphLib;
typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef Display<std::string, std::string> display;

void Tarjan();
void strongconnect(vertex *vPtr);


void showVert(Vertex<std::string, std::string>& v)
{
	std::cout << "\n  " << v.id();
}

template<typename V, typename E>
void TshowVert(Vertex<V, E>& v)
{
	std::cout << "\n  " << v.id();
}
graph depGraph;
#ifdef TEST_GRAPH

int main()
{
	std::cout << "\n  Testing Graph Library";
	std::cout << "\n =======================\n";
	std::unordered_map<std::string, std::set<std::string>> dependencies;
	dependencies.insert({ "File1",{ "File2" } });
	dependencies.insert({ "File2",{ { "File3" },{ "File4" } } });
	dependencies.insert({ "File3",{ { "File4" },{ "File5" } } });
	dependencies.insert({ "File4",{ { "File1" },{ "File6" } } });
	try {
		for (auto item : dependencies)
		{
			std::cout << item.first;
			std::string val = item.first;
			vertex parentVertex = depGraph.findVertexByName1(val);
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
		display::show(depGraph);
		Tarjan();
	}
	catch (std::exception e) {
		std::cout << "Exception : " << e.what();
	}
	std::cout << "\n\n";
	return 0;
}

#endif
void Tarjan()
{
	for (size_t i = 0; i < depGraph.size(); i++)
	{
		if (depGraph[i].index == -1)
		{
			strongconnect(&depGraph[i]);
		}
	}
	
}
void strongconnect(vertex *v)
{
	static int index = 0;
	static std::stack<vertex *> S;
	vertex *w;
	static int scccount = 0;
	std::unordered_map<int, std::vector<std::string>> scc;

	v->index = index;
	v->lowlink = index;
	index = index + 1;
	S.push(v);
	v->onStack = true;

	//consider the successor of v
	for (size_t i = 0; i < v->size(); ++i)
	{
		vertex::Edge edge = (*v)[i];
		w = &depGraph[edge.first];

		if (w->index == -1){
			//successor w has not yet been visited, recurse on it
			strongconnect(w);
			v->lowlink = std::min(v->lowlink, w->lowlink);
		}
		else if (w->onStack){
			//successor w is in the stack and hence in the current SSC
			v->lowlink = std::min(v->lowlink, w->index);
		}
	}
	//if v is a root node, pop the stack and generate an SSC
	if (v->lowlink == v->index)
	{
		std::vector<vertex*> strongcompo;
		std::cout << "\nNew Strongly connected component";
		do
		{
			w = S.top();
			S.pop();
			w->onStack = false;
			strongcompo.push_back(w);
			scc[scccount].push_back(w->get_v());
		} while (w->id() != v->id());

		for (auto vt : strongcompo) {

			std::cout << vt->get_v() << "\t";
		}
		std::cout << "\n";
	}
}