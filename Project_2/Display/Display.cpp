
////////////////////////////////////////////////////////////////////
// Display.cpp - Definitions for display.h functions,             //
//                    prints all the strings                      //
// Ver 1.0                                                        //
// Platform:    Dell, Win 8.1 Pro, Visual Studio 2013             //
// Author:      Venkata karthikeya jangal,                        //
//              vjangal@syr.edu                                   //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*This package is responsible for printing the strings passed to it
*
*
* Build Process:
* --------------
*   devenv AST.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 21 march 15
* - Added functions to print strings

*/

#include "Display.h"
#include<iostream>
#include <string>

void Display::display(std::string String)
{
	std::string print = String;
	std::cout<< print;
}
void Display::display_int(int i)
{
	int k = i;
	std::cout << k;
}

#ifdef display_h
int main()
{
	std::string k = "karthik";
	Display ptr;
	ptr.display(k);
}
#endif