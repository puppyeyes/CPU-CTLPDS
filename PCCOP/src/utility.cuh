/*
 * utility.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */
#include <stdio.h>
#include "stdlib.h"
#include <map>
#include <string>
#include <assert.h>
#include <cuda_util.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <iostream>

#ifndef UTILITY_H_
#define UTILITY_H_
using namespace std;

void parseXML(const char *file_name);

int parse_abpds_xml(const char *file_name);

extern map<string,int> state_mp;
extern map<string,int> stack_mp;

#endif /* UTILITY_H_ */
