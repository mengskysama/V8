#pragma once

#include "utility\expat++\expat_document.h"
#include "utility\SystemHelper.h"
#include <string>
#include <list>
#include <vector>

#define XML_HALLTREE_FILE	 L"resource\\channel"
#define XML_ROOMNODE_NAME    L"TopRoom,ChildRoom"
#define ROOM_PNG_DIR         L"resource\\halltree"

struct hallNode
{
    hallNode()
	{
		m_ID = 0;
		m_RoomID = 0;	
		m_CurSelect = false;
		m_NewUrl = false;
	}

	UINT      m_ID;
	UINT      m_RoomID;
	CString   m_PeopleNum;
	CString   m_Name;	
	CString   m_pngNormalExpand;   //有子层时为扩展时图片，无子层时为普通时图片
	CString   m_pngSelectContract; //有子层时为收缩时图片，无子层时为选中时图片
	CString   m_Url;	
	bool      m_CurSelect;
	bool	  m_NewUrl;
	std::list<hallNode*> m_Child;
};

class CExpatHallTree
{
public:
    CExpatHallTree();
	virtual ~CExpatHallTree();

	bool Load();   //加载HallTree.xml目录树的xml文件，生成目录树
	void Unload(); //释放目录树

public:
	std::list<hallNode*> m_HallNode;  //大厅目录树数据

private:
	bool Parse(common::utility::expat_document& xmlDoc, std::wstring roomName, 
		std::list<hallNode*> &hallnode);	  //解释roomName目录树
	HBITMAP LoadImage(CString strPath);
	void ClearNode(std::list<hallNode*> &RoomNode);  //递归删除树

	CString m_resPath;
	std::map<CString, HBITMAP> m_path2Bitmap;
};


