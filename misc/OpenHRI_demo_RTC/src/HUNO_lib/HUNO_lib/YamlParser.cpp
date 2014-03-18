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
#include "stdafx.h"
#include "YamlParser.h"


/**** YamlNode **/
YamlNode::YamlNode():parentNode(NULL){
	type = YAML_NONE;
}

YamlNode::~YamlNode(){
	int len = childNodes.size();
	for(int i(0);i<len;i++){
		delete childNodes[i];
	}
	childNodes.clear();
}

void 
YamlNode::addChild(YamlNode *n){
	childNodes.push_back(n);
	n->parentNode = this;
}

void 
YamlNode::print()
{
	switch(type){
		case YAML_SCALAR:
			{
				YamlScalar *v = reinterpret_cast<YamlScalar *>(this);
				v->print();
			}
			break;
		case YAML_MAPPING:
			{
				YamlMapping *v = reinterpret_cast<YamlMapping *>(this);
				v->print();
			}
			break;
		case YAML_SEQUENCE:
			{
				YamlSequence *v = reinterpret_cast<YamlSequence *>(this);
				v->print();
			}
			break;
		default:
			std::cerr << "<ROOT>" << std::endl;
			break;
	}

	int len = childNodes.size();
	for(int i(0); i<len;i++){
		std::cerr << "  " ;
		childNodes[i]->print();
	}
}

YamlScalar *
YamlNode::toScalar()
{
	if(type == YAML_SCALAR){
		return reinterpret_cast<YamlScalar *>(this);
	}

	return NULL;
}

YamlMapping *
YamlNode::toMapping()
{
	if(type == YAML_MAPPING){
		return reinterpret_cast<YamlMapping *>(this);
	}

	return NULL;
}

YamlSequence *
YamlNode::toSequence()
{
	if(type == YAML_SEQUENCE){
		return reinterpret_cast<YamlSequence *>(this);
	}

	return NULL;
}

bool
YamlNode::isScalar()
{
	if(type == YAML_SCALAR){
		return true;
	}
	return false;
}

bool
YamlNode::isMapping()
{
	if(type == YAML_MAPPING){
		return true;
	}
	return false;
}

bool
YamlNode::isSequence()
{
	if(type == YAML_SEQUENCE){
		return true;
	}
	return false;
}

bool
YamlNode::isRoot()
{
	if(type == YAML_NONE && parentNode == NULL){
		return true;
	}
	return false;
}

bool
YamlNode::equalTo(char *v)
{
	if(isScalar()){
		if(toScalar()->toString() == v){
			return true;
		}
	}
	return false;
}
/**** YamlScalar **/

YamlScalar::YamlScalar(char *str){
	type = YAML_SCALAR;
	setValue(str);
}

YamlScalar::~YamlScalar(){
	
}

void
YamlScalar::setValue(char *str){
	value = std::string(str);
}

void
YamlScalar::print()
{
	std::cerr << value << " ";
}

std::string 
YamlScalar::toString()
{
	return value;
}

int
YamlScalar::toInteger()
{
	std::istrstream istr(value.data());
	int i;

	istr >> i;
	
	return i;
}

double
YamlScalar::toDouble()
{
	std::istrstream istr(value.data());
	double d;

	istr >> d;
	return d;
}


/**** YamlMapping **/
YamlMapping::YamlMapping(){
	type = YAML_MAPPING;
}


YamlMapping::~YamlMapping(){
	std::map<std::string, YamlNode *>::iterator it = values.begin();
	while( it != values.end())
	{
		delete (*it).second;
		++it;
	}
	values.clear();
}

void
YamlMapping::insertValue(char *k, YamlNode *v){
	values.insert( std::map<std::string, YamlNode *>::value_type(std::string(k), v));
}

void
YamlMapping::print(){
	std::map<std::string, YamlNode *>::iterator it = values.begin();
	std::cerr << "{";
	while( it != values.end())
	{
		std::cerr << (*it).first << ":";
		(*it).second->print();
		++it;
	}
	std::cerr << "}" << std::endl;
}

YamlNode *
YamlMapping::getValue(char *k){
	std::map<std::string, YamlNode *>::iterator it = values.begin();
	while( it != values.end())
	{
		if( (*it).first  == k){
		  return (*it).second;
		}
		++it;
	}

	return NULL;
}


bool 
YamlMapping::equalTo(char *k, char *val){
	YamlNode *n = getValue(k);
	if(n != NULL){
		return n->equalTo(val);
	}
	return false;
}

YamlScalar *
YamlMapping::getScalar(char *k)
{
	YamlNode *n = getValue(k);
	if(n != NULL){
		return n->toScalar();
	}
	return NULL; 
}

YamlMapping *
YamlMapping::getMapping(char *k)
{
	YamlNode *n = getValue(k);
	if(n != NULL){
		return n->toMapping();
	}
	return NULL; 
}

YamlSequence *
YamlMapping::getSequence(char *k)
{
	YamlNode *n = getValue(k);
	if(n != NULL){
		return n->toSequence();
	}
	return NULL; 
}

/**** YamlSequence **/
YamlSequence::YamlSequence(){
	type = YAML_SEQUENCE;
}

YamlSequence::~YamlSequence(){
	int len = value.size();
	for(int i(0); i<len;i++){
		delete value[i];
	}
	value.clear();
}

void
YamlSequence::appendValue(YamlNode *n){
	value.push_back(n);
}

void
YamlSequence::print()
{
	int len = value.size();
	std::cerr << "(" << len << ")" << "[" ;
	for(int i(0); i<len;i++){
		std::cerr << "  " ;
		value[i]->print();
	}
	std::cerr << "]" << std::endl ;
}

int
YamlSequence::size()
{
	return value.size();
}

YamlNode *
YamlSequence::at(int x)
{
	try{
		return value.at(x);
	}catch(std::out_of_range& e){
		return NULL;
	}
}

YamlScalar *
YamlSequence::getScalarAt(int x)
{
	try{
		YamlNode *n = value.at(x);
		return n->toScalar();
	}catch(std::out_of_range& e){
		return NULL;
	}
}

YamlMapping *
YamlSequence::getMappingAt(int x)
{
	try{
		YamlNode *n = value.at(x);
		return n->toMapping();
	}catch(std::out_of_range& e){
		return NULL;
	}
}
YamlSequence *
YamlSequence::getSequenceAt(int x)
{
	try{
		YamlNode *n = value.at(x);
		return n->toSequence();
	}catch(std::out_of_range& e){
		return NULL;
	}
}



/**** Yaml Document ***/
YamlDocument::YamlDocument():root(NULL){

}

YamlDocument::~YamlDocument(){
	delete root;
}


bool
YamlDocument::load(char *fname)
{
	
	FILE *fh = fopen(fname, "rb");


	if(fh == NULL){
		std::cerr << "Fail to open file: " << fname <<  std::endl;
		return false;
	}

	if(!yaml_parser_initialize(&parser)){
		std::cerr << "Fail to initialize parser." << std::endl;
		fclose(fh);
		return false;
	}
	yaml_parser_set_input_file(&parser, fh);

	do{
		yaml_parser_parse(&parser, &event);
		switch(event.type){
		case YAML_STREAM_START_EVENT:
			break;
		case YAML_STREAM_END_EVENT:
			break;
		case YAML_DOCUMENT_START_EVENT:
			root = ParseYamlDoc();
			break;
		case YAML_DOCUMENT_END_EVENT:
			break;
		default:
			std::cerr << "YAML Format error." << std::endl;
			break;
		}
		if(event.type != YAML_STREAM_END_EVENT){
	  		yaml_event_delete(&event);
		}
	}while(event.type != YAML_STREAM_END_EVENT);

	yaml_event_delete(&event);
	yaml_parser_delete(&parser);
	fclose(fh);

	return (root != NULL);
}

YamlSequence *
YamlDocument::ParseYamlSequence()
{
	YamlSequence * ret = new YamlSequence();

	yaml_event_delete(&event);
	do{
		yaml_parser_parse(&parser, &event);
		switch(event.type)
		{
		case YAML_SEQUENCE_START_EVENT:
			{
				YamlSequence *v;
				v = ParseYamlSequence();
				ret->appendValue(reinterpret_cast<YamlNode *>(v));
			}
			break;

		case YAML_SEQUENCE_END_EVENT:
			break;

		case YAML_MAPPING_START_EVENT:
			{
				YamlMapping *v;
				v=ParseYamlMapping();
				ret->appendValue(reinterpret_cast<YamlNode *>(v));
			}
			break;

		case YAML_MAPPING_END_EVENT:
			break;

		case YAML_SCALAR_EVENT:
			{
				YamlScalar *v = new YamlScalar((char *)event.data.scalar.value );
				ret->appendValue(reinterpret_cast<YamlNode *>(v));
			}
			break;
		case YAML_NO_EVENT:
			break;
		case YAML_ALIAS_EVENT:
			break;
		default:
			std::cerr << "FORMAT ERROR in Sequence." << std::endl;
			break;
		}
		if(event.type != YAML_SEQUENCE_END_EVENT){
			yaml_event_delete(&event);
		}
	}while(event.type !=  YAML_SEQUENCE_END_EVENT);

	yaml_event_delete(&event);
	return ret;
}

YamlMapping *
YamlDocument::ParseYamlMapping()
{
	YamlMapping *ret= new YamlMapping();
	int key=0;
	char * keyVal;

	yaml_event_delete(&event);

	do{
		yaml_parser_parse(&parser, &event);
		switch(event.type)
		{
		case YAML_SEQUENCE_START_EVENT:
			{
				if(key == 0){
					std::cerr << "Format ERROR: Invalid KEY" << std::endl;
				}
				YamlSequence *v;
				v=ParseYamlSequence();
				ret->insertValue(keyVal, reinterpret_cast<YamlNode *>(v));
				key = 0;
			}
			break;
		case YAML_MAPPING_START_EVENT:
			{
				if(key == 0){
					std::cerr << "Format ERROR: Invalid KEY" << std::endl;
					return ret;
				}
				YamlMapping *v;
				v = ParseYamlMapping();
				ret->insertValue(keyVal, reinterpret_cast<YamlNode *>(v));
				key = 0;
			}
			break;

		case YAML_MAPPING_END_EVENT:
			break;

		case YAML_SCALAR_EVENT:
			if(key == 0){
				keyVal = (char *)strdup((char *)event.data.scalar.value);
				key = 1;
			}else{
				YamlScalar *v= new YamlScalar((char *)event.data.scalar.value);
				ret->insertValue(keyVal, reinterpret_cast<YamlNode *>(v));
				key = 0;
			}
			break;
		default:

			break;
		}
		if(event.type != YAML_MAPPING_END_EVENT){
			yaml_event_delete(&event);
		}
	}while(event.type != YAML_MAPPING_END_EVENT);
	yaml_event_delete(&event);
	return ret;

}

YamlNode *
YamlDocument::ParseYamlDoc()
{
	YamlNode *root = new YamlNode();

	yaml_event_delete(&event);

	do{
		yaml_parser_parse(&parser, &event);
		switch(event.type){
		case YAML_STREAM_END_EVENT:
		case YAML_DOCUMENT_END_EVENT:
			return root;

		case YAML_SEQUENCE_START_EVENT:
			{
				YamlSequence *v;
				v = ParseYamlSequence();
				root->addChild(reinterpret_cast<YamlNode *>(v));
			}
			break;
		case YAML_SEQUENCE_END_EVENT:
			break;
		case YAML_MAPPING_START_EVENT:
			{
				YamlMapping *v;
				v=ParseYamlMapping();
				root->addChild(reinterpret_cast<YamlNode *>(v));
			}
			break;
		case YAML_MAPPING_END_EVENT:
			break;

		case YAML_SCALAR_EVENT:
			{
				YamlScalar *v = new YamlScalar((char *)event.data.scalar.value );
				root->addChild(reinterpret_cast<YamlNode *>(v));
			}
			break;
		default:
			std::cout << "FORMAT ERROR" << std::endl;
			break;
		}
		if(event.type != YAML_DOCUMENT_END_EVENT){
			yaml_event_delete(&event);
		}
	}while(event.type != YAML_DOCUMENT_END_EVENT);

	yaml_event_delete(&event);

	return root;
}


void
YamlDocument::print()
{
	root->print();
}

bool 
YamlDocument::empty() 
{
	if(root == NULL || root->childNodes.empty()){
		return true;
	}
	return false;
}