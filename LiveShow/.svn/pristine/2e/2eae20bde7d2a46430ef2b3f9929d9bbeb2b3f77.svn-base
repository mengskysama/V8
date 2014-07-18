#include "StdAfx.h"
#include "utility\expat++\expat_parse.h"
#include "utility\expat++\expat_handler.h"

namespace common { namespace utility {

expat_parse::expat_parse(void)
{
	parser = XML_ParserCreate(NULL);

	/* initialize Expat handlers */
	XML_SetXmlDeclHandler(parser, xmldecl_handler);
	XML_SetElementHandler(parser, start_handler, end_handler);
	XML_SetCharacterDataHandler(parser, char_handler);
	XML_SetUserData(parser, this);

	/* ignore white spaces by default */
	ignore_whitespaces = 1;

	/* no callback by default */
	stream_callback = NULL;
}

expat_parse::~expat_parse(void)
{
	if( parser)
	{
		XML_ParserFree( parser);
		parser = NULL;
	}
}


}}