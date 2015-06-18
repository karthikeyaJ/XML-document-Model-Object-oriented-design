#ifndef DISPLAY_H
#define DISPLAY_H
////////////////////////////////////////////////////////////////////
// Display.h - prints all the strings to the outstream            //
// Ver 1.2                                                        //
// Platform:    Dell, Win 8.1 Pro, Visual Studio 2013             //
// Author:      Venkata karthikeya jangal,                        //
//              vjangal@syr.edu                                   //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*This package is responsible for printing the strings passed to it
*
* Public Interface Documentation
*void display(std::string String);
*void display_int(int i);
* Build Process:
* --------------
*   devenv AST.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 21 march 15
* - Added functions to print strings

*/

#include<iostream>

	class Display
	{
	public:
		void display(std::string String);
		void display_int(int i);
	};

#endif