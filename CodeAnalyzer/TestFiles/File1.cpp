#include "File1.h"
#include <string>
#include<iostream>
#include <vector>
using namespace File1NS;

void myGlobFunc()
{
	std::cout << "I dnt do any thing..except I am global function";
}

typedef int int_t;

using globAlias = std::vector<std::string>;
struct File1Str
{
	int a;
	std::string str;

};

void File1CL::file1Fun1()
{
	File1Str str1;
	str1.a = 23;
	str1.str = "myString";

	std::cout << "Jst the normal function that uses struct";
	
}

int File1CL::file1Fun2()
{

	using myAliasFile1 = std::string;

	
	varFile1 = 23;
	myAliasFile1 str1 = "Komal";
	return varFile1;
}
