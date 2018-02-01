# DependencyAnalysis
A Software tool for Code Analysis using C++, Visual Studio 2015.

Purpose:
Build a software tool for Code Analysis. Currently the software emphasizes on C++ Code. But it can be easily extended to other similar languages like C# and Java.


Code Analyis consists of extracting the Lexical content from Source files, analyzing the Code's Syntax from its lexical content, and building a Type Table (TT) that holds the results of analysis.
It is then fairly easy to build several backends that can do further analysis on the data held in Type Table to construct code metrics, search for particular constructs, or some other interesting feature of the code.

![codeanalyzerpackages](https://user-images.githubusercontent.com/24962915/35677161-8f2f1fba-071c-11e8-95ef-839b2018c83f.png)

Description to the Major Packages in the Code Analysis -

1. TypeTable - Provides a container class that stores type information needed for dependency analysis. Type Table is built using the Parser Package.
(Inorder to understand how Parser works, see the ParserBlog: 
http://ecs.syr.edu/faculty/fawcett/handouts/webpages/BlogParser.htm

This Parser demo code is packaged so that you only need to change 3 files to support any parsing application:
- ActionsAndRules.h:
  Defines all of the parser's IRule and IAction derived classes.
  - Each rule detects some grammatical construct, e.g., function definition or data declaration.
  - Each action does something when the input SemiExp matches the rule, e.g., add info to Repository or print something
- ConfigParser.h and ConfigParser.cpp
  Constructs instances of tokenizer, parser, all the rules, and all the actions.  
  The ConfigParserToConsole owns all of these objects, holds onto them for the entire exection and then destroys them when it goes out of scope.
You might also add to Utilities to provide small helper functions and classes as needed by your implementation.)

![parserstaticstructure](https://user-images.githubusercontent.com/24962915/35677192-a76e0852-071c-11e8-892a-4aa7ed19a750.png)

2. TypeAnalysis - Finds all the types and global functions defined in each of a collection of C++ source files. 
                  It does this by building rules to detect:
                  ◦ type definitions - classes, structs, enums, typedefs, and aliases. 
                  ◦ global function definitions
                  ◦ global data definitions and capture their fully qualified names and the files where they are defined. It uses the TypeTable package to store that information. 

3. DependencyAnalysis:Finds, for each file in a specified file collection, all other files from the file collection on which they depend. File A depends on file B, if and only if, it uses the name of any type or global function or global data defined in file B. It might do that by calling a function or method of a type or by inheriting the type. Note that this intentionally does not record dependencies of a file on files outside the file set, e.g., language and platform libraries. 

4. Strong Components:A strong component is the largest set of files that are all mutually dependent. That is, all the files which can be reached from any other file in the set by following direct or transitive dependency links. The term "Strong Component" comes from the theory of directed graphs. There are a number of algorithms for finding strong components in graphs. In this project, I am using Tarjan Algorithm. 

5. Display:Uses information in the Analyzer's AST and TypeTable to build an effective display of the dependency relationships between all files in the selected file set. 

6. NoSqlDb:Stores and retrieves dependency information in a NoSqlDb<std::string>, (Please See NoSQL Database Repository for Details). Here, child relationships denote compile dependencies. 

7. TestExecutive:Provides code to demonstrate functioning of all the packages described above. 








