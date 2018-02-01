#pragma once
// DepAnal.h

#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <set>
#include <fstream>
#include <string>
#include "../TypeTable/TypeTable.h"


using namespace CodeAnalysis;
//namespace DepAnalysis {

	//using File = std::string;
	//using Files = std::vector<File>;
	
	using Pattern = std::string;
	using Ext = std::string;
	using FileMap = std::unordered_map<Pattern, std::vector<std::string>>;
	using SPtr = std::shared_ptr<ASTNode*>;

	class DepAnal
	{
	public:
		
		DepAnal();
		void doDepAnal();
		void tokenizeForDependencies(FileMap files, std::unordered_map<Type, std::vector<TypeInfo>> typeTable);
		void analyzeDependencies(std::vector<std::string> vec, std::string file, std::unordered_map<Type, std::vector<TypeInfo>> typetable);
		void displayDependencies();
		std::unordered_map<std::string, std::set<std::string>> getDependencies();
	private:
		void DFS(ASTNode* pNode);
		AbstrSynTree& ASTref_;
		ScopeStack<ASTNode*> scopeStack_;
		Scanner::Toker& toker_;

		std::unordered_map<std::string, std::set<std::string>> depAnalStore;
	};

	inline DepAnal::DepAnal() :
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker()))
	{
	}

	inline void DepAnal::DFS(ASTNode* pNode)
	{
		static std::string path = "";
		if (pNode->path_ != path)
		{
			std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
			path = pNode->path_;
		}
		std::cout << "\n  " << pNode->name_;
		std::cout << ", " << pNode->type_;
		for (auto pChild : pNode->children_)
			DFS(pChild);
	}

	inline void DepAnal::doDepAnal()
	{
		std::cout << "\n  starting type analysis:\n";
		ASTNode* pRoot = ASTref_.root();
		DFS(pRoot);
	}

//}