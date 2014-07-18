#include "IRender.h"

class CIRenderImpl:public IRender
{
public:
	CIRenderImpl();
	~CIRenderImpl();

public:
	virtual ICommonRender* CreateRender(HWND hWnd,unsigned int width,unsigned int height,IRender::En_Used_Order& m_en_render);
	virtual void DestroyRender(void);
	
	virtual void RegisterRender(void);
private:
	volatile bool m_isRenderRegister;//是否已经注册过Render
};