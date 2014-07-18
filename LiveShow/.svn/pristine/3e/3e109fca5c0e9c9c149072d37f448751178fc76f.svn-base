#pragma once
#include "Common.h"
#include <Windows.h>
#include <map>

//Render Comm接口
class COMMON_OUT_CLASS ICommonRender
{
public:
	virtual ~ICommonRender(){}
	virtual bool Create(HWND hWnd, long lWidth, long lHeight) = 0;
	virtual bool PutData(unsigned char* pData, long lWidth, long lHeight,int index=0) = 0;
	virtual bool BeginPaint()=0;
	virtual bool Paint(HWND hWnd,HDC hDC,int x,int y,int index=0,RECT* rc=NULL) = 0;
	virtual bool EndPaint()=0;
};


//DDraw Render接口
class IDDrawRender:public ICommonRender
{
public:

};

//GDI Render接口
class IGDIRender:public ICommonRender
{
public:

};

//GDI+ Render接口
class IGDIPlusRender:public ICommonRender
{
public:

};

//VMR Render接口
class IVMRRender:public ICommonRender
{
public:

};

//Render接口
class COMMON_OUT_CLASS IRender
{
public:
	enum En_Used_Order
	{
		ER_DDR_RENDER = 0, //ddraw render
		ER_GDP_RENDER = 1, //GDI+ render
		ER_GDI_RENDER = 2, //GDI render
		ER_NUM_RENDER = 3  //render 数量
	};

	enum En_Render_Impl
	{
		ERI_COMM_IMPL = 0
	};
public:
	IRender(){}
	virtual ~IRender(){}
	
	virtual ICommonRender* CreateRender(HWND hWnd,unsigned int width,unsigned int height,IRender::En_Used_Order& m_en_render)= 0;
	virtual void DestroyRender(void) = 0;

	static inline IRender* GetInstance(En_Render_Impl rip=ERI_COMM_IMPL){
		ERenderImplMap::iterator iter = m_mapRenderImpl.find(rip);
		return (iter!=m_mapRenderImpl.end())?iter->second:NULL;
	}
protected:
	typedef std::map<En_Render_Impl, IRender*> ERenderImplMap ;
	static ERenderImplMap m_mapRenderImpl;

};