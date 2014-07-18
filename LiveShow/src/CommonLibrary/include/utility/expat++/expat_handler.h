#ifndef XHANDLER_H_ALEIX0211250023
#define XHANDLER_H_ALEIX0211250023

#include "../../thirdlibrary/expat/expat.h"

namespace common { namespace utility {

/* Expat callback for XML declaration. */
void
xmldecl_handler(void* data, XML_Char const* version, XML_Char const* encoding,
				int standalone);

/* Expat callback for starting elements. */
void
start_handler(void* data, XML_Char const* elem, XML_Char const** attr);

/* Expat callback for ending elements. */
void
end_handler(void* data, XML_Char const* elem);

/* Expat callback for element contents. */
void
char_handler(void* data, XML_Char const* s, int len);

#endif /* XHANDLER_H_ALEIX0211250023 */

}}