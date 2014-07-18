#include "VideoEffect.h"

#define BIT_WIDTH 3
#define BOUND(x,mn,mx) ((x)<(mn)?(mn):((x)>(mx)?(mx):(x)))

inline BYTE** CVideoEffect::CreatImage(BYTE* image,BYTE** buf, unsigned int width, unsigned int height)//获取图像每行开始位置
{
	BYTE** imageBuf = buf;
	for(int y=0; y<height; y++)
	{
		imageBuf[y] = image+y*width*3; //使imageBuf中每个指针分别指向其下标表示的行的行首地址
	}
	return imageBuf;
}

int CVideoEffect::TempltExcuteCl(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y, int cn)//拉普拉斯算法模版
{
	int i,j;                      
	int m=0;//用来存放加权和
	int px,py;   
	int tw_t = tw >> 1;
	//依次对邻域中每个像素进行运算
	for(i=0; i<tw; i++)
	{
		for(j=0; j<tw; j++)
		{
			py=y-tw_t+i;//计算对应模板上位置的像素在原图像中的位置
			px=x-tw_t+j;
			m+=imageBuf0[py][px*BIT_WIDTH+cn] * templt[i*tw+j];//加权求和
		}
	}
	return m;                
}

static int templtlp[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
void CVideoEffect::SharpLaplacianCl(BYTE* image0, BYTE*   image1, unsigned int w, unsigned int h, int db)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0,m_ImageBufSrc, w, h);
	BYTE** imageBuf1 = CreatImage(image1,m_ImageBufDst, w, h);

	int a = 0;
	int index,index1;
	int m1,m2,m3;
	//依次对原图像的每个像素进行处理
	for(int y=1; y<h-1; y++)
	{
		for(int x=1; x<w-1; x++)
		{
			
			m2 = x  * BIT_WIDTH;
			m1 = m2 - BIT_WIDTH;
			m3 = m2 + BIT_WIDTH;

			index = m2;
			int n1 = y-1;
			int n2 = y+1;

			{
				//i=0  a+=imageBuf0[y-1+i][(x-1+j)*BIT_WIDTH+c] * templtlp[i*3+j];//加权求和
				a-=imageBuf0[n1][m1];
				a-=imageBuf0[n1][m2];
				a-=imageBuf0[n1][m3];
				//i=1
				a-=imageBuf0[y][m1];
				a+=imageBuf0[y][m2] * templtlp[4];
				a-=imageBuf0[y][m3];
				//i=2
				a-=imageBuf0[n2][m1];
				a-=imageBuf0[n2][m2];
				a-=imageBuf0[n2][m3];

				a/= db;
				a=imageBuf0[y][index]+a;//对中心像素进行增强
				a = BOUND(a,0,255);
				imageBuf1[y][index]=a;
			}

			++index; ++m1; ++m2; ++m3;
			{
				//i=0  a+=imageBuf0[y-1+i][(x-1+j)*BIT_WIDTH+c] * templtlp[i*3+j];//加权求和
				a-=imageBuf0[n1][m1];
				a-=imageBuf0[n1][m2];
				a-=imageBuf0[n1][m3];
				//i=1
				a-=imageBuf0[y][m1];
				a+=imageBuf0[y][m2] * templtlp[4];
				a-=imageBuf0[y][m3];
				//i=2
				a-=imageBuf0[n2][m1];
				a-=imageBuf0[n2][m2];
				a-=imageBuf0[n2][m3];

				a/= db;
				a=imageBuf0[y][index]+a;//对中心像素进行增强
				a = BOUND(a,0,255);
				imageBuf1[y][index]=a;
			}
			
			++index; ++m1; ++m2; ++m3;
			{
				//i=0  a+=imageBuf0[y-1+i][(x-1+j)*BIT_WIDTH+c] * templtlp[i*3+j];//加权求和
				a-=imageBuf0[n1][m1];
				a-=imageBuf0[n1][m2];
				a-=imageBuf0[n1][m3];
				//i=1
				a-=imageBuf0[y][m1];
				a+=imageBuf0[y][m2] * templtlp[4];
				a-=imageBuf0[y][m3];
				//i=2
				a-=imageBuf0[n2][m1];
				a-=imageBuf0[n2][m2];
				a-=imageBuf0[n2][m3];

				a/= db;
				a=imageBuf0[y][index]+a;//对中心像素进行增强
				a = BOUND(a,0,255);
				imageBuf1[y][index]=a;
			}
		}
	}
}