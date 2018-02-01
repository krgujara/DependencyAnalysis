/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include "../CppProperties/CppProperties.h"
#include "../Convert/Convert.h"
#include "../StrHelper.h"
#include "NoSqlDb.h"
using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
#ifdef NOSQLDB_CPP
int main()
{
  std::cout << "\n  Creating and saving NoSqlDb elements with string data";
  NoSqlDb<StrData> db;
  Element<StrData> elem1;
  elem1.name = "elem1";
  elem1.category = "test";
  elem1.description = "elem1 description";
  elem1.timeDate = "12.00 23-5-1993";
  elem1.data = "elem1's StrData";
  db.add(elem1.name, elem1);
  Element<StrData> elem2;
  elem2.name = "elem2";
  elem2.category = "test";
  elem2.description = "elem2 description";
  elem2.childrenList.push_back("elem1");
  elem2.timeDate = "12.00 23-5-1993";
  elem2.data = "elem2's StrData";
  db.add(elem2.name, elem2);
  std::cout << "\n Changing the description of element 2";
  Element<std::string> &ele = db.value("elem2");
  ele.description = "new Description";
  std::cout << "\n  Retrieving elements from NoSqlDb<string>";
  Keys keys = db.keys();
  for (Key key : keys)
  {
    std::cout << "\n  " << key << ":";
    std::cout << db.value(key).show();
  }
  std::cout << "\n  Creating and saving NoSqlDb elements with int data";
  NoSqlDb<int> idb;
  Element<int> ielem3;
  ielem3.name = "elem3";
  ielem3.category = "test";
  ielem3.data = 4;
  idb.add(ielem3.name, ielem3);
  Element<int> ielem4;
  ielem4.name = "elem4";
  ielem4.category = "test";
  ielem4.childrenList.push_back(ielem3.name);
  ielem4.data = 3;
  std::cout << "\n  Retrieving elements from NoSqlDb<int>";
  std::cout << "\n ---------------------------------------\n";
  Keys ikeys = idb.keys();
  for (Key key : ikeys)
  {
    std::cout << "\n  " << key << ":";
    std::cout << idb.value(key).show();
  }
 
}
#endif