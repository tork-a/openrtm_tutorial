/*
    YmalParser for SerialRobot

    Copyright(C) 2011, Isao HARA, AIST,Japan

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
	are permitted provided that the following conditions are met:

      Redistributions of source code must retain the above copyright notice, 
	    this list of conditions and the following disclaimer.
      Redistributions in binary form must reproduce the above copyright notice, 
	    this list of conditions and the following disclaimer in the documentation
		and/or other materials provided with the distribution.
      Neither the name of the AIST,Japan nor the names of its contributors may be
	    used to endorse or promote products derived from this software without
		specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
	OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
	AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
	OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
	OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __YAML_PARSER_H__
#define __YAML_PARSER_H__

#include <iostream>
#include <strstream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <yaml.h>


enum YamlNodeType{
	YAML_NONE, YAML_SCALAR, YAML_MAPPING, YAML_SEQUENCE,
};

class YamlScalar;
class YamlMapping;
class YamlSequence;
class YamlNode;
class YamlDocument;

class YamlNode
{
public:
	YamlNode();
	~YamlNode();

	void addChild(YamlNode *n);
	void print();

	YamlScalar *toScalar();
	YamlMapping *toMapping();
	YamlSequence *toSequence();

	bool isScalar();
	bool isMapping();
	bool isSequence();
    bool isRoot();

	bool equalTo(char *str);

public:
	enum YamlNodeType type;
	std::vector<YamlNode *> childNodes;
	YamlNode *parentNode;

};


class YamlScalar:  YamlNode
{
public:
	YamlScalar(char *str);
	~YamlScalar();

	void setValue(char *str);
	void print();

	std::string toString();
	int toInteger();
	double toDouble();

public:
	std::string value;

};


class YamlMapping:  YamlNode
{
public:
	YamlMapping();
	~YamlMapping();

	void insertValue(char *k, YamlNode *v);
	void print();

	YamlNode *getValue(char *k);

	bool equalTo(char *k, char *val);
	YamlScalar *getScalar(char *k);
	YamlMapping *getMapping(char *k);
	YamlSequence *getSequence(char *k);

	YamlNode * operator[](char *k) { return getValue(k); }

public:
	std::map<std::string, YamlNode *> values;

};

class YamlSequence:  YamlNode
{
public:
	YamlSequence();
	~YamlSequence();

	void appendValue(YamlNode *v);
	void print();

	int size();
	YamlNode *at(int x);
	YamlScalar *getScalarAt(int x);
	YamlMapping *getMappingAt(int x);
	YamlSequence *getSequenceAt(int x);

	YamlNode * operator[](int n) { return value[n]; }

public:
	std::vector<YamlNode *> value;

};



class YamlDocument
{
public:
	YamlDocument();
	~YamlDocument();

public:
	bool load(char *fname);
	void print();
	bool empty();

private:
	YamlSequence *ParseYamlSequence();
	YamlMapping *ParseYamlMapping();
	YamlNode *ParseYamlDoc();

public:
	YamlNode *root;

private:
	yaml_parser_t parser;
	yaml_event_t event;
};



#endif