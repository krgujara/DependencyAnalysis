#pragma once
/////////////////////////////////////////////////////////////////////
// NoSqlDb.h - key/value pair in-memory database                   //
//                                                                 //
// Komal Gujarathi, Project 1 Key Value database                   //
/////////////////////////////////////////////////////////////////////
/*
NoSql db acts as the in memory database that stores the data as well 
as meta data associated with that data.
NoSql Db is templated on the data part, meaning, it can take any type od
data, and metadata stores the information about that data.

* Required Files:
* ---------------
*  cppProperties.h, cppProperties.cpp
*  XmlDocument.h , XmlDocument.cpp
*
* Build Process:
* --------------
*   devenv FileMgr.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1 created on 31th Jan 2017 
* - added more prologue comments
*/

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#include <regex>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"

/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair
//
template <class T>
class NoSqlDb;

using StrData = std::string;
using Key = std::string;
using keys = std::vector<std::string>;


template<typename Data>
class Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using Description = std::string;
	using TimeDate = std::string;
	using Children = std::vector<std::string>;
	using child = std::string;

	Property<Name> name;            // metadata
	Property<Category> category;    // metadata
	Property<Description> description;//metadata
	Property<TimeDate> timeDate;    // metadata
	Children childrenList;		  // metadata
	Property<Data> data;            // data
	std::vector<std::string>::iterator it;
	std::string show();
	void addChildToElement(NoSqlDb<StrData>, child);
	};
/////////////////////////////////////////////////////////////////////
///ValidateChild function is used to validate if the
//child entered by the user is already a key to some 
//element.

template<typename Data>
void Element<Data>::addChildToElement(NoSqlDb<StrData> db, child ch)
{

	//Iterated through all the keys in the store and finds if that key exixts as a parent
		childrenList.push_back(ch);

}


template<typename Data>
std::string Element<Data>::show()
{
	
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "description" << " : " << description;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "children list" << " : ";
	for (it = childrenList.begin(); it != childrenList.end(); ++it)
	{
		out << "\n      " << *it;
	}
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	out << "\n\n\n";
	return out.str();
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements

template<typename Data>
class NoSqlDb
{

private:
	using Item = std::pair<Key, Element<Data>>;

	std::unordered_map<Key, Element<Data>> store;

public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	Keys keys();
	bool add(Key key, Element<Data> elem);
	bool delete_(Key key);
	Element<Data> value(Key key);
	size_t count();
	friend void Element<Data>::addChildToElement(NoSqlDb<StrData> db, child ch);
	enum QueryType { keyType, nameType, categoryType, datetimeType, childrenType, stringDataType };
	std::vector<std::string> executeQuery(QueryType qT, std::string arg1, std::string arg2 = "");
	std::vector<std::string> CompoundQuery(bool AND, std::vector<std::string>&r1, std::vector<std::string>&r2);
	void insertChild(Element<std::string>&elem, std::string child);
	void toXML(std::string filepath);
	void fromXML(std::string filepath);
	void editKey(Key key);
};


template<typename Data>
void NoSqlDb<Data>::insertChild(Element<std::string>&elem, std::string child) {
	//if (store.find(child)!=store.end()) {
		elem.childrenList.push_back(child);
	//}
	//else {
		//std::cout << "\nthis is not valid child";
	//}
}
template<typename Data>
void NoSqlDb<Data>:: editKey(Key key)
{
	std::cout << "\nERROR: Editing of keys is forbidden";
}



template<typename Data>
std::vector<std::string> NoSqlDb<Data>::executeQuery(QueryType qt, std::string pattern, std::string arg2) {

	//regex string that either ends with arg1 or string is between or starts with arg1
	std::string s = "(.*)" + pattern + "(.*)|(.*)" + pattern + "|" + pattern + "(.*)";
	std::regex rgx(s);
	time_t dt1, dt2, keydt1;

	if (qt == childrenType && store.find(pattern) != store.end())
		return store[pattern].childrenList;
	
	std::vector<std::string> result;
	//switch based on the choice entered by user
	for (auto it = store.begin(); it != store.end(); ++it) {
		switch (qt) {
		case keyType:
			if (std::regex_match(it->first, rgx))
				result.push_back(it->first);
			break;
		case  nameType:
			if (std::regex_match(it->second.name.getValue(), rgx))
				result.push_back(it->first);
			break;
		case categoryType:
			if (std::regex_match(it->second.category.getValue(), rgx))
				result.push_back(it->first);
			break;
		case datetimeType:
			dt1 = ConvertDateTime(pattern);
			dt2 = ConvertDateTime(arg2);
			keydt1 = ConvertDateTime(it->second.timeDate.getValue());

			if (difftime(dt1, keydt1) <= 0 && 
				difftime(keydt1, dt2) <= 0)
			{
				result.push_back(it->first);
			}
			break;
		case stringDataType:
			if (std::regex_match(it->second.data.getValue(), rgx))
				result.push_back(it->first);
			break;
		default:
			result.push_back(it->first);
			break;
		}
	}
	return result;
}

time_t ConvertDateTime(std::string buff)
{
	int yy, mm, dd, hour, min;
	struct tm when;

	sscanf_s(buff.c_str(), "%d/%d/%d %d:%d", &mm, &dd, &yy, &hour, &min);

	when.tm_year = yy - 1900; when.tm_mon = mm - 1; when.tm_mday = dd;
	when.tm_hour = hour; when.tm_min = min; when.tm_sec = 0;
	return(mktime(&when));
}


/////////////////////////////////////////////////////////////////////
///Compound Query class is used to demonstrate requirement number 8,9,12

template<typename Data>
std::vector<std::string> NoSqlDb<Data>::CompoundQuery(bool AND, std::vector<std::string>&r1, std::vector<std::string>&r2)
{
	std::set<std::string> s1(r1.begin(),r1.end());
	std::set<std::string> s2(r2.begin(), r2.end());
	std::vector<std::string> results;
	if (AND) std::set_intersection(s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter(results));
	else std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(results));
	return results;
}

/////////////////////////////////////////////////////////////////////
/// Converts the in memory Database to XML format and stores in XML file

template<typename Data>
inline void NoSqlDb<Data>::toXML(std::string filepath)
{
	std::ostringstream oss;
	oss << "<database>";
	for (auto it = store.begin(); it != store.end(); ++it) {
		oss << "   <element>";
		oss << "\n      <key>" << it->first << "</key>";
		oss << "\n      <name>" << it->second.name.getValue() << "</name>";
		oss << "\n      <category>" << it->second.category.getValue() << "</category>";
		oss << "\n      <description>" << it->second.description.getValue() << "</description>";
		oss << "\n      <timeDate>" << it->second.timeDate.getValue() << "</timeDate>";
		oss << "\n      <children>";// << it->second.children << "</children>";
		std::vector<std::string> children = it->second.childrenList;
		size_t size = children.size();
		for (int i = 0; i < size; i++)
			oss << "\n         <child>" << children[i] << "</child>";
		oss << "\n      </children>";
		oss << "\n      <data>" << it->second.data.getValue() << "</data>";
		oss << "\n   </element>";

	}
	oss << "\n</database>";

	std::ofstream ofs(filepath);
	ofs << oss.str();
	ofs.close();
	std::cout << "saved at path: " << filepath;
}


/////////////////////////////////////////////////////////////////////
/// NoSqlDb class takes the contents from XMl file and reads into the in-memory database


template<typename Data>
inline void NoSqlDb<Data>::fromXML(std::string filepath)
{	try
	{
		XmlDocument doc(filepath, XmlDocument::file);
		std::vector<SPtr> desc = doc.descendents("element").select();
		std::istringstream iss;
		int size_ = desc.size();
		for (int i = 0; i < desc.size(); i++)
		{
			Element<Data> newE;
			// fetch the name string from XML element
			std::vector<XmlDocument::sPtr> desc = doc.descendents("name").select();
			std::string name_value = desc[i]->children()[0]->value();
			newE.name = name_value;
			desc = doc.descendents("description").select();
			name_value = desc[i]->children()[0]->value();
			newE.description = name_value;
			desc = doc.descendents("category").select();
			name_value = desc[i]->children()[0]->value();
			newE.category = name_value;
			desc = doc.descendents("timeDate").select();
			name_value = desc[i]->children()[0]->value();
			newE.timeDate = name_value;
			desc = doc.descendents("data").select();
			Data data;
			iss.str(desc[i]->children()[0]->value());
			iss >> data;
			newE.data = data;
			iss.clear();
			std::vector<SPtr> desc2 = doc.descendents("children").select()[i]->children();
			int size_ = desc2.size();
			//std::cout << "printing size_ for" << i << size_;
			for (int j = 0; j < desc2.size(); j++)
			{
				//std::vector<SPtr> desc2 = doc.descendents("child").select();
				std::string  new_value1 = desc2[j]->children()[0]->value();
				newE.childrenList.push_back(new_value1);
			}
			add(newE.name, newE);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\nError " << ex.what();
	}
}

/////////////////////////////////////////////////////////////////////
/// Keys returns all the keys from database

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}

template<typename Data>
bool NoSqlDb<Data>::add(Key key, Element<Data> elem)
{
	if (store.find(key) != store.end())
		return false;
	store[key] = elem;
	return true;
}

template<typename Data>
bool NoSqlDb<Data>::delete_(Key key)
{
	if (store.find(key) != store.end()) {
		//there exists an element
		store.erase(key);

		//Element refrerences from the children List of other elements also deleted
		for (auto it = store.begin(); it != store.end(); it++) {
			std::vector<std::string>::iterator it2;
			it2= find(it->second.childrenList.begin(), it->second.childrenList.end(), key);
			if (it2 != it->second.childrenList.end())
			{
				it->second.childrenList.pop_back();
			}
		}
		std::cout << "\nElement deleted successfully!!Element reference from Other child list also deleted";
		return true;

	}
	else {
		return false;
	}
}


template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}

template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.size();
}

std::istream& operator >> (std::istream& in, std::string& ss) {
	std::getline(in, ss);
	return in;
}


