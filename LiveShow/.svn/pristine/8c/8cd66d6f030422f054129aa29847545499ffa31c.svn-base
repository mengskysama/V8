#include "stdafx.h"
#include "ExpatHallTree.h"
#include "utility\StringHelper.h"

CExpatHallTree::CExpatHallTree()
{
	std::wstring strXml = common::utility::systemhelper::Get179AppPath();
	m_resPath.Format(L"%s",strXml.c_str());	
}

CExpatHallTree::~CExpatHallTree()
{
	Unload();

	for(std::map<CString, HBITMAP>::iterator it = m_path2Bitmap.begin();
		it != m_path2Bitmap.end(); ++it)
	{
		DeleteObject(it->second);
	}
	m_path2Bitmap.clear();
}

bool CExpatHallTree::Load()
{
	common::utility::expat_document xmlDoc;
	CString xmlPath = m_resPath + XML_HALLTREE_FILE;
	bool bSucc = xmlDoc.load_from_file(xmlPath);
	assert(bSucc);
	if(!bSucc)
		return false;

	Unload();

	bool parok1 = Parse(xmlDoc, L"HallRoom", m_HallNode);	
	if(parok1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//释放目录树
void CExpatHallTree::Unload()
{
	ClearNode(m_HallNode);
}

 //解释roomName目录树
/*
目录树大至格式
<TopRoom ID="3" name="179KTV" PeopleNum="1112" PicContract="179KTV1" PicExpand="179KTV2" >
	<ChildRoom ID="4" ChannelID="3" Name="KTV推荐" PeopleNum="" Url="http://client.179.com/room/roomlist?channel=1" PicItem="KTVpush1" PicItemSelect="KTVpush2"/>
	<ChildRoom ID="5" ChannelID="3" Name="浪漫点歌" PeopleNum="" Url="http://client.179.com/room/roomlist?channel=1" PicItem="RomanticSong1" PicItemSelect="RomanticSong2"/>
	<ChildRoom ID="6" ChannelID="3" Name="岁月留声" PeopleNum="" Url="http://client.179.com/room/roomlist?channel=1" PicItem="KTVSound1" PicItemSelect="KTVSound2"/>
</TopRoom>
*/

bool CExpatHallTree::Parse(common::utility::expat_document& xmlDoc,
						   std::wstring roomName,std::list<hallNode*> &hallnode)
{
	bool treeok = true;  //下一个孩子结点
	common::utility::expat_element*	pTiXmlElement = xmlDoc.get_root();
	common::utility::expat_element* pFrontElement = pTiXmlElement;
	std::vector<std::wstring> roomVec;

	if(!pFrontElement)
	{
		return false;
	}

	std::wstring lowNodeName = common::utility::stringhelper::MakeLower(std::wstring(XML_ROOMNODE_NAME)); //转小写
	roomVec =  common::utility::stringhelper::Split_String_by_Delimiter(lowNodeName, ',');

	pTiXmlElement=pTiXmlElement->get_element_by_name(roomName);  //获取房间开始结点
	treeok = true; //找下一个孩子结点

	if(!pTiXmlElement)
	{
		return false;
	}

	common::utility:: expat_attribute *hallAttr;
	std::wstring elvalue;
	CString strPath;
	std::list<hallNode*> *pNode = &hallnode;
	std::list<hallNode*> pCurNode; //当前房间结点路径

	while(pTiXmlElement)
	{
		pFrontElement = pTiXmlElement;			
		pTiXmlElement = pTiXmlElement->get_next_element(treeok); //下一个孩子结点或兄结点	

		if(pTiXmlElement)
		{
			std::wstring elname = pTiXmlElement->get_element_name();
			std::wstring elnameL = common::utility::stringhelper::MakeLower(elname);	//转小写		

			bool roomOK = false;
			for(int i=0;i<roomVec.size();i++)
			{
				if(elnameL == roomVec[i])
				{
					roomOK = true;					
					break;
				}
			}

			if(roomOK)   //有新的房间结点
			{
				if(elnameL == L"toproom")
				{
					hallNode *newRoom = new hallNode;   //添加第一层房间				

					hallnode.push_back(newRoom);				
					pCurNode.push_back(newRoom);

					pNode =  &hallnode;
				}
				else
				{
					hallNode *CurRoom = pCurNode.back();
					hallNode *newRoom = new hallNode;	  //给上一层房间添加子房间

					CurRoom->m_Child.push_back(newRoom);
					pCurNode.push_back(newRoom);
					pNode = &CurRoom->m_Child;
				}	

				//获取房间结点的属性值
				hallNode *CurRoom = pNode->back();  				

				hallAttr = pTiXmlElement->get_attribute_by_name(_T("ID"));
				if(hallAttr != NULL)
				{
					elvalue = hallAttr->get_attribute_value();
					CurRoom->m_ID = _wtoi(elvalue.c_str());
				}
				
				hallAttr = pTiXmlElement->get_attribute_by_name(_T("Name"));
				if(hallAttr != NULL)
				{
					elvalue = hallAttr->get_attribute_value();
					CurRoom->m_Name.Format(L"%s",elvalue.c_str());
				}
				
				hallAttr = pTiXmlElement->get_attribute_by_name(_T("PeopleNum"));
				if(hallAttr != NULL)
				{
					elvalue = hallAttr->get_attribute_value();
					if(elvalue.size() > 0)
					{
						CurRoom->m_PeopleNum.Format(_T("%s人"),elvalue.c_str());
					}
					
				}
				
				hallAttr = pTiXmlElement->get_attribute_by_name(_T("PicExpand"));
				if(hallAttr != NULL)
				{
					elvalue = hallAttr->get_attribute_value();
					CurRoom->m_pngNormalExpand.Format(L"%s%s\\%s.PNG",m_resPath,ROOM_PNG_DIR,elvalue.c_str());
				}
				
				hallAttr = pTiXmlElement->get_attribute_by_name(_T("PicContract"));
				if(hallAttr != NULL)
				{
					elvalue = hallAttr->get_attribute_value();
					CurRoom->m_pngSelectContract.Format(L"%s%s\\%s.PNG",m_resPath,ROOM_PNG_DIR,elvalue.c_str());
				}
				
				hallAttr = pTiXmlElement->get_attribute_by_name(_T("PicItem"));
				if(hallAttr != NULL)
				{
					elvalue = hallAttr->get_attribute_value();
					CurRoom->m_pngNormalExpand.Format(L"%s%s\\%s.PNG",m_resPath,ROOM_PNG_DIR,elvalue.c_str());
				}

				hallAttr = pTiXmlElement->get_attribute_by_name(_T("PicItemSelect"));
				if(hallAttr != NULL)
				{
					elvalue = hallAttr->get_attribute_value();
					CurRoom->m_pngSelectContract.Format(L"%s%s\\%s.PNG",m_resPath,ROOM_PNG_DIR,elvalue.c_str());
				}
				
				hallAttr = pTiXmlElement->get_attribute_by_name(_T("Url"));
				if(hallAttr != NULL)
				{
					elvalue = hallAttr->get_attribute_value();
					CurRoom->m_Url.Format(L"%s",elvalue.c_str());
				}

				hallAttr = pTiXmlElement->get_attribute_by_name(_T("NewUrl"));
				if(hallAttr != NULL)
				{
					elvalue = hallAttr->get_attribute_value();
					CurRoom->m_NewUrl = (elvalue == L"true" ? true : false);
				}
			}
		}

		if(!pTiXmlElement && treeok)  //无孩子结点时，找下一个兄弟结点
		{
			pTiXmlElement=pFrontElement;
			treeok = false; //找下一个兄弟结点		
			if(pCurNode.size()>0)
			{
				pCurNode.pop_back();
			}
		}
		else if(!pTiXmlElement && !treeok)	//无下一个兄弟结点时，找父结点的下一个兄弟结点
		{
			pTiXmlElement=pFrontElement->m_parent;
			treeok = false; //找父结点的下一个兄弟结点
			if(pCurNode.size()>0)
			{
				pCurNode.pop_back();
			}

			if(pTiXmlElement->get_element_name() == roomName)  //房间结点结束
			{
				break;
			}			
		}
		else
		{
			treeok = true;
		}
	}

	return true;
}

HBITMAP CExpatHallTree::LoadImage(CString strPath)
{
	std::map<CString, HBITMAP>::iterator it = m_path2Bitmap.find(strPath);

	if(it != m_path2Bitmap.end())
	{
		return it->second;
	}

	strPath = m_resPath + strPath;

	HBITMAP bitmap = (HBITMAP)::LoadImage(AfxGetResourceHandle(),
		strPath.GetBuffer(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	m_path2Bitmap[strPath] = bitmap;

	return bitmap;
}

//递归删除树
void CExpatHallTree::ClearNode(std::list<hallNode*> &RoomNode)
{
	std::list<hallNode*> ::iterator iter;

	iter = RoomNode.begin();
	while(iter != RoomNode.end())
	{	
		hallNode* curRoom = *iter;		
		if(curRoom->m_Child.size()>0)
		{	
			ClearNode(curRoom->m_Child);
			
		}
		delete curRoom;
		iter++;

	}

	RoomNode.clear();

}
