#ifndef functions_h
#define functions_h

#include "glut.h"
#include <stdlib.h>
#include "Load_BMP.h"
#include <iostream>
#include "math.h"
#include "time.h"
#include "string.h"
//==============��������=====================
//ȫ����//
const float pi = 3.14159265358;
#define GAMETIME 30//��Ϸʱ������λ��s��
bool FullScreen = 0;//ȫ���ж�
float main_size = 100;//��պд�С
int win_x, win_y;//��Ļ��Ⱥ͸߶�
int displayMOD = 1;//1,��ʼ���� 2,��Ϸ���� 3,��������
int gameMOD = 1;//1,�̶�λ��ģʽ 2,���ƶ�ģʽ
int t_time = 1;//ȡֵ��Χ1-600000,600sһ������,1=1ms.ȫ�ּ�ʱ��Ҫ�ã�
int game_time = -1;//��Ϸʱ��1-60000,һ����һ����Ϸ
bool game_over = 0;//��Ϸ�������ж�
//����//
int HitNum = 0;
int MissNum = 0;
int MaxHitNum = 0;
//���//
static int mouse_x = 0, mouse_y = 0;
float mouse_speed = 0.1;
bool mouse_in = 1;//�ж��������Ļ�ڲ�
//���β�������//
float c_x = 0.0, c_y = 50.0, c_z = 80, c_angle = 90.0;//���λ��//���Ĺ۲�
float eye_y = 0;//���Ǹ߶�
float eye_angle = 0;//����
float speed = 10;//�ƶ��ٶ�
 //׼��������������//
bool shooted = 0;
float shoot_x, shoot_y, shoot_z;//��¼���ʱ׼��λ��
int shoot_time = -1;
float aim_x, aim_y, aim_z;//׼����������
//����//
struct Box
{
	//���Ӳ���
	float size;
	float x, y, z;
	float dropSpeed;
	//��ը����
	bool explosion;//�����ж�
	float ex, ey, ez;//��ըʱ������
	int exp_time;//��ըʱ��㣬-1��ʾδ��ը
};
float box_basic_Speed = 0.1;
float box_angle = 0;
#define NBOX 5
Box xbox[NBOX];
//��������//
BMPPic* Pic_Ground;
BMPPic* Pic_Sky[5];
BMPPic* Pic_Box;
GLuint textPic[7];
//===================��������=======================
//���ܺ���
void resetPosition(int game_MOD);//����λ�����ӽ�
void initBox();//��ʼ��ȫ������
void reInitBox(int i);//���³�ʼ�����Ϊi�ĺ���
void reStart();//��Ϸ���¿�ʼ
void gameOver();//��Ϸ����
bool inLine(float bx, float by, float bz, float a);//�ж�����
void initFunc();//��ʼ����Ҫ����

//����
void initTex();
//��ͼ
void drawSky(float sky_size);//��պ�
void drawBox(float box_size);//����
void drawAimPoint(float size, float distance, float angle_C, float angle_Y);//׼��
void drawShooting(int dt);//�������
void drawExplosion();//��ը��Ӧ
void drawHitNum(float x, float y, float z);//��ʾ���и���
void drawMaxHitNum(float x, float y, float z);//��ʾ��¼
void drawMissNumber(float x, float y, float z);//��ʾ��©����
void drawLastTime(float x, float y, float z);//��ʾʣ��ʱ��
//reshape��display
void reshape(int w, int h);
void display_begin();
void display_over();
void display();
//�˵�
void processMenu(int value);
void creatMenu();
//��ʱ��
void timerProc(int id);
//���
void mouseButton(int button, int state, int x, int y);
void mouseEntry(int state);
void mousePassiveMove(int x, int y);
//����
void processNormalKeys(unsigned char key, int x, int y);
void pressKey(int key, int x, int y);

#endif
