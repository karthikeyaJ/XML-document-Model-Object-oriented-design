/////////////////////////////////////////////////////////////////////
//  Exec.cpp - Responsible for handling operations of Xml tree     //
//             operations                                          //
//  ver 1.0                                                        //
//                                                                 //
//  Language:      Visual C++ 2013, SP1                            //
//  Platform:      Dell i578,windows 8.1                           //
//  Application:   Prototype for CSE687 Pr2, Sp15                  //
//  Author:        Venkata Karthikeya Jangal,Syracuse Universit    //
//                  vjangal@syr.edu                                //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
*Responsible for taking commandline arguments
*Responsible for processing commandline arguments
*Responsible for Building tree using Xml Document object
*Responsible for displaying tree
*Responsible for module operations on the tree such as adding/removing tags,attributes from tree
*Mainly takes two kinds of inputs 1: String 2: File
Required files
- xmldocument.h,xmlelement.h

Maintenance History:
====================
ver 1.0 : 24 Mar 15

*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include<iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stack>
#include <exception>

#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include "../Display/Display.h"

using namespace XmlProcessing;

// Adding child to the tagged element
void adding_child(XmlDocument d,std::string element,std::string child)
{
	try
	{
		Display dstr;
		dstr.display("\n\n ###############################################################\n");
		dstr.display("\n\n demonstrating the requirement 7(a)\n i.e capability to add a child to any element in the tree\n");
		dstr.display(" ---------------------------------------------------------------\n");
		std::string print = "\n\n Searching the tag: <" + element + "> in the tree and\n adding child <" + child + "> to the tree\n";
		dstr.display(print);
		std::shared_ptr < AbstractXmlElement > found = d.DFS_r_ltr(d.getpdocElement(), element);
		if (found == NULL)
		{
			dstr.display("\n\n element not found,so add is unsuccessful\n\n");
			return;
		}
		std::shared_ptr < AbstractXmlElement > new_tag_element = makeTaggedElement(child);
		dstr.display("\n\nTree before adding element is \n\n");
		//if (d.getpdocElement()==NULL)
		dstr.display(d.getpdocElement()->toString());
		found->addChild(new_tag_element);
		dstr.display("\n\nTree after adding element is \n\n");
		dstr.display(d.getpdocElement()->toString());
		dstr.display("\n\n Element is succesfully added \n\n");
		dstr.display("=================================\n");
	}
	catch (std::exception e)
	{
		Display dstr;
		dstr.display(e.what());
	}
}


void removing_child(XmlDocument d, std::string element, std::string child)
{
	try
	{
		Display dstr;
		dstr.display("\n\n ###############################################################\n");
		dstr.display("\n\n demonstrating the requirement 7(b)\n i.e capability to remove a child element from any element in the tree\n");
		dstr.display(" ---------------------------------------------------------------\n");
		std::string prints = "\n\n Searching the tag: <" + element + "> in the tree and\n removing child <" + child + "> from the tree\n";
		dstr.display(prints);
		std::shared_ptr < AbstractXmlElement > found1 = d.DFS_r_ltr(d.getpdocElement(), element);
		if (found1 == NULL)
		{
			dstr.display("\n\n element not found,so remove is unsuccessful\n\n");
		}
		std::shared_ptr < AbstractXmlElement > found2 = d.DFS_r_ltr(d.getpdocElement(), child);
		if (found2 == NULL)
		{
			dstr.display("\n\n child not found,so remove is unsuccessful\n\n");
			return;
		}

		found1->removeChild(found2);
		dstr.display("=================================\n");
		dstr.display("\n\nTree after removing element is \n\n");
		dstr.display(d.getpdocElement()->toString());
		dstr.display("\n\n Element is succesfully deleted \n\n");
	}
	catch (std::exception e)
	{
		Display dstr;
		dstr.display(e.what());
	}
}




// Storing elements of same tag name of a tree in a vector and Printing them
void unique_id_vector(XmlDocument d,std::string tag)
{
	try
	{
		Display dstr;
		dstr.display("\n\n ###############################################################\n");
		dstr.display("\n\n demonstrating the requirement 6\n i.e collection of elements that have a specified tag\n");
		dstr.display(" ---------------------------------------------------------------\n");
		dstr.display("\n Searching the specified tag in the tree \n");
		std::vector<std::shared_ptr < AbstractXmlElement > > found = d.element_vector(d.getpdocElement(), tag);
		if (found.size() == 0)
		{
			dstr.display("\n\n element not found\n\n");
			return;
		}
		dstr.display("\n the size of the returned vector is :  ");
		dstr.display_int(found.size());
		dstr.display("\n\n the tag searched is : ");
		dstr.display(tag);
		dstr.display("\n\n the elements present in the vector with specified tag are : \n");
		if (found.size() != 0)
		{
			for (size_t i = 0; i < found.size(); i++)
			{
				dstr.display(found.at(i)->toString());
				dstr.display("\n");
			}
		}
	}
	catch (std::exception e)
	{
		Display dstr;
		dstr.display(e.what());
	}
}



//  Searching the atribute of particular tagged element
void attribute(XmlDocument d,std::string unique_attribute)
{
	try
	{
		Display dstr;
		dstr.display("\n\n ###############################################################\n");
		dstr.display("\n\n demonstrating the requirement 5\n i.e finding element by unique attribute\n");
		dstr.display(" ---------------------------------------------------------------\n");
		dstr.display("\n Searching for the unique element which has a attribute: \"");
		std::string attt = unique_attribute + "\"" + "\n";
		dstr.display(attt);
		std::shared_ptr < AbstractXmlElement > k = d.find_attribute(d.getpdocElement(), unique_attribute);
		dstr.display("\n\n this unique element is present in tag: <");
		if (k != NULL)
		{
			dstr.display(k->value());
			dstr.display(">");
		}
		else
			dstr.display("no unique element found");
	}
	catch (std::exception e)
	{
		Display dstr;
		dstr.display(e.what());
	}
}



// Adding attribute or attributes to the tree by searching tagged element
void addAttribute(XmlDocument d, std::vector<std::pair<std::string, std::string>> at,std::string tag)
{
	Display dstr;
	dstr.display("\n The actual tree present is\n");
	if (d.getpdocElement() == NULL)
		return;
	dstr.display(d.getpdocElement()->toString());
	std::string m = "\n\n Adding the following attributes to the tag " + tag + "\n" + " -------------------------------------------\n";
	dstr.display(m);
	for (size_t i = 0; i < at.size(); ++i)
	{
		std::string n = at.at(i).first + "=" + at.at(i).second + "\n";
		dstr.display(n);
	}
	try
	{
		std::shared_ptr < AbstractXmlElement > k = d.DFS_r_ltr(d.getpdocElement(), tag);
		if (k == NULL)
		{
			dstr.display("/n the tag entered is not found,please enter tag and try again/n");
			return;
		}
		for (size_t i = 0; i < at.size(); ++i)
			k->addAttrib(at.at(i).first, at.at(i).second);
		dstr.display("\n \n printing tree after adding attribute to the tag element :\n");
		dstr.display(d.getpdocElement()->toString());
	}
	catch (std::exception e)
	{
		Display dstr;
		dstr.display(e.what());
	}
}

//Deleting attribute of particular element of a tree
void deleteAttribute(XmlDocument d, std::pair<std::string, std::string> p, std::string tag)
{
	Display dstr;
	try{
		std::shared_ptr < AbstractXmlElement > k = d.DFS_r_ltr(d.getpdocElement(), tag);
		if (k == NULL)
		{
			dstr.display("\n the tag entered is not found,please enter tag and try again\n");
		return;
		}
		k->removeAttrib(p.first);
		dstr.display("\n \n printing tree after deleting attribute from the tagged element :\n");
		dstr.display(d.getpdocElement()->toString());
	}
	catch (std::exception e)
	{
		Display dstr;
		dstr.display(e.what());
	}
}

//Printing all the element details of a tag
void elementDetails(XmlDocument d,std::string tagg)
{
	try
	{
		Display dstr;
		dstr.display("\n\n ###############################################################\n");
		dstr.display("\n\n demonstrating the requirement 8\n i.e return a std::vector containing all the name-value attribute pairs and \n children attached to that element when pointer to that element is supplied.\n");
		std::string p="\n\n Searching the tag<" + tagg + "> in the tree and\n returning all the pointers of attributes and children \n";
		dstr.display(p);
		std::vector<std::pair<std::string, std::string>> found = d.attributeChildren(d.getpdocElement(), tagg);
		std::vector<std::shared_ptr < AbstractXmlElement >> chil = d.childrenAttribute(d.getpdocElement(), tagg);
		if (found.size() == 0)
			dstr.display("\n\n no elements with attributes found\n\n");
		dstr.display("\n the size of the returned vector is : ");
		dstr.display_int(found.size());
		dstr.display("\n\n the elements present in the vector are : \n");
		if (found.size() != 0)
		{
			for (size_t i = 0; i < found.size(); i++)
			{
				dstr.display("\n ===========Printing attribute with name and value==========\n");
				dstr.display(found.at(i).first);
				dstr.display(":");
				dstr.display(found.at(i).second);
			}
		}
		dstr.display("\n the size of the children vector is : ");
		dstr.display_int(chil.size());
		dstr.display("\n\n the children present in the vector are : \n");
		if (chil.size() != 0)
		{
			for (size_t i = 0; i < chil.size(); i++)
			{
				dstr.display("\n ===========Printing children==========\n"); 
				dstr.display(chil.at(i)->value());
			}
		}
		else
		{
			dstr.display("\n There are no children present to the tag \n");
		}
	}
	catch (std::exception e)
	{
		Display dstr;
		dstr.display(e.what());
	}
}

// BUilding a new tree if string or file is empty
void buildNewTree()
{
	try
	{
		Display dstr;
		using sPtr = std::shared_ptr < AbstractXmlElement >;
		dstr.display("Creating a new Xml document with content:\n");
		sPtr root = makeTaggedElement("root");
		sPtr dec1 = makeXmlDeclarElement();
		dec1->addAttrib("version", "1.0");
		dec1->addAttrib("encoding", "utf-8");
		sPtr docEl = makeDocElement(root, dec1);
		dstr.display(docEl->toString());
		std::ofstream file;
		file.open("new.xml");
		dstr.display("\n\n Writing the contents of xml to same file");
		file << docEl->toString();
		file.close();
		dstr.display("\n###############################################################\n");
	}
	catch (std::exception e)
	{
		Display dstr;
		dstr.display(e.what());
	}
}

// Calling move operations
void mov(XmlDocument d)
{
	Display dstr;
	dstr.display("\n\n ###############################################################\n");
	dstr.display("\n\n demonstrating the requirement 4\n i.e move construction.\n");
	dstr.display(" ---------------------------------------------------------------\n");
	if (d.getpdocElement() != NULL)
	{
		dstr.display("Printing tree before moving\n");
		dstr.display(d.getpdocElement()->toString());
		dstr.display("\n\nMoving the xml document\n");
		XmlDocument new_doc = std::move(d);
		dstr.display("\nPrinting tree after moving\n");
		dstr.display(new_doc.getpdocElement()->toString());
		dstr.display("\n\n\nd original:  ");
		std::cout<<(&d);// << "   yDoc (Moved)  " << &yDoc << "\n";
		dstr.display("\nnew_doc moved :");
		std::cout << (&new_doc);


		dstr.display("\n\n\n  Move Operator Demo:");
		
		XmlDocument zDoc;
		dstr.display(zDoc.getpdocElement()->toString());
		zDoc = std::move(new_doc);
		dstr.display(zDoc.getpdocElement()->toString());

		std::cout << "\n\nnew_doc (original):  " << &new_doc << "   zDoc (Moved)  " << &zDoc << "\n";
	}

}

//Printing requirement 9
void printReq9()
{
	Display dstr;
	dstr.display("\n\n ###############################################################\n");
	dstr.display("\n demonstrating the requirements 9\n i.e ability to add or remove an attribute name-value pair from any element node\n");
	dstr.display(" ---------------------------------------------------------------\n");
}

//Adding attribute values
std::vector<std::pair<std::string, std::string>> adAttr()
{
	std::vector<std::pair<std::string, std::string>> at;
	std::pair<std::string, std::string> pair("side", "'one_side'");
	at.push_back(pair);
	return at;
}

//Demonstrate recquirements
void operations(XmlDocument d)
{
	try
	{
		Display dstr;
		getchar();
		mov(d);
		std::string unique_attribute = "course";
		attribute(d, unique_attribute);
		getchar();
		std::string taggg = "title";
		unique_id_vector(d, taggg);
		getchar();
		std::string add_element = "reference";
		std::string add_child = "review";
		adding_child(d, add_element, add_child);
		getchar();
		std::string remove_element = "LectureNote";
		std::string remove_child = "comment";
		removing_child(d, remove_element, remove_child);
		getchar();
		std::string elem = "LectureNote";
		elementDetails(d, elem);
		getchar();
		printReq9();
		std::vector<std::pair<std::string, std::string>> at = adAttr();
		std::string tag_ = "page";
		std::pair<std::string, std::string> pair1("papercolor", "'pure_white'");
		at.push_back(pair1);
		addAttribute(d, at, tag_);
		dstr.display("\n\n Deleting the second attribute given above\n");
		deleteAttribute(d, pair1, tag_);
		getchar();
	}
	catch (std::exception e)
	{
		Display dstr;
		dstr.display(e.what());
	}
}

//writing string to a file
void write(XmlDocument d)
{
	Display dstr;
	using sPtr = std::shared_ptr < AbstractXmlElement >;
	dstr.display("Writing the contents of resultant xml tree to file\n");
	std::ofstream file;
	file.open("result.xml");
	if (d.getpdocElement()!=NULL)
	file << d.getpdocElement()->toString();
	file.close();
	dstr.display("\n###############################################################\n");
}

// Starting of the program
int main(int argc, char* argv[])
{
	try{
		Display dstr;
		using sPtr = std::shared_ptr < AbstractXmlElement >;
		if (argc < 3)
		{
			dstr.display("\n  please enter name of file or string as first argument and source type as second argument to process on command line\n\n");
			return 1;
		}
		static std::string xmldata("<?xml version=\"1.0\" encoding=\"utf - 8\"?><!--XML test case --><LectureNote course =\"CSE681\"><title>XML Example #1< / title><reference><title>Programming Microsoft.Net< / title><author>Jeff Prosise <note Company = 'Wintellect'>< / note>< / author><publisher>Microsoft Press< / publisher><date>2002< / date><page>608< / page>< / reference><comment>Description of PCDATA< / comment>< / LectureNote>");
		std::string srctype = argv[2];

		if (srctype=="newtree")
		{
			buildNewTree();
			return 1;
		}
		else if (srctype=="file")
		{
			std::ifstream myfile;
			myfile.open(argv[1]);
			if (myfile.is_open())
			{
				XmlDocument d(argv[1], argv[2]);
				operations(d);
				write(d);
				return 1;
			}
			else
			{
				dstr.display("\n\n file unable to open\n\n");
				myfile.close();
				return 0;
			}
	    }
		else
		{
			dstr.display("\n Demonstrating requirements for the given input type:string\n");
			XmlDocument d(argv[1], argv[2]);
			operations(d);
			write(d);
			return 1;
		}
	}
	catch (std::exception e)
	{std::cout << "Exception :" << e.what();
	}
}