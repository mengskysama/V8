#pragma once

//从内存创建Icon，iconSize为要提取的icon的大小，-1为第一个，如果找不到所要的大小则提取第一个
HICON MakeIconFromBuf(LPBYTE pBuf, LONG lSize, LONG iconSize = -1);

//从内存创建Cursor
HCURSOR MakeCursorFromBuf(LPBYTE pBuf, LONG lSize);
