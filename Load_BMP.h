#ifndef load_bmp_h
#define load_bmp_h

//ReadBitMap
//
#include <string.h> 
#include <math.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <malloc.h>
//修改
//#include <Windows.h>


#define   WIDTHBYTES(bits) (((bits)+31)/32*4)

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;


//位图文件头信息结构定义
//其中不包含文件类型信息（由于结构体的内存结构决定，要是加了的话将不能正确读取文件信息）

typedef struct tagBITMAPFILEHEADER_x {
	DWORD bfSize;           //文件大小
	WORD   bfReserved1; 	//保留字，不考虑
	WORD   bfReserved2; 	//保留字，同上
	DWORD bfOffBits;        //实际位图数据的偏移字节数，即前三个部分长度之和
} BITMAPFILEHEADER_x;
//typedef tagBITMAPFILEHEADER BITMAPFILEHEADER;

//信息头BITMAPINFOHEADER，也是一个结构，其定义如下：

typedef struct tagBITMAPINFOHEADER_x {
	//public:
	DWORD   biSize;         	//指定此结构体的长度，为40
	LONG    biWidth;       		//位图宽
	LONG    biHeight;       	//位图高
	WORD    biPlanes;       	//平面数，为1
	WORD    biBitCount;     	//采用颜色位数，可以是1，2，4，8，16，24，新的可以是32
	DWORD   biCompression;  	//压缩方式，可以是0，1，2，其中0表示不压缩
	DWORD   biSizeImage;    	//实际位图数据占用的字节数
	LONG    biXPelsPerMeter;	//X方向分辨率
	LONG    biYPelsPerMeter;	//Y方向分辨率
	DWORD   biClrUsed;      	//使用的颜色数，如果为0，则表示默认值(2^颜色位数)
	DWORD   biClrImportant; 	//重要颜色数，如果为0，则表示所有颜色都是重要的
} BITMAPINFOHEADER_x;
//typedef tagBITMAPINFOHEADER BITMAPINFOHEADER;


//调色板Palette，当然，这里是对那些需要调色板的位图文件而言的。24位和32位是不需要调色板的。
//（似乎是调色板结构体个数等于使用的颜色数。）

typedef struct tagRGBQUAD_x {
	//public:
	BYTE     rgbBlue; //该颜色的蓝色分量
	BYTE     rgbGreen; //该颜色的绿色分量
	BYTE     rgbRed; //该颜色的红色分量
	BYTE     rgbReserved; //保留值
} RGBQUAD_x;
//typedef tagRGBQUAD RGBQUAD;

typedef struct
{
	WORD    iwidth;    //位图宽
	WORD    iheight;   //位图高
	BYTE    *pByte;		//图片数据指针
}BMPPic;

//void showBmpHead(BITMAPFILEHEADER* pBmpHead);
void showBmpInforHead(tagBITMAPINFOHEADER_x* pBmpInforHead);
//void showRgbQuan(tagRGBQUAD_x* pRGB);
BMPPic *ZXCLoadBMP(const char *szFileName);


//===============================================

void showBmpInforHead(tagBITMAPINFOHEADER_x* pBmpInforHead)
{
	printf("位图信息头:\n");
	printf("结构体的长度:%d\n", pBmpInforHead->biSize);
	printf("位图宽:%d\n", pBmpInforHead->biWidth);
	printf("位图高:%d\n", pBmpInforHead->biHeight);
	printf("biPlanes平面数:%d\n", pBmpInforHead->biPlanes);
	printf("biBitCount采用颜色位数:%d\n", pBmpInforHead->biBitCount);
	printf("压缩方式:%d\n", pBmpInforHead->biCompression);
	printf("biSizeImage实际位图数据占用的字节数:%d\n", pBmpInforHead->biSizeImage);
	printf("X方向分辨率:%d\n", pBmpInforHead->biXPelsPerMeter);
	printf("Y方向分辨率:%d\n", pBmpInforHead->biYPelsPerMeter);
	printf("使用的颜色数:%d\n", pBmpInforHead->biClrUsed);
	printf("重要颜色数:%d\n", pBmpInforHead->biClrImportant);
}


BMPPic *ZXCLoadBMP(const char *szFileName)
{
	static BMPPic pic = { 0,0,NULL };
	BITMAPFILEHEADER_x   bitHead;
	BITMAPINFOHEADER_x bitInfoHead;
	FILE* pfile;

	//char strFile[50];
	//printf("please input the .bmp file name:\n");
	//scanf("%s",strFile);

	//pfile = fopen(szFileName, "rb");//打开文件
	fopen_s(&pfile, szFileName, "rb");

	if (pfile != NULL)
	{
		// printf("file bkwood.bmp open success.\n");
		//读取位图文件头信息
		WORD fileType;
		fread(&fileType, 1, sizeof(WORD), pfile);
		if (fileType != 0x4d42)
		{
			//printf("file is not .bmp file!");
			return NULL;
		}
		//fseek(pfile,2,SEEK_CUR);   // "BM"
		fread(&bitHead, 1, sizeof(tagBITMAPFILEHEADER_x), pfile);

		//showBmpHead(&bitHead);
		printf("\n\n");

		//读取位图信息头信息
		fread(&bitInfoHead, 1, sizeof(BITMAPINFOHEADER_x), pfile);
		//showBmpInforHead(&bitInfoHead);
		printf("\n");
	}
	else
	{
		printf("file open fail!\n");
		return NULL;
	}


	tagRGBQUAD_x *pRgb;

	if (bitInfoHead.biBitCount < 24)//有调色板 *******************************************************
	{
		//读取调色盘结信息
		long nPlantNum = long(pow(2, double(bitInfoHead.biBitCount)));    //   Mix color Plant Number;
		pRgb = (tagRGBQUAD_x *)malloc(nPlantNum*sizeof(tagRGBQUAD_x));
		memset(pRgb, 0, nPlantNum*sizeof(tagRGBQUAD_x));
		int num = fread(pRgb, 4, nPlantNum, pfile);

		printf("Color Plate Number: %d\n", nPlantNum);

		printf("颜色板信息:\n");
		for (int i = 0; i < nPlantNum; i++)
		{
			if (i % 5 == 0)
			{
				printf("\n");
			}
			//showRgbQuan(&pRgb[i]);
		}
		printf("\n");
	}

	// 24位、32位位图*********************************************************************************
	int width = bitInfoHead.biWidth;
	int height = bitInfoHead.biHeight;
	pic.iwidth = width;
	pic.iheight = height;

	//分配内存空间把源图存入内存   
	//	int l_width   = WIDTHBYTES(width* bitInfoHead.biBitCount);//计算位图的实际宽度并确保它为32的倍数,看BMPLoad.h中的#define，实现4字节对齐!!!!!!!
	int l_width = WIDTHBYTES(width)*height;//计算位图的实际宽度并确保它为32的倍数,看BMPLoad.h中的#define，实现4字节对齐!!!!!!!
	pic.pByte = (BYTE *)malloc(height*l_width);
	memset(pic.pByte, 0, height*l_width);
	long nData = height*l_width;

	//把位图数据信息读到数组里   
	fread(pic.pByte, 1, nData, pfile);

	return &pic;
}


#endif