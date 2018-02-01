#include "File2.h"
#include <iostream>

int File2CL::funCL2()
{
	int val = cl.file1Fun2();
	return val;
}

typedef int int_tim;
enum File2Enum {a,b,c};

namespace file2NS
{
	struct Fil
	{
		int_tim al;
		int_tim bl;

	};
	
}

int main()
{
	File2CL file2;
	int i = file2.funCL2();

	std::cout << "Printed from main : " << i;


	
}