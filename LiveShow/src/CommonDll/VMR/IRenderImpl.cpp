#include "stdafx.h"
#include "IRender.h"
#include "Common.h"
#include "IRenderImpl.h"
#include "RenderFactory.h"
#include "GDIRender.h"
#include "DDrawRender.h"
#include "GDIRender.h"
#include "DDrawRender.h"
#include "GDIPlusRender.h"
#include "MyAssert.h"

CRenderFactory g_RenderFactory;
IRender::ERenderImplMap IRender::m_mapRenderImpl;
CIRenderImpl g_RenderImpl;

CIRenderImpl::CIRenderImpl()
:IRender()
,m_isRenderRegister(false)
{
	m_mapRenderImpl.insert(IRender::ERenderImplMap::value_type(ERI_COMM_IMPL,this));
}

CIRenderImpl::~CIRenderImpl()
{
	m_mapRenderImpl.erase(m_mapRenderImpl.find(ERI_COMM_IMPL));
}

ICommonRender* CIRenderImpl::CreateRender(HWND hWnd,unsigned int width,unsigned int height,IRender::En_Used_Order& m_en_render)
{
	if (!m_isRenderRegister)
	{
		RegisterRender();
	}
	ICommonRender* pCommRender = NULL; //Render实例
	//创建DDraw Render
// 	SAFE_DELETE(pCommRender);
// 	pCommRender = g_RenderFactory.CreateRender(IRender::ER_DDR_RENDER,hWnd,width,height);
// 	if (pCommRender)
// 	{
// 		ErrTrace("当前使用IRender::ER_DDR_RENDER\n");
// 		m_en_render = ER_DDR_RENDER;
// 		return pCommRender;
// 	}
	
	//创建GDIPlus Render
	SAFE_DELETE(pCommRender);
	pCommRender = g_RenderFactory.CreateRender(IRender::ER_GDP_RENDER,hWnd,width,height);
	if (pCommRender)
	{
		ErrTrace("当前使用IRender::ER_GDP_RENDER\n");
		m_en_render = ER_GDP_RENDER;
		return pCommRender;
	}

	//创建GDI Render
	SAFE_DELETE(pCommRender);
	pCommRender = g_RenderFactory.CreateRender(IRender::ER_GDI_RENDER,hWnd,width,height);
	if (pCommRender)
	{
		ErrTrace("当前使用IRender::ER_GDI_RENDER\n");
		m_en_render = ER_GDI_RENDER;
		return pCommRender;
	}
	
	return NULL;
}

void CIRenderImpl::DestroyRender(void)
{
}

//注册支持的Render
void CIRenderImpl::RegisterRender(void)
{
	struct REGISTERRENDER
	{
		IRender::En_Used_Order uo;
		CRenderFactory::RenderCreateCallback pCallback;
	};

	REGISTERRENDER Entries [] = {
								{IRender::ER_DDR_RENDER,  &DDrawRender::CreateRender},
								{IRender::ER_GDI_RENDER,  &GDIRender::CreateRender},
								{IRender::ER_GDP_RENDER,  &GDIPlusRender::CreateRender}
								} ;

	for ( int i = 0 ; i < sizeof(Entries) / sizeof(REGISTERRENDER) ; i++ )
	{
		g_RenderFactory.RegisterRender ( Entries[i].uo, Entries[i].pCallback ) ;
	}

	m_isRenderRegister = true;
}