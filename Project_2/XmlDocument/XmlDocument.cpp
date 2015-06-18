///////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a container of XmlElement nodes             //
// Ver 1.2                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Jim Fawcett, CST 4-187, 443-3948                 //
//              jfawcett@twcny.rr.com                            //
///////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//  XmlDocument.cpp - extension by adding functions                //
//  ver 1.3                                                        //
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
* Abstract Syntax Trees, defined in this package, are unordered trees with
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes


Public Interface:
=================
std::shared_ptr < AbstractXmlElement > DFS_r_ltr(sPtr pNode,std::string tagname); //depth first search
std::vector<sPtr> XmlDocument::element_vector(sPtr pNode, std::string tag);       //collects elements of particuolar vector
sPtr find_attribute(sPtr pNode, std::string tag);                                 //return pointer to the searched attribute

Required files
- tokenizer.h, xmlelementparts.h,xmlelement.h

Maintenance History:
====================
ver 1.3 : 24 Mar 15
-Added functions: findTextinXmlelement
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "XmlDocument.h"
#include "../XmlElementParts/Tokenizer.h"
#include "../XmlElementParts/XmlElementParts.h"
#include "../XmlElement/XmlElement.h"
#include <stack>          // std::stack

using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;

// Finding the text in xml comment
std::string XmlProcessing::XmlDocument::findTextInXmlComment(const std::string& s)
{
	std::size_t found1 = s.find("! --");
	std::size_t found3 = s.find("-- >");
	std::size_t found2 = found1 + 4;
	std::string comment;
	std::size_t k = found3 - found2;
	comment = s.substr(found2, k);
	return comment;
}

//processing declaration elements
void XmlProcessing::XmlDocument::processingDeclaration(std::string& part, XmlParts& parts, sPtr& dec)
{
	std::vector<std::string> DeclareElements;
	DeclareElements = parts.declareElements(part);
	dec = makeXmlDeclarElement();
	if (dec == NULL)
		return;
	DeclareElements = parts.declareElements(part);
	int l = 0;
	dec->addAttrib(DeclareElements[0], DeclareElements[1]);
	dec->addAttrib(DeclareElements[2], DeclareElements[3]);
}

//Procesing stating of the tag
void XmlProcessing::XmlDocument::processingStartingTag(std::string& part, XmlParts& parts, std::stack<sPtr>& s1)
{
	//std::vector<std::string> col_ = parts.collects(part);
	sPtr node = parts.collects(part);

	if (!s1.empty())
	{
		XmlDocument::sPtr elem = s1.top();
		elem->addChild(node);
	}
	s1.push(node);
}


XmlProcessing::XmlDocument& XmlDocument::operator = (XmlDocument&& document)
{
	if (this != &document)
	{
		pDocElement_ = NULL;         // AST that holds procInstr, comments, XML root, and more comments
		found_.clear();
		pDocElement_ = document.pDocElement_;
		found_ = document.found_;
		document.pDocElement_ = NULL;
		document.found_.clear();
	}
	return *this;
}

//Move constructor to move xml from one to another.
XmlDocument::XmlDocument(XmlDocument&& document)
{
	pDocElement_ = NULL;         // AST that holds procInstr, comments, XML root, and more comments
	found_.clear();
	pDocElement_ = document.pDocElement_;
	found_ = document.found_;
	document.pDocElement_ = NULL;
	document.found_.clear();
}




//BUilding a tree read from file or string and printing to outstream
void XmlProcessing::XmlDocument::print(const std::string& s,bool condition)
{
	Display dstr;
	Toker toker(s,condition);
	toker.setMode(Toker::xml);
	XmlParts parts(&toker);
	std::stack<XmlDocument::sPtr> s1;
	sPtr dec, commElem;
	while (parts.get())
	{
		std::string part = parts.show();
		if (parts.isDeclaration(part))
		{
			processingDeclaration(part,parts,dec);
		}
		else if (parts.isComment(part))
		{
			std::string comment = findTextInXmlComment(part);
			commElem = makeCommentElement(part);
		}
		else{
			if (parts.isStartingOfTag(part))
			{
				processingStartingTag(part,parts,s1);
			}
			else if (parts.isEndingOfTag(part))
			{
				if (s1.size() == 1)
				{
					XmlDocument::sPtr root = s1.top();
					sPtr doc1 = makeDocElement(root, dec, commElem);
					dstr.display(doc1->toString());
					 pDocElement_= doc1;
				}
				s1.pop();
			}
			else
			{
				XmlDocument::sPtr textElem = XmlProcessing::makeTextElement(part);
				if (s1.size() != NULL)
				{
					XmlDocument::sPtr elem = s1.top();
					elem->addChild(textElem);
				}
			}
		}
	}
}




// Searching fot the tag and returning pointer to that tag
sPtr XmlDocument::DFS_r_ltr(sPtr pNode,std::string tag)
{
	std::stack<sPtr> nodeStack;
	nodeStack.push(pNode);
	sPtr node;
	while (nodeStack.size() > 0)
	{
		sPtr pCurrentNode = nodeStack.top();
 		nodeStack.pop();

		if (pCurrentNode == NULL)
			return node;
		if (pCurrentNode->value().compare(tag) == 0)
		{
			return pCurrentNode;
		}
		size_t numChildren = pCurrentNode->children().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodeStack.push(pCurrentNode->children()[numChildren - i - 1]);
	}
	return node;
}


//returns pointer to the tag which has searched attribute
sPtr XmlDocument::find_attribute(sPtr pNode, std::string attr)
{
	sPtr elements;
	std::stack<sPtr> nodeStack;
	nodeStack.push(pNode);
	std::string attrib = attr;
	while (nodeStack.size() > 0)
	{
		sPtr pCurrentNode = nodeStack.top();
		nodeStack.pop();
			
		if (pCurrentNode == NULL)
			return elements;
		std::vector<std::pair<std::string, std::string>> attributes=pCurrentNode->getAttribs();

			if (attributes.size() > 0)
			{
				for (size_t i = 0; i < attributes.size(); ++i)
				{
					if (attributes.at(i).first == attrib)
					{
						elements = pCurrentNode;
						return elements;
					}
				}

			}

		size_t numChildren = pCurrentNode->children().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodeStack.push(pCurrentNode->children()[numChildren - i - 1]);
	}
	return elements;
}

//returns vector of attribute which has particular tag
std::vector<std::pair<std::string, std::string>> XmlDocument::attributeChildren(sPtr pNode, std::string tag)
{
	std::vector<std::pair<std::string, std::string>> elements;
	std::string attrib = tag;
	std::stack<sPtr> nodeStack;
	nodeStack.push(pNode);
	while (nodeStack.size() > 0)
	{
		sPtr pCurrentNode = nodeStack.top();
		nodeStack.pop();
		if (pCurrentNode == NULL)
			return elements;
		if (pCurrentNode->value().compare(tag) == 0)
		{
		 elements = pCurrentNode->getAttribs();
		}
		size_t numChildren = pCurrentNode->children().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodeStack.push(pCurrentNode->children()[numChildren - i - 1]);
	}
	return elements;
}


//returns vector of pointers to children of particular tag
std::vector<sPtr> XmlDocument::childrenAttribute(sPtr pNode, std::string tag)
{
	std::vector<sPtr> elements;
	std::string attrib = tag;
	std::stack<sPtr> nodeStack;
	nodeStack.push(pNode);
	while (nodeStack.size() > 0)
	{
		sPtr pCurrentNode = nodeStack.top();
		nodeStack.pop();

		if (pCurrentNode == NULL)
			return elements;
		if (pCurrentNode->value().compare(tag) == 0)
		{
			elements = pCurrentNode->children();
		}
		size_t numChildren = pCurrentNode->children().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodeStack.push(pCurrentNode->children()[numChildren - i - 1]);
	}
	return elements;
}




// Returns  vector of collection of pointers with specific tag name in a tree
std::vector<sPtr> XmlDocument::element_vector(sPtr pNode, std::string tag)
{
	std::vector<sPtr> elements;
	std::stack<sPtr> nodeStack;
	nodeStack.push(pNode);
	while (nodeStack.size() > 0)
	{
		sPtr pCurrentNode = nodeStack.top();
		nodeStack.pop();
		if (pCurrentNode == NULL)
			return elements;

		if (pCurrentNode->value().compare(tag) == 0)
		{
			elements.push_back(pCurrentNode);
		}
		size_t numChildren = pCurrentNode->children().size();
		for (size_t i = 0; i < numChildren; ++i)
			nodeStack.push(pCurrentNode->children()[numChildren - i - 1]);
	}
	return elements;
}

//constructor for Xml document
XmlProcessing::XmlDocument::XmlDocument(const std::string& src, std::string srcType)
{
	Display dstr;
	dstr.display("\n\n File is not empty \n\n");
	std::string lm = "\n  BUilding tree \n " + std::string(23, '=') + "\n";
	dstr.display(lm);
	dstr.display("\n  Processing file :");
	dstr.display("\n");
	sPtr global;
	try
	{
		if (srcType.compare("string") != 0)
		{
			print(src, true);
		}
		else
		{
			print(src, false);			
		}
		dstr.display("\n\n finished printing tree \n");
}
	catch (std::exception ex)
	{
		dstr.display(ex.what());
	}
}


#ifdef TEST_XMLDOCUMENT
int main()
{
  title("Testing XmlDocument class");
  XmlDocument doc("LectureNote.xml","true");
  std::cout << "\n\n";
}
#endif