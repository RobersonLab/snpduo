// Standard Includes
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cerrno>

// Namespace
using namespace std;

// Custom headers
#include "helper.h"
#include "output.h"

// FUNCTIONS

//////////////////////////////////////////////////
//
// Functions for command-line argument parser
//
//////////////////////////////////////////////////
CArgs::CArgs(int n, char *params[])
{
	parametercount = n;
	parsed.resize( n );
	parameters.resize( n );
	
	for (int i = 0; i < n; ++i) parsed[i] = false;
	
	parsed[0] = true; // Filename automatically returned
	
	for (int i = 0; i < n; ++i) parameters[i] = params[i];
}

void CArgs::CheckSwitches()
{
	vector<int> badoptions;
	
	for (int i = 0; i < parametercount; ++i)
	{
		if (parsed[i] == false) badoptions.push_back(i);
	}
	
	if (badoptions.size())
	{
		printLog("Unrecognized command-line switch(es):\n");
		for (unsigned int i = 0; i < badoptions.size(); ++i)
		{
			printLog(parameters[ badoptions[i] ] + "\n");
		}
		
		error("Bad command-line options selected");
	}
}

bool CArgs::Search(string s)
{
	for (int i = 0; i < parametercount; ++i)
	{
		if (parameters[i] == s)
		{
			parsed[i] = true;
			return true;
		}
	}
	
	return false;
}

int CArgs::Index(string s)
{
	for (int i = 0; i < parametercount; ++i)
	{
		if (parameters[i] == s and i + 1 < parametercount)
		{
			parsed[i + 1] = true;
			return i + 1;
		}
	}
	
	error("Missing argument for " + s);
}

void CArgs::print()
{
	printLog("Options from command line:\n");
	for (int i = 0; i < parametercount; ++i)
	{
		printLog(parameters[i] + "\n");
	}
}

void CArgs::printArgs()
{
	printLog("Command-line Options Set:\n");
	for (unsigned int i = 0; i < clinelist.size(); ++i)
	{
		printLog( clinelist[i] + " " + clinevalue[i] + "\n");
	}
	
	printLog("\n");
}

//////////////////////////////////////////////////
//
// Accessory functions for handling errors
//
//////////////////////////////////////////////////

void error (string s)
{
	printLog("\nError: " +  s);
	
	exit(1);
}

void shutdown()
{
	exit (0);
}

//////////////////////////////////////////////////
// Accessory function for file reading:
// Reads a character at a time. Puts characters
// together in a string and returns a bool for
// whether a string was actually read
//////////////////////////////////////////////////

bool getString (ifstream &f, string &s)
{
	s = "";
	char c;
	bool read = false;
	
	while (1)
	{
		f.get(c);
		
		if (c == ' ' or c == '\t' or c == '\n' or f.eof()) break;
		else if (c == '\r') continue;
		else
		{
			s += c;
			read = true;
		}
	}
	
	return read;
}

bool getWhiteSpacePaddedString (ifstream &f, string &s, char &c)
{
	s = "";
	c = ' ';
	bool read = false;
	
	while (1)
	{
		f.get(c);
		
		if (c == '\n' or f.eof()) break;
		else if (c == '\r') continue;
		else if (c == ' ' or c == '\t')
		{
			if (read) break;
			else continue;
		}
		else
		{
			s += c;
			read = true;
		}
	}
	
	return read;
}
