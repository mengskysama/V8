/* 
  provide version info
*/

#ifndef _CT_ISO_LIB_INF_H_
#define _CT_ISO_LIB_INF_H_

#include "MP4OSMacros.h"

typedef struct {
  char date[20];
  char versionNo[20];
} CtIsomediaLibInfo;


MP4_EXTERN (void)
CtIsomediaGetLibInfo (CtIsomediaLibInfo* libInfo);

MP4_EXTERN (int)
CtIsomediaRequireLibVersion( const char *versionStringRequired);

#endif /* _CT_ISO_LIB_INF_H_ */
