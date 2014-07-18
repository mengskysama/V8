#pragma once
#include <WTypes.h>

class CVideoEffect
{
public:
	//拉普拉斯锐化
	void SharpLaplacianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, int db=11);

protected:
	inline BYTE** CreatImage(BYTE* image,BYTE** buf, unsigned int width, unsigned int height);//获取源图像每行开始位置
	int TempltExcuteCl(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y, int cn);//拉普拉斯算法模版
private:
	BYTE* m_ImageBufSrc[480]; //源图像行开始位置
	BYTE* m_ImageBufDst[480]; //目标图像行开始位置
};