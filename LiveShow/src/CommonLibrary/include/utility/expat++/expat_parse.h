#pragma once

#include "../../thirdlibrary/expat/expat.h"

namespace common { namespace utility {


class expat_tree;
class expat_element;
class expat_parse;

/**
* Callback function type.
*
* @return 1 if callback call had no errors, 0 otherwise.
*/
typedef unsigned int SCEW_CALLBACK(expat_parse* parser);


/* Stack to keep previous parsed elements */
typedef struct _stack_element
{
	expat_element* element;
	struct _stack_element* prev;
} stack_element;

class expat_parse
{
public:
	expat_parse(void);
	~expat_parse(void);


private:

	XML_Parser parser;      /* Expat parser */
	expat_tree* tree;        /* XML document tree */
	expat_tree* current;  /* Current parsed element */
	stack_element* stack;   /* Current parsed element stack */
	int ignore_whitespaces; /* 1 if ignore white spaces, 0 otherwise */
	SCEW_CALLBACK* stream_callback; /* Function to call while reading streams */
};


}}