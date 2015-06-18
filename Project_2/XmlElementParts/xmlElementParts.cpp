/////////////////////////////////////////////////////////////////////
//  XmlElementParts.cpp - Collects tokens for XML analysis         //
//  ver 1.3                                                        //
//                                                                 //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr2,Spring 2015            //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//                 Extended by: venkata Karthikeya Jangal          // 
//                 vjangal@syr.edu                                 //
/////////////////////////////////////////////////////////////////////
#include <algorithm>
#include "XmlElementParts.h"
using namespace XmlProcessing;
using sPtr = std::shared_ptr<AbstractXmlElement>;
//----< construct XmlParts instance >---------------------------

XmlParts::XmlParts(Toker* pTokr) : pToker(pTokr), Verbose(false)
{

}
//----< destructor >-------------------------------------------

XmlParts::~XmlParts()
{
}
//----< is this a terminating token? >-------------------------

bool XmlParts::isTerminator(std::string tok)
{
  if(tok.length() == 0)
    return false;
  if(tok == ">")
    return true;
  return false;
}
//
//----< collect an XmlElementParts sequence >------------------

bool XmlParts::get()
{
  toks.clear();
  std::string tok;
  do
  {
    if(pToker->isFileEnd())
    {
      if(length() > 0)
        return true;
      return false;
    }
    tok = pToker->getTok();
    if(Verbose)
    {
      if(tok != "\n")
        std::cout << "\n--tok=" << tok << std::endl;
      else
        std::cout << "\n--tok=newline\n";
    }
    if(toks.size() > 0 && tok == "<")
    {
      pToker->putback('<');
      break;
    }
    if(tok != "\n")
      toks.push_back(tok);
  } while(!isTerminator(tok));
  return true;
}
//----< index operator >---------------------------------------

std::string& XmlParts::operator[](int n)
{
  if(n < 0 || toks.size() <= (size_t)n)
    throw std::exception("XmlParts index out of range");
  return toks[n];
}
//----< collect semi-expression as space-seperated string >----

std::string XmlParts::show()
{
  if(toks.size() == 0)
    return "";
  std::string temp(" ");
  for(size_t i=0; i<toks.size(); ++i)
    if(toks[i] != "\n")
      temp.append(" ").append(toks[i]);
  return temp;
}
//
//----< is tok found in XmlElementParts? >---------------------

int XmlParts::find(const std::string& tok)
{
  for(int i=0; i<length(); ++i)
    if(tok == toks[i])
      return i;
  return length();
}

bool XmlParts::finding(const std::string& tok1, const std::string& tok2)
{
	for (int i = 0; i < length(); ++i)
	{
		if (tok1 == toks[i])
		{
			if (tok2==toks[i+1])
			return true;
		}
	}
	return false;
}

// testing if it is a declaration element

bool XmlParts::isDeclaration(const std::string& str)
{
	std::size_t found = str.find("< ?");
	if (found != std::string::npos)
		return true;
	else
		return false;
}

// testing if it is a comment element

bool XmlParts::isComment(const std::string& str)
{
	std::size_t found = str.find("< ! --");
	if (found != std::string::npos)
		return true;
	else
		return false;
}

// testing if it is a starting of a tagged element

bool XmlParts::isStartingOfTag(const std::string& str)
{
	std::string tok1 = "<";
	std::vector<std::string> tokens;
	for (int i = 0; i < length(); ++i)
	{
		if (tok1 == toks[i])
		{
					if (toks[i + 1] != "?" && toks[i + 1] != "!" && toks[i + 1] != "/")
					return true;
		}
	}
	return false;
}

// testing if it is a ending of a tagged  element

bool XmlParts::isEndingOfTag(const std::string& str)
{
	for (int i = 0; i < length(); ++i)
	{
		std::string k = "<";
		std::string l = "/";

		if (k == toks[i] && l == toks[i + 1])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

// testing if it is a declare element
std::vector<std::string> XmlParts::declareElements(const std::string& s)
{
	std::vector<std::string> kkr;

	for (int i = 0; i < length(); ++i)
	{
		std::string k = "=";

		if (k == toks[i])
		{
			kkr.push_back(toks[i - 1]);
			kkr.push_back(toks[i + 1]);
			int p = i + 1;
			for (p; p < length(); ++p)
			{
				if (k == toks[p])
				{
					kkr.push_back(toks[i + 2]);
					kkr.push_back(toks[i + 4]);
					return kkr;
				}
			}	
		}


	}
	return kkr;
}

//Collects attricbutes for tags
sPtr XmlParts::collects(const std::string& str)
{
	std::vector<std::string> kk;
	sPtr node = makeTaggedElement(toks[1]);
	for (size_t i = 0; i < toks.size(); ++i)
	{
		std::string k = "=";

		if (k == toks[i])
		{
			node->addAttrib(toks[i-1],toks[i+1]);
		}
	}
	return node;
}



//----< remove tok if found in semi-expression >---------------

bool XmlParts::remove(const std::string& tok)
{
  std::vector<std::string>::iterator it;
  it = std::find(toks.begin(),toks.end(),tok);
  if(it != toks.end())
  {
    toks.erase(it);
    return true;
  }
  return false;
}
//----< remove leading newlines >------------------------------

void XmlParts::trimFront()
{
  while(toks.size() > 0 && toks[0] == "\n")
    remove("\n");
}
//----< make tokens lowercase >--------------------------------

void XmlParts::toLower()
{
  for(int i=0; i<length(); ++i)
  {
    for(size_t j=0; j<(*this)[i].length(); ++j)
    {
      (*this)[i][j] = tolower((*this)[i][j]);
    }
  }
}
//
//----< test stub >--------------------------------------------

#ifdef TEST_XMLELEMENTPARTS

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing XmlParts class\n "
            << std::string(23,'=') << std::endl;
  std::cout
    << "\n  Note that quotes are returned as single tokens\n\n";

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-') << "\n\n";
    try
    {
      Toker toker(argv[i]);
      toker.setMode(Toker::xml);
      XmlParts parts(&toker);
    //parts.verbose();                 // uncomment to show token details
      while(parts.get())
        std::cout << parts.show().c_str() << std::endl;
      std::cout << "\n\n";
    }
    catch(std::exception ex)
    {
      std::cout << "\n  " << ex.what() << "\n\n";
    }
  }
}

#endif
