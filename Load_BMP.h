#ifndef load_bmp_h
#define load_bmp_h

//ReadBitMap
//
#include <string.h> 
#include <math.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <malloc.h>
//�޸�
//#include <Windows.h>


#define   WIDTHBYTES(bits) (((bits)+31)/32*4)

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;


//λͼ�ļ�ͷ��Ϣ�ṹ����
//���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��

typedef struct tagBITMAPFILEHEADER_x {
	DWORD bfSize;           //�ļ���С
	WORD   bfReserved1; 	//�����֣�������
	WORD   bfReserved2; 	//�����֣�ͬ��
	DWORD bfOffBits;        //ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��
} BITMAPFILEHEADER_x;
//typedef tagBITMAPFILEHEADER BITMAPFILEHEADER;

//��ϢͷBITMAPINFOHEADER��Ҳ��һ���ṹ���䶨�����£�

typedef struct tagBITMAPINFOHEADER_x {
	//public:
	DWORD   biSize;         	//ָ���˽ṹ��ĳ��ȣ�Ϊ40
	LONG    biWidth;       		//λͼ��
	LONG    biHeight;       	//λͼ��
	WORD    biPlanes;       	//ƽ������Ϊ1
	WORD    biBitCount;     	//������ɫλ����������1��2��4��8��16��24���µĿ�����32
	DWORD   biCompression;  	//ѹ����ʽ��������0��1��2������0��ʾ��ѹ��
	DWORD   biSizeImage;    	//ʵ��λͼ����ռ�õ��ֽ���
	LONG    biXPelsPerMeter;	//X����ֱ���
	LONG    biYPelsPerMeter;	//Y����ֱ���
	DWORD   biClrUsed;      	//ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)
	DWORD   biClrImportant; 	//��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��
} BITMAPINFOHEADER_x;
//typedef tagBITMAPINFOHEADER BITMAPINFOHEADER;


//��ɫ��Palette����Ȼ�������Ƕ���Щ��Ҫ��ɫ���λͼ�ļ����Եġ�24λ��32λ�ǲ���Ҫ��ɫ��ġ�
//���ƺ��ǵ�ɫ��ṹ���������ʹ�õ���ɫ������

typedef struct tagRGBQUAD_x {
	//public:
	BYTE     rgbBlue; //����ɫ����ɫ����
	BYTE     rgbGreen; //����ɫ����ɫ����
	BYTE     rgbRed; //����ɫ�ĺ�ɫ����
	BYTE     rgbReserved; //����ֵ
} RGBQUAD_x;
//typedef tagRGBQUAD RGBQUAD;

typedef struct
{
	WORD    iwidth;    //λͼ��
	WORD    iheight;   //λͼ��
	BYTE    *pByte;		//ͼƬ����ָ��
}BMPPic;

//void showBmpHead(BITMAPFILEHEADER* pBmpHead);
void showBmpInforHead(tagBITMAPINFOHEADER_x* pBmpInforHead);
//void showRgbQuan(tagRGBQUAD_x* pRGB);
BMPPic *ZXCLoadBMP(const char *szFileName);


//===============================================

void showBmpInforHead(tagBITMAPINFOHEADER_x* pBmpInforHead)
{
	printf("λͼ��Ϣͷ:\n");
	printf("�ṹ��ĳ���:%d\n", pBmpInforHead->biSize);
	printf("λͼ��:%d\n", pBmpInforHead->biWidth);
	printf("λͼ��:%d\n", pBmpInforHead->biHeight);
	printf("biPlanesƽ����:%d\n", pBmpInforHead->biPlanes);
	printf("biBitCount������ɫλ��:%d\n", pBmpInforHead->biBitCount);
	printf("ѹ����ʽ:%d\n", pBmpInforHead->biCompression);
	printf("biSizeImageʵ��λͼ����ռ�õ��ֽ���:%d\n", pBmpInforHead->biSizeImage);
	printf("X����ֱ���:%d\n", pBmpInforHead->biXPelsPerMeter);
	printf("Y����ֱ���:%d\n", pBmpInforHead->biYPelsPerMeter);
	printf("ʹ�õ���ɫ��:%d\n", pBmpInforHead->biClrUsed);
	printf("��Ҫ��ɫ��:%d\n", pBmpInforHead->biClrImportant);
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

	//pfile = fopen(szFileName, "rb");//���ļ�
	fopen_s(&pfile, szFileName, "rb");

	if (pfile != NULL)
	{
		// printf("file bkwood.bmp open success.\n");
		//��ȡλͼ�ļ�ͷ��Ϣ
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

		//��ȡλͼ��Ϣͷ��Ϣ
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

	if (bitInfoHead.biBitCount < 24)//�е�ɫ�� *******************************************************
	{
		//��ȡ��ɫ�̽���Ϣ
		long nPlantNum = long(pow(2, double(bitInfoHead.biBitCount)));    //   Mix color Plant Number;
		pRgb = (tagRGBQUAD_x *)malloc(nPlantNum*sizeof(tagRGBQUAD_x));
		memset(pRgb, 0, nPlantNum*sizeof(tagRGBQUAD_x));
		int num = fread(pRgb, 4, nPlantNum, pfile);

		printf("Color Plate Number: %d\n", nPlantNum);

		printf("��ɫ����Ϣ:\n");
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

	// 24λ��32λλͼ*********************************************************************************
	int width = bitInfoHead.biWidth;
	int height = bitInfoHead.biHeight;
	pic.iwidth = width;
	pic.iheight = height;

	//�����ڴ�ռ��Դͼ�����ڴ�   
	//	int l_width   = WIDTHBYTES(width* bitInfoHead.biBitCount);//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���,��BMPLoad.h�е�#define��ʵ��4�ֽڶ���!!!!!!!
	int l_width = WIDTHBYTES(width)*height;//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���,��BMPLoad.h�е�#define��ʵ��4�ֽڶ���!!!!!!!
	pic.pByte = (BYTE *)malloc(height*l_width);
	memset(pic.pByte, 0, height*l_width);
	long nData = height*l_width;

	//��λͼ������Ϣ����������   
	fread(pic.pByte, 1, nData, pfile);

	return &pic;
}


#endif