//游戏名称：疯狂的盒子
//作者：EricBX
//玩法：天空不知为何落下疯狂的盒子，请瞄准并射击天空上落下来的盒子，记分，当时间达到30s时游戏结束。。。
//功能：窗口模式下鼠标右击出菜单：切换难易程度，切换游戏模式，进入全屏模式，重新开始，退出游戏。//全屏模式下菜单不显示
//		移动鼠标可以改变视角，鼠标左键单机射击；
//		移动模式下wsad控制前后左右平移；
//		Esc键退出全屏。
//		测试专用的功能：F1强制切换Welcome界面，F2强制切换Game界面，F3强制切换Gameover界面
//		测试专用功能：任意模式下方向键↑↓控制前后移动，←→控制水平视角左右转动，PgUp和PgDn控制仰角；

#include "functions.h"

void main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(750, 500);
	glutCreateWindow("疯狂的盒子");

	//纹理初始化//
	initTex();

	//盒子初始化//
	initBox();

	//菜单//
	creatMenu();

	//功能初始化//
	initFunc();

	//进入循环//
	glutMainLoop();

}


//功能
void resetPosition(int game_MOD)
{
	c_x = 0;
	c_y = main_size / 2;
	if (game_MOD == 1)
	{
		c_z = 0.8*main_size;
		c_angle = 90;
	}
	else if (game_MOD == 2)
	{
		c_z = 0.5*main_size;
		c_angle = 270;
	}
	eye_angle = 0;
}

void initBox()//初始化全部盒子
{
	int i;
	int span = main_size;
	srand((unsigned)time(NULL));
	for (i = 0; i < NBOX; i++)          //产生随机数  
	{
		xbox[i].x = -span / 2 + rand() % span;
		xbox[i].z = -3 * span / 4 + rand() % span;
		xbox[i].y = main_size + xbox[i].size * 2;
		xbox[i].size = 4 + (float)(i / 10);
		xbox[i].dropSpeed = box_basic_Speed + (float)(rand() % 20 + i) / (float)50;
		xbox[i].explosion = 0;
		xbox[i].exp_time = -1;
		//std::cout << "\n——————————\nBOX[" << i << "]初始化完成！\n";
		//std::cout<<"坐标(x,z)=("<<xbox[i].x<<","<<xbox[i].z<<")\n——————————\n";
	}
}

void reInitBox(int i)//重新初始化标号为i的盒子
{
	int span = main_size;
	srand((unsigned)time(NULL));
	xbox[i].x = -span / 2 + rand() % span + 3 * i - 15;
	xbox[i].z = -3 * span / 4 + rand() % span - 3 * i;
	xbox[i].y = main_size + xbox[i].size * 2;
	xbox[i].dropSpeed = box_basic_Speed + (float)(rand() % 20 + i) / (float)50;
	xbox[i].explosion = 0;
	//std::cout << "\n——————————\n\tBOX["<<i<<"]重生了！\n";
	//std::cout << "坐标(x,z)=(" << xbox[i].x << "," << xbox[i].z << ")\n";
	//std::cout << "dropSpeed=" << xbox[i].dropSpeed << '\n';
	//std::cout << "——————————\n";
}

void reStart()
{
	resetPosition(gameMOD);
	HitNum = 0;
	MissNum = 0;
	initBox();
	displayMOD = 2;
	game_time = 1;
}

void gameOver()
{
	initBox();
	resetPosition(1);
	displayMOD = 3;
	for (int i = 0; i < 10; i++)
	{
		xbox[i].dropSpeed = 0;
	}
	if (HitNum > MaxHitNum)
		MaxHitNum = HitNum;
	std::cout << "\n——————————\n游\t戏\t结\t束\t！\n命中：" << HitNum << "\t丢失：" << MissNum << "\n最佳命中：" << MaxHitNum << "\n——————————\n";
}
//命中判定函数//
bool inLine(float bx, float by, float bz, float a)//判断box和aimpoint与camera连线是否共线，a为命中角度误差的余弦表示
{
	float ax, ay, az;//准心中点坐标
	float angle_C = pi*(c_angle) / 180;
	float angle_Y = pi*(eye_angle) / 180;
	ax = c_x - 2 * cos(angle_Y)* cos(angle_C);
	ay = c_y + 2 * sin(angle_Y);
	az = c_z - 2 * cos(angle_Y)*sin(angle_C);

	//勾股判定法
	float a2 = (bx - c_x)*(bx - c_x) + (by - c_y)*(by - c_y) + (bz - c_z)*(bz - c_z);
	float b2 = (ax - c_x)*(ax - c_x) + (ay - c_y)*(ay - c_y) + (az - c_z)*(az - c_z);
	float c2 = (bx - ax)*(bx - ax) + (by - ay)*(by - ay) + (bz - az)*(bz - az);
	float cos_ACB = (a2 + b2 - c2) / (2 * sqrt(a2)*sqrt(b2));
	//std::cout << "余弦=" << cos_ACB << '\n';
	if (cos_ACB >= 1 - a)
		return 1;
	else return 0;

}

//纹理
void initTex()
{
	glGenTextures(6, textPic);

	Pic_Sky[0] = ZXCLoadBMP("sky2_0.bmp");
	//Pic_Sky[0] = ZXCLoadBMP("dom_0.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[0]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Sky[0]->iwidth, Pic_Sky[0]->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Sky[0]->pByte);
	std::cout << "图像载入成功!\n图像大小为（" << Pic_Sky[0]->iwidth << "，" << Pic_Sky[0]->iheight << "）\n";
	//free(Pic_Sky[0]->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Sky[1] = ZXCLoadBMP("sky2_1.bmp");
	//Pic_Sky[1] = ZXCLoadBMP("dom_1.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[1]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Sky[1]->iwidth, Pic_Sky[1]->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Sky[1]->pByte);
	std::cout << "图像载入成功!\n图像大小为（" << Pic_Sky[0]->iwidth << "，" << Pic_Sky[0]->iheight << "）\n";
	//free(Pic_Sky[0]->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Sky[2] = ZXCLoadBMP("sky2_2.bmp");
	//Pic_Sky[2] = ZXCLoadBMP("dom_2.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[2]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Sky[2]->iwidth, Pic_Sky[2]->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Sky[2]->pByte);
	std::cout << "图像载入成功!\n图像大小为（" << Pic_Sky[0]->iwidth << "，" << Pic_Sky[0]->iheight << "）\n";
	//free(Pic_Sky[0]->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Sky[3] = ZXCLoadBMP("sky2_3.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[3]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Sky[3]->iwidth, Pic_Sky[3]->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Sky[3]->pByte);
	std::cout << "图像载入成功!\n图像大小为（" << Pic_Sky[0]->iwidth << "，" << Pic_Sky[0]->iheight << "）\n";
	//free(Pic_Sky[0]->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Sky[4] = ZXCLoadBMP("sky2_4.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[4]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Sky[4]->iwidth, Pic_Sky[4]->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Sky[4]->pByte);
	std::cout << "图像载入成功!\n图像大小为（" << Pic_Sky[0]->iwidth << "，" << Pic_Sky[0]->iheight << "）\n";
	//free(Pic_Sky[0]->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Ground = ZXCLoadBMP("ground2.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[5]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Ground->iwidth, Pic_Ground->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Ground->pByte); // 图片存储到显卡中
	std::cout << "图像载入成功!\n图像大小为（" << Pic_Ground->iwidth << "，" << Pic_Ground->iheight << "）\n";
	//free(Pic_Ground->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Box = ZXCLoadBMP("Ground4.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[6]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Box->iwidth, Pic_Box->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Box->pByte); // 图片存储到显卡中
	std::cout << "图像载入成功!\n图像大小为（" << Pic_Box->iwidth << "，" << Pic_Box->iheight << "）\n";
	//free(Pic_Box->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

//画图
void drawSky(float sky_size)//天空盒
{
	glBindTexture(GL_TEXTURE_2D, textPic[5]);//down
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);	glVertex3f(sky_size, 0, sky_size);
	glTexCoord2f(1, 1); glVertex3f(sky_size, 0, -sky_size);
	glTexCoord2f(0, 1); glVertex3f(-sky_size, 0, -sky_size);
	glTexCoord2f(0, 0); glVertex3f(-sky_size, 0, sky_size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textPic[0]);//up
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);	glVertex3f(sky_size, sky_size, -sky_size);
	glTexCoord2f(1, 1); glVertex3f(sky_size, sky_size, sky_size);
	glTexCoord2f(0, 1); glVertex3f(-sky_size, sky_size, sky_size);
	glTexCoord2f(0, 0); glVertex3f(-sky_size, sky_size, -sky_size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textPic[1]);//1
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);	glVertex3f(sky_size, 0, -sky_size);
	glTexCoord2f(1, 1); glVertex3f(sky_size, sky_size, -sky_size);
	glTexCoord2f(0, 1); glVertex3f(-sky_size, sky_size, -sky_size);
	glTexCoord2f(0, 0); glVertex3f(-sky_size, 0, -sky_size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textPic[2]);//2
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);	glVertex3f(sky_size, 0, sky_size);
	glTexCoord2f(1, 1); glVertex3f(sky_size, sky_size, sky_size);
	glTexCoord2f(0, 1); glVertex3f(sky_size, sky_size, -sky_size);
	glTexCoord2f(0, 0); glVertex3f(sky_size, 0, -sky_size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textPic[3]);//3
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);	glVertex3f(-sky_size, 0, sky_size);
	glTexCoord2f(1, 1); glVertex3f(-sky_size, sky_size, sky_size);
	glTexCoord2f(0, 1); glVertex3f(sky_size, sky_size, sky_size);
	glTexCoord2f(0, 0); glVertex3f(sky_size, 0, sky_size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textPic[4]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);	glVertex3f(-sky_size, 0, -sky_size);
	glTexCoord2f(1, 1); glVertex3f(-sky_size, sky_size, -sky_size);
	glTexCoord2f(0, 1); glVertex3f(-sky_size, sky_size, sky_size);
	glTexCoord2f(0, 0); glVertex3f(-sky_size, 0, sky_size);
	glEnd();
}

void drawBox(float box_size)
{
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textPic[6]);
	glBegin(GL_QUADS); // 前面
	glTexCoord2f(0., 0.); glVertex3f(-box_size, -box_size, box_size);
	glTexCoord2f(1., 0.); glVertex3f(box_size, -box_size, box_size);
	glTexCoord2f(1., 1.); glVertex3f(box_size, box_size, box_size);
	glTexCoord2f(0., 1.); glVertex3f(-box_size, box_size, box_size);
	glEnd();
	glBegin(GL_QUADS); // 后面
	glTexCoord2f(0., 0.); glVertex3f(box_size, -box_size, -box_size);
	glTexCoord2f(1., 0.); glVertex3f(-box_size, -box_size, -box_size);
	glTexCoord2f(1., 1.); glVertex3f(-box_size, box_size, -box_size);
	glTexCoord2f(0., 1.); glVertex3f(box_size, box_size, -box_size);
	glEnd();
	glBegin(GL_QUADS); // 左面
	glTexCoord2f(0., 0.); glVertex3f(-box_size, -box_size, -box_size);
	glTexCoord2f(1., 0.); glVertex3f(-box_size, -box_size, box_size);
	glTexCoord2f(1., 1.); glVertex3f(-box_size, box_size, box_size);
	glTexCoord2f(0., 1.); glVertex3f(-box_size, box_size, -box_size);
	glEnd();
	glBegin(GL_QUADS); // 右面
	glTexCoord2f(0., 0.); glVertex3f(box_size, -box_size, box_size);
	glTexCoord2f(1., 0.); glVertex3f(box_size, -box_size, -box_size);
	glTexCoord2f(1., 1.); glVertex3f(box_size, box_size, -box_size);
	glTexCoord2f(0., 1.); glVertex3f(box_size, box_size, box_size);
	glEnd();
	glBegin(GL_QUADS); // 上面
	glTexCoord2f(0., 0.); glVertex3f(-box_size, box_size, box_size);
	glTexCoord2f(1., 0.); glVertex3f(box_size, box_size, box_size);
	glTexCoord2f(1., 1.); glVertex3f(box_size, box_size, -box_size);
	glTexCoord2f(0., 1.); glVertex3f(-box_size, box_size, -box_size);
	glEnd();
	glBegin(GL_QUADS); // 下面
	glTexCoord2f(0., 0.); glVertex3f(-box_size, -box_size, box_size);
	glTexCoord2f(1., 0.); glVertex3f(-box_size, -box_size, -box_size);
	glTexCoord2f(1., 1.); glVertex3f(box_size, -box_size, -box_size);
	glTexCoord2f(0., 1.); glVertex3f(box_size, -box_size, box_size);
	glEnd();
}

void drawAimPoint(float size, float distance, float angle_C, float angle_Y)//准心
{
	glColor3f(1, 0, 0);
	glPointSize(size);

	glBegin(GL_POINTS);
	//中心
	glVertex3f(0, 0, 0);
	//横向
	glVertex3f(-distance* sin(angle_C), 0, distance*cos(angle_C));
	glVertex3f(+distance* sin(angle_C), 0, -distance*cos(angle_C));
	//纵向
	glVertex3f(-distance*sin(angle_Y)* cos(angle_C), -distance * cos(angle_Y), -distance*sin(angle_Y)*sin(angle_C));
	glVertex3f(distance*sin(angle_Y)* cos(angle_C), distance * cos(angle_Y), distance*sin(angle_Y)*sin(angle_C));
	glEnd();
}

void drawShooting(int dt)//射击动画，t是传递的时间，范围0-1000,表示1s
{
	float dx = shoot_x - c_x, dy = shoot_y - c_y, dz = shoot_z - c_z;
	float  k = (float)dt / 5;//比例系数,与dt范围有关//需要修改到恰好出界

							 //画球法
	glColor3f(1, 0, 0);
	glTranslatef(c_x + k*dx, c_y - 2.5 + k*dy, c_z + k*dz);
	for (int i = 0; i < 100; i++)
	{
		glTranslatef((float)(i / 25)*dx, (float)(i / 25)*dy, (float)(i / 25)*dz);
		glutSolidSphere(0.5, 10, 10);
	}
}

void drawExplosion()
{
	glColor3f(1, 0, 0);
	glutSolidSphere(4.5, 50, 50);
}

void drawHitNum(float x, float y, float z)
{
	char HitNum_1 = '0' + HitNum % 10;
	char HitNum_10 = '0' + (HitNum % 100) / 10;
	char HitNum_100 = '0' + (HitNum % 1000) / 100;
	char *Text = "Hit Numbers = ";
	char *c;

	glRasterPos3f(x, y, z);
	glScalef(2, 2, 2);
	for (c = Text; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	if (HitNum > 99)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, HitNum_100);
	if (HitNum > 9)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, HitNum_10);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, HitNum_1);
}

void drawMaxHitNum(float x, float y, float z)
{
	char MaxHitNum_1 = '0' + MaxHitNum % 10;
	char MaxHitNum_10 = '0' + (MaxHitNum % 100) / 10;
	char MaxHitNum_100 = '0' + (MaxHitNum % 1000) / 100;
	char *Text = "Your Max Hit Numbers = ";
	char *c;

	glRasterPos3f(x, y, z);
	glScalef(2, 2, 2);
	for (c = Text; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	if (MaxHitNum > 99)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, MaxHitNum_100);
	if (MaxHitNum > 9)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, MaxHitNum_10);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, MaxHitNum_1);
}

void drawMissNumber(float x, float y, float z)
{
	char MissNum_1 = '0' + MissNum % 10;
	char MissNum_10 = '0' + (MissNum % 100) / 10;
	char MissNum_100 = '0' + (MissNum % 1000) / 100;
	char *Text = "Miss Numbers = ";
	char *c;

	glRasterPos3f(x, y, z);
	glScalef(2, 2, 2);
	for (c = Text; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	if (MissNum > 99)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, MissNum_100);
	if (MissNum > 9)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, MissNum_10);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, MissNum_1);
}

void drawLastTime(float x, float y, float z)
{
	int LastTime = GAMETIME - game_time / 1000;
	char LastTime_1 = '0' + LastTime % 10;
	char LastTime_10 = '0' + (LastTime % 100) / 10;
	char LastTime_100 = '0' + (LastTime % 1000) / 100;
	char *Text = "Remaining Time = ";
	char *c;

	glRasterPos3f(x, y, z);
	glScalef(2, 2, 2);
	for (c = Text; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	if (LastTime > 99)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, LastTime_100);
	if (LastTime > 9)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, LastTime_10);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, LastTime_1);
}


//基本设置//

void reshape(int w, int h)
{
	win_x = w; win_y = h;
	mouse_x = w / 2;	mouse_y = h / 2;//鼠标在窗口中央时为中心视角
	glutPostRedisplay();
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);//投影变换//
	glLoadIdentity();
	float R = (float)w / h;
	gluPerspective(45.0, R, 0.1, 1000.);

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void display_begin()
{
	glColor3f(1, 1, 1);
	char *Text1 = "Welcome to The Crazy Box !";
	char *Text2 = "Press 'Q' to begin the Game......";
	char *c;

	glPushMatrix();
	glRasterPos3f(-30, 70, 0);
	//glScalef(2, 2, 2);
	for (c = Text1; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	glPopMatrix();

	glPushMatrix();
	glRasterPos3f(-30, 60, 10);
	glColor3f(1, 0, 0);
	for (c = Text2; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	glPopMatrix();
}

void display_over()
{
	glColor3f(1, 0, 0);
	char *Text1 = "Game Over !";
	char *Text2 = "If you want to restart a game,please press 'Q' ......";
	char *c;

	glPushMatrix();
	glRasterPos3f(-30, 75, 0);
	for (c = Text1; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	glPopMatrix();

	glPushMatrix();
	drawHitNum(-20, 71, 0);
	glPopMatrix();

	glPushMatrix();
	drawMissNumber(-20, 67, 0);
	glPopMatrix();

	glPushMatrix();
	drawMaxHitNum(-20, 63, 0);
	glPopMatrix();

	glPushMatrix();
	glRasterPos3f(-30, 59, 0);

	for (c = Text2; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	glPopMatrix();
}

void display()
{
	c_y = main_size / 2;
	float angle_C = pi*(c_angle) / 180;
	float angle_Y = pi*(eye_angle) / 180;
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//天空盒//
	glPushMatrix();
	glColor3f(1., 1., 1.);
	drawSky(main_size);
	glPopMatrix();
	//漫游//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(c_x, c_y, c_z,
		c_x - 10.*cos(angle_Y)* cos(angle_C), c_y + 10 * sin(angle_Y), c_z - 10.*cos(angle_Y)*sin(angle_C),
		0, 1, 0);

	switch (displayMOD)
	{
	case 1:
		display_begin();
		break;
	case 2:
		//准心//
		glPushMatrix();
		glTranslatef(c_x - 2 * cos(angle_Y)* cos(angle_C), c_y + 2 * sin(angle_Y), c_z - 2 * cos(angle_Y)*sin(angle_C));
		drawAimPoint(5, 0.05, angle_C, angle_Y);
		glPopMatrix();

		for (int i = 0; i < NBOX; i++)
		{
			glPushMatrix();
			glTranslatef(xbox[i].x, xbox[i].y, xbox[i].z);
			glRotatef(box_angle, 1, 1, 1);
			drawBox(xbox[i].size);
			glPopMatrix();
		}

		//爆炸反应//
		for (int j = 0; j<NBOX; j++)
		{
			if (((xbox[j].exp_time + 1000) % 600000)>t_time)
			{
				glPushMatrix();
				glTranslatef(xbox[j].ex, xbox[j].ey, xbox[j].ez);
				drawExplosion();
				glPopMatrix();
			}
			else xbox[j].exp_time = -1;
		}

		//射击弹轨动画
		//int deltaT = ((t_time - shoot_time + 60000) % 60000);
		if (((t_time - shoot_time + 600000) % 600000) <= 100)//动画时长=0.1s
		{
			glPushMatrix();
			drawShooting((t_time - shoot_time + 600000) % 600000);
			glPopMatrix();
		}
		else shoot_time = -1;


		//写字
		glPushMatrix();
		glColor3f(0, 1, 0);
		drawHitNum(20, 70, 0);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 0);
		drawLastTime(10, 75, 10);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 0, 0);
		drawMissNumber(20, 65, 0);
		glPopMatrix();

		break;
	case 3:
		display_over();
		break;
	}

	glutSwapBuffers();

}


//菜单//
void processMenu(int value)
{
	switch (value)
	{
	case 0:
		box_basic_Speed = 0.1;//简单模式
		break;
	case 1:
		box_basic_Speed = 0.8;//困难模式
		break;
	case 2:
		if (displayMOD == 2)
		{
			gameMOD = 1;//固定位置
			resetPosition(gameMOD);
		}
		break;
	case 3:
		if (displayMOD == 2)
		{
			gameMOD = 2;//移动位置
			resetPosition(gameMOD);
		}
		break;
	case 4:
		glutFullScreen();//全屏化
		FullScreen = 1;
		break;
	case 5:
		reStart();//重新开始
		break;
	case 6:
		exit(0);//退出游戏
		break;
	}

	glutPostRedisplay();
}

void creatMenu()
{
	int menu;
	menu = glutCreateMenu(processMenu);
	glutAddMenuEntry("简单模式", 0);
	glutAddMenuEntry("困难模式", 1);
	glutAddMenuEntry("固定位置模式", 2);
	glutAddMenuEntry("移动位置模式", 3);
	glutAddMenuEntry("全屏模式", 4);
	glutAddMenuEntry("重新开始", 5);
	glutAddMenuEntry("退出游戏", 6);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutPostRedisplay();

}

//计时器//

void timerProc(int id)
{
	//时间计数
	if (t_time < 600000)	t_time += 10;
	else			t_time = 1;

	if (displayMOD == 2 && game_time < GAMETIME * 1000 && game_time >= 0)
	{
		game_time += 10;

		//盒子旋转
		if (box_angle < 3600)	box_angle += 2;
		else				box_angle = 0;
		//盒子下落与命中反应
		for (int i = 0; i < NBOX; i++)
		{
			if (xbox[i].explosion&&xbox[i].exp_time < 0)
			{
				xbox[i].ex = xbox[i].x;
				xbox[i].ey = xbox[i].y;
				xbox[i].ez = xbox[i].z;

				//此处重新初始化xbox[i]//
				reInitBox(i);

				xbox[i].exp_time = t_time;
			}
			else if (xbox[i].y >= -2 * xbox[i].size)
				xbox[i].y -= xbox[i].dropSpeed;
			else if (game_time >= 0)
			{

				reInitBox(i);
				MissNum++;
			}
		}
	}
	else if (game_time >= GAMETIME * 1000)
	{
		gameOver();
		game_time = -1;
	}
	//射击反应
	if (shooted&&shoot_time < 0)
	{
		shoot_x = aim_x;
		shoot_y = aim_y;
		shoot_z = aim_z;
		shoot_time = t_time;
		shooted = 0;
	}

	//无限制移动视角
	//方位角
	if (displayMOD == 1 && c_angle>65 && c_angle < 96)
	{
		if (mouse_x <= 2)
			c_angle -= 10 * mouse_speed;
		else if (mouse_x >= win_x - 2)
			c_angle += 10 * mouse_speed;
	}
	else if (displayMOD == 2 && gameMOD == 2)
	{
		if (mouse_x <= 2)
			c_angle -= 10 * mouse_speed;
		else if (mouse_x >= win_x - 2)
			c_angle += 10 * mouse_speed;
	}
	else if (displayMOD == 2 && gameMOD == 1 && c_angle>30 && c_angle < 150)
	{
		if (mouse_x <= 2)
			c_angle -= 10 * mouse_speed;
		else if (mouse_x >= win_x - 2)
			c_angle += 10 * mouse_speed;
	}

	//仰角
	if (displayMOD == 2)
	{
		if (mouse_y <= 5 && (eye_angle + 10 * mouse_speed) < 88)
			eye_angle += 10 * mouse_speed;
		else if (mouse_y >= win_y - 5 && (eye_angle - 10 * mouse_speed) > -88)
			eye_angle -= 10 * mouse_speed;
	}
	else if (displayMOD == 1)
	{
		if (mouse_y <= 5 && (eye_angle + 10 * mouse_speed) < 27.5)
			eye_angle += 10 * mouse_speed;
		else if (mouse_y >= win_y - 5 && (eye_angle - 10 * mouse_speed) > -4.5)
			eye_angle -= 10 * mouse_speed;
	}


	glutPostRedisplay();
	glutTimerFunc(10, timerProc, 1);
}

//鼠标//

void mousePassiveMove(int x, int y)//只是移动鼠标
{
	//计算准心坐标
	float angle_C = pi*(c_angle) / 180;
	float angle_Y = pi*(eye_angle) / 180;
	aim_x = c_x - 2 * cos(angle_Y)* cos(angle_C);
	aim_y = c_y + 2 * sin(angle_Y);
	aim_z = c_z - 2 * cos(angle_Y)*sin(angle_C);

	int dx = x - mouse_x, dy = y - mouse_y;


	//限制左右视角的方法：
	if (displayMOD == 1)
	{
		if ((c_angle + dx*mouse_speed) > 65 && (c_angle + dx*mouse_speed) < 96)
			c_angle += dx*mouse_speed;
		if ((eye_angle - dy*mouse_speed) < 27.5 && (eye_angle - dy*mouse_speed) > -4.5)
			eye_angle -= dy*mouse_speed;
	}
	else if (displayMOD == 2)
	{
		if (gameMOD == 1)
		{
			if ((c_angle + dx*mouse_speed) > 30 && (c_angle + dx*mouse_speed) < 150)
				c_angle += dx*mouse_speed;
		}
		else if (gameMOD == 2)
			c_angle += dx*mouse_speed;

		if ((eye_angle - dy*mouse_speed) < 88 && (eye_angle - dy*mouse_speed) > -88)
			eye_angle -= dy*mouse_speed;
	}

	if (mouse_in)
	{
		mouse_x = x;
		mouse_y = y;
	}
	glutPostRedisplay();
	//std::cout << "视角移动（" << dx << "," << dy << "）\n";
	//std::cout << "鼠标现在位置（" << mouse_x << "," << mouse_y << "）\n";
	//std::cout << "c_angle=" << c_angle << '\n';
	//std::cout << "eye_angle=" << eye_angle << '\n';
}

void mouseButton(int button, int state, int x, int y)//鼠标按键与松开//可以用来设计命中反应
{
	if (state == GLUT_DOWN&&button == GLUT_LEFT_BUTTON&&displayMOD == 2)
	{
		shooted = 1;//进行了一次射击
		bool flag = 0;
		for (int i = 0; i < 10; i++)
		{
			if (inLine(xbox[i].x, xbox[i].y, xbox[i].z, 0.001))
			{
				xbox[i].explosion = 1;
				std::cout << "————————\n命中目标！！\n————————\n";
				flag = 1;
				HitNum++;
			}
		}

		if (!flag)
			std::cout << "——————————\n未\t命\t中\t\n——————————\n";
	}
}

void mouseEntry(int state)
{
	if (state == GLUT_ENTERED)
	{
		mouse_in = 1;
	}
	else if (state == GLUT_LEFT)
	{
		mouse_in = 0;
	}
}

//键盘//
void processNormalKeys(unsigned char key, int x, int y)
{
	float angle_C = pi*(c_angle) / 180;//用于前后移动
	float angle_D = pi*(c_angle - 90) / 180;//用于左右移动
	bool move_able_w = 1, move_able_s = 1, move_able_a = 1, move_able_d = 1;//若值为0：出界，不可移动
	float ratio = 0.6;//移动范围
	if ((c_x - speed*cos(angle_C)) < main_size*ratio && (c_x - speed*cos(angle_C)) > -main_size*ratio &&
		(c_z - speed*sin(angle_C)) < main_size*ratio && (c_z - speed*sin(angle_C)) > -main_size*ratio)
		move_able_w = 1;
	else move_able_w = 0;

	if ((c_x + speed*cos(angle_C)) < main_size*ratio && (c_x + speed*cos(angle_C)) > -main_size*ratio &&
		(c_z + speed*sin(angle_C)) < main_size*ratio && (c_z + speed*sin(angle_C)) > -main_size*ratio)
		move_able_s = 1;
	else move_able_s = 0;

	if ((c_x - speed*cos(angle_D)) < main_size*ratio && (c_x - speed*cos(angle_D)) > -main_size*ratio &&
		(c_z - speed*sin(angle_D)) < main_size*ratio && (c_z - speed*sin(angle_D)) > -main_size*ratio)
		move_able_a = 1;
	else move_able_a = 0;

	if ((c_x + speed*cos(angle_D)) < main_size*ratio && (c_x + speed*cos(angle_D)) > -main_size*ratio &&
		(c_z + speed*sin(angle_D)) < main_size*ratio && (c_z + speed*sin(angle_D)) > -main_size*ratio)
		move_able_d = 1;
	else move_able_d = 0;

	switch (key)
	{
	case 27://退出全屏
		if (FullScreen)
		{
			glutReshapeWindow(750, 500);
			glutPositionWindow(100, 100);
			FullScreen = 0;
		}
		break;
	case 'r'://复位
		resetPosition(gameMOD);
		break;
	case 'w': //向前
		if ((gameMOD == 2) && move_able_w)
		{
			c_x -= speed*cos(angle_C);
			c_z -= speed*sin(angle_C);
		}
		break;
	case 's'://向后
		if ((gameMOD == 2) && move_able_s)
		{
			c_x += speed*cos(angle_C);
			c_z += speed*sin(angle_C);
		}
		break;
	case 'a': //向左
		if ((gameMOD == 2) && move_able_a)
		{
			c_x -= speed*cos(angle_D);
			c_z -= speed*sin(angle_D);
		}
		break;
	case 'd'://向右
		if ((gameMOD == 2) && move_able_d)
		{
			c_x += speed*cos(angle_D);
			c_z += speed*sin(angle_D);
		}
		break;

	case'Q':
	case'q'://开始游戏
		if (displayMOD != 2)
			reStart();
	}
	glutPostRedisplay();
}

void pressKey(int key, int x, int y)
{
	float angle_C = pi*(c_angle) / 180;
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		if (eye_angle < 89)
			eye_angle += 1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (eye_angle > -89)
			eye_angle -= 1;
		break;
	case GLUT_KEY_LEFT: // 左转
		c_angle -= 5.0;
		break;
	case GLUT_KEY_RIGHT: // 右转
		c_angle += 5.0;
		break;
	case GLUT_KEY_UP: //向前
		c_x -= speed*cos(angle_C);
		c_z -= speed*sin(angle_C);
		break;
	case GLUT_KEY_DOWN://向后
		c_x += speed*cos(angle_C);
		c_z += speed*sin(angle_C);
		break;

		//以下用来测试
	case GLUT_KEY_F1://欢迎界面
		displayMOD = 1;
		resetPosition(1);
		break;
	case GLUT_KEY_F2://游戏界面
		displayMOD = 2;
		resetPosition(gameMOD);
		break;
	case GLUT_KEY_F3://游戏结束界面
		displayMOD = 3;
		resetPosition(1);
		break;
	}
	glutPostRedisplay();
}

void initFunc()
{
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutEntryFunc(mouseEntry);
	glutMouseFunc(mouseButton);
	glutPassiveMotionFunc(mousePassiveMove);

	glutTimerFunc(10, timerProc, 1);
}

