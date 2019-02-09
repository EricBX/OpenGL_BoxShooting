#ifndef functions_h
#define functions_h

#include "glut.h"
#include <stdlib.h>
#include "Load_BMP.h"
#include <iostream>
#include "math.h"
#include "time.h"
#include "string.h"
//==============变量声明=====================
//全局量//
const float pi = 3.14159265358;
#define GAMETIME 30//游戏时长（单位：s）
bool FullScreen = 0;//全屏判定
float main_size = 100;//天空盒大小
int win_x, win_y;//屏幕宽度和高度
int displayMOD = 1;//1,开始界面 2,游戏界面 3,结束界面
int gameMOD = 1;//1,固定位置模式 2,可移动模式
int t_time = 1;//取值范围1-600000,600s一个周期,1=1ms.全局计时都要用！
int game_time = -1;//游戏时间1-60000,一分钟一场游戏
bool game_over = 0;//游戏结束的判定
//积分//
int HitNum = 0;
int MissNum = 0;
int MaxHitNum = 0;
//鼠标//
static int mouse_x = 0, mouse_y = 0;
float mouse_speed = 0.1;
bool mouse_in = 1;//判断鼠标在屏幕内部
//漫游参数设置//
float c_x = 0.0, c_y = 50.0, c_z = 80, c_angle = 90.0;//相机位置//中心观察
float eye_y = 0;//仰角高度
float eye_angle = 0;//仰角
float speed = 10;//移动速度
 //准心与射击弹轨参数//
bool shooted = 0;
float shoot_x, shoot_y, shoot_z;//记录射击时准心位置
int shoot_time = -1;
float aim_x, aim_y, aim_z;//准心中心坐标
//盒子//
struct Box
{
	//盒子参数
	float size;
	float x, y, z;
	float dropSpeed;
	//爆炸参数
	bool explosion;//命中判定
	float ex, ey, ez;//爆炸时的坐标
	int exp_time;//爆炸时间点，-1表示未爆炸
};
float box_basic_Speed = 0.1;
float box_angle = 0;
#define NBOX 5
Box xbox[NBOX];
//纹理设置//
BMPPic* Pic_Ground;
BMPPic* Pic_Sky[5];
BMPPic* Pic_Box;
GLuint textPic[7];
//===================函数声明=======================
//功能函数
void resetPosition(int game_MOD);//重置位置与视角
void initBox();//初始化全部盒子
void reInitBox(int i);//重新初始化标号为i的盒子
void reStart();//游戏重新开始
void gameOver();//游戏结束
bool inLine(float bx, float by, float bz, float a);//判定命中
void initFunc();//初始化重要函数

//纹理
void initTex();
//绘图
void drawSky(float sky_size);//天空盒
void drawBox(float box_size);//盒子
void drawAimPoint(float size, float distance, float angle_C, float angle_Y);//准心
void drawShooting(int dt);//射击动画
void drawExplosion();//爆炸反应
void drawHitNum(float x, float y, float z);//显示命中个数
void drawMaxHitNum(float x, float y, float z);//显示纪录
void drawMissNumber(float x, float y, float z);//显示遗漏个数
void drawLastTime(float x, float y, float z);//显示剩余时间
//reshape和display
void reshape(int w, int h);
void display_begin();
void display_over();
void display();
//菜单
void processMenu(int value);
void creatMenu();
//计时器
void timerProc(int id);
//鼠标
void mouseButton(int button, int state, int x, int y);
void mouseEntry(int state);
void mousePassiveMove(int x, int y);
//键盘
void processNormalKeys(unsigned char key, int x, int y);
void pressKey(int key, int x, int y);

#endif
