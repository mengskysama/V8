/**
* NetLayer.h
* 
* @version 1.0 (2010-1-11)
* @author plushu
*
*/
#pragma  once

#include "..\Common.h"
#include "ConnectionLayer.h"

namespace netlayer
{
	/* 
	* @return 返回网络组件单例
	*/

	COMMON_INTERFACE netlayer::ConnectionLayer* GetConnectionLayer();

	/* 
	* @return 销毁网络组件单例
	*/

	COMMON_INTERFACE void DestroyConnectionLayer();
}
