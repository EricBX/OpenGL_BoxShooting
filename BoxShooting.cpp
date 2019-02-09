//��Ϸ���ƣ����ĺ���
//���ߣ�EricBX
//�淨����ղ�֪Ϊ�����·��ĺ��ӣ�����׼�����������������ĺ��ӣ��Ƿ֣���ʱ��ﵽ30sʱ��Ϸ����������
//���ܣ�����ģʽ������һ����˵����л����׳̶ȣ��л���Ϸģʽ������ȫ��ģʽ�����¿�ʼ���˳���Ϸ��//ȫ��ģʽ�²˵�����ʾ
//		�ƶ������Ըı��ӽǣ����������������
//		�ƶ�ģʽ��wsad����ǰ������ƽ�ƣ�
//		Esc���˳�ȫ����
//		����ר�õĹ��ܣ�F1ǿ���л�Welcome���棬F2ǿ���л�Game���棬F3ǿ���л�Gameover����
//		����ר�ù��ܣ�����ģʽ�·������������ǰ���ƶ�����������ˮƽ�ӽ�����ת����PgUp��PgDn�������ǣ�

#include "functions.h"

void main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(750, 500);
	glutCreateWindow("���ĺ���");

	//�����ʼ��//
	initTex();

	//���ӳ�ʼ��//
	initBox();

	//�˵�//
	creatMenu();

	//���ܳ�ʼ��//
	initFunc();

	//����ѭ��//
	glutMainLoop();

}


//����
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

void initBox()//��ʼ��ȫ������
{
	int i;
	int span = main_size;
	srand((unsigned)time(NULL));
	for (i = 0; i < NBOX; i++)          //���������  
	{
		xbox[i].x = -span / 2 + rand() % span;
		xbox[i].z = -3 * span / 4 + rand() % span;
		xbox[i].y = main_size + xbox[i].size * 2;
		xbox[i].size = 4 + (float)(i / 10);
		xbox[i].dropSpeed = box_basic_Speed + (float)(rand() % 20 + i) / (float)50;
		xbox[i].explosion = 0;
		xbox[i].exp_time = -1;
		//std::cout << "\n��������������������\nBOX[" << i << "]��ʼ����ɣ�\n";
		//std::cout<<"����(x,z)=("<<xbox[i].x<<","<<xbox[i].z<<")\n��������������������\n";
	}
}

void reInitBox(int i)//���³�ʼ�����Ϊi�ĺ���
{
	int span = main_size;
	srand((unsigned)time(NULL));
	xbox[i].x = -span / 2 + rand() % span + 3 * i - 15;
	xbox[i].z = -3 * span / 4 + rand() % span - 3 * i;
	xbox[i].y = main_size + xbox[i].size * 2;
	xbox[i].dropSpeed = box_basic_Speed + (float)(rand() % 20 + i) / (float)50;
	xbox[i].explosion = 0;
	//std::cout << "\n��������������������\n\tBOX["<<i<<"]�����ˣ�\n";
	//std::cout << "����(x,z)=(" << xbox[i].x << "," << xbox[i].z << ")\n";
	//std::cout << "dropSpeed=" << xbox[i].dropSpeed << '\n';
	//std::cout << "��������������������\n";
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
	std::cout << "\n��������������������\n��\tϷ\t��\t��\t��\n���У�" << HitNum << "\t��ʧ��" << MissNum << "\n������У�" << MaxHitNum << "\n��������������������\n";
}
//�����ж�����//
bool inLine(float bx, float by, float bz, float a)//�ж�box��aimpoint��camera�����Ƿ��ߣ�aΪ���нǶ��������ұ�ʾ
{
	float ax, ay, az;//׼���е�����
	float angle_C = pi*(c_angle) / 180;
	float angle_Y = pi*(eye_angle) / 180;
	ax = c_x - 2 * cos(angle_Y)* cos(angle_C);
	ay = c_y + 2 * sin(angle_Y);
	az = c_z - 2 * cos(angle_Y)*sin(angle_C);

	//�����ж���
	float a2 = (bx - c_x)*(bx - c_x) + (by - c_y)*(by - c_y) + (bz - c_z)*(bz - c_z);
	float b2 = (ax - c_x)*(ax - c_x) + (ay - c_y)*(ay - c_y) + (az - c_z)*(az - c_z);
	float c2 = (bx - ax)*(bx - ax) + (by - ay)*(by - ay) + (bz - az)*(bz - az);
	float cos_ACB = (a2 + b2 - c2) / (2 * sqrt(a2)*sqrt(b2));
	//std::cout << "����=" << cos_ACB << '\n';
	if (cos_ACB >= 1 - a)
		return 1;
	else return 0;

}

//����
void initTex()
{
	glGenTextures(6, textPic);

	Pic_Sky[0] = ZXCLoadBMP("sky2_0.bmp");
	//Pic_Sky[0] = ZXCLoadBMP("dom_0.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[0]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Sky[0]->iwidth, Pic_Sky[0]->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Sky[0]->pByte);
	std::cout << "ͼ������ɹ�!\nͼ���СΪ��" << Pic_Sky[0]->iwidth << "��" << Pic_Sky[0]->iheight << "��\n";
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
	std::cout << "ͼ������ɹ�!\nͼ���СΪ��" << Pic_Sky[0]->iwidth << "��" << Pic_Sky[0]->iheight << "��\n";
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
	std::cout << "ͼ������ɹ�!\nͼ���СΪ��" << Pic_Sky[0]->iwidth << "��" << Pic_Sky[0]->iheight << "��\n";
	//free(Pic_Sky[0]->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Sky[3] = ZXCLoadBMP("sky2_3.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[3]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Sky[3]->iwidth, Pic_Sky[3]->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Sky[3]->pByte);
	std::cout << "ͼ������ɹ�!\nͼ���СΪ��" << Pic_Sky[0]->iwidth << "��" << Pic_Sky[0]->iheight << "��\n";
	//free(Pic_Sky[0]->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Sky[4] = ZXCLoadBMP("sky2_4.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[4]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Sky[4]->iwidth, Pic_Sky[4]->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Sky[4]->pByte);
	std::cout << "ͼ������ɹ�!\nͼ���СΪ��" << Pic_Sky[0]->iwidth << "��" << Pic_Sky[0]->iheight << "��\n";
	//free(Pic_Sky[0]->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Ground = ZXCLoadBMP("ground2.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[5]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Ground->iwidth, Pic_Ground->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Ground->pByte); // ͼƬ�洢���Կ���
	std::cout << "ͼ������ɹ�!\nͼ���СΪ��" << Pic_Ground->iwidth << "��" << Pic_Ground->iheight << "��\n";
	//free(Pic_Ground->pByte);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Pic_Box = ZXCLoadBMP("Ground4.bmp");
	glBindTexture(GL_TEXTURE_2D, textPic[6]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Pic_Box->iwidth, Pic_Box->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pic_Box->pByte); // ͼƬ�洢���Կ���
	std::cout << "ͼ������ɹ�!\nͼ���СΪ��" << Pic_Box->iwidth << "��" << Pic_Box->iheight << "��\n";
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

//��ͼ
void drawSky(float sky_size)//��պ�
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
	glBegin(GL_QUADS); // ǰ��
	glTexCoord2f(0., 0.); glVertex3f(-box_size, -box_size, box_size);
	glTexCoord2f(1., 0.); glVertex3f(box_size, -box_size, box_size);
	glTexCoord2f(1., 1.); glVertex3f(box_size, box_size, box_size);
	glTexCoord2f(0., 1.); glVertex3f(-box_size, box_size, box_size);
	glEnd();
	glBegin(GL_QUADS); // ����
	glTexCoord2f(0., 0.); glVertex3f(box_size, -box_size, -box_size);
	glTexCoord2f(1., 0.); glVertex3f(-box_size, -box_size, -box_size);
	glTexCoord2f(1., 1.); glVertex3f(-box_size, box_size, -box_size);
	glTexCoord2f(0., 1.); glVertex3f(box_size, box_size, -box_size);
	glEnd();
	glBegin(GL_QUADS); // ����
	glTexCoord2f(0., 0.); glVertex3f(-box_size, -box_size, -box_size);
	glTexCoord2f(1., 0.); glVertex3f(-box_size, -box_size, box_size);
	glTexCoord2f(1., 1.); glVertex3f(-box_size, box_size, box_size);
	glTexCoord2f(0., 1.); glVertex3f(-box_size, box_size, -box_size);
	glEnd();
	glBegin(GL_QUADS); // ����
	glTexCoord2f(0., 0.); glVertex3f(box_size, -box_size, box_size);
	glTexCoord2f(1., 0.); glVertex3f(box_size, -box_size, -box_size);
	glTexCoord2f(1., 1.); glVertex3f(box_size, box_size, -box_size);
	glTexCoord2f(0., 1.); glVertex3f(box_size, box_size, box_size);
	glEnd();
	glBegin(GL_QUADS); // ����
	glTexCoord2f(0., 0.); glVertex3f(-box_size, box_size, box_size);
	glTexCoord2f(1., 0.); glVertex3f(box_size, box_size, box_size);
	glTexCoord2f(1., 1.); glVertex3f(box_size, box_size, -box_size);
	glTexCoord2f(0., 1.); glVertex3f(-box_size, box_size, -box_size);
	glEnd();
	glBegin(GL_QUADS); // ����
	glTexCoord2f(0., 0.); glVertex3f(-box_size, -box_size, box_size);
	glTexCoord2f(1., 0.); glVertex3f(-box_size, -box_size, -box_size);
	glTexCoord2f(1., 1.); glVertex3f(box_size, -box_size, -box_size);
	glTexCoord2f(0., 1.); glVertex3f(box_size, -box_size, box_size);
	glEnd();
}

void drawAimPoint(float size, float distance, float angle_C, float angle_Y)//׼��
{
	glColor3f(1, 0, 0);
	glPointSize(size);

	glBegin(GL_POINTS);
	//����
	glVertex3f(0, 0, 0);
	//����
	glVertex3f(-distance* sin(angle_C), 0, distance*cos(angle_C));
	glVertex3f(+distance* sin(angle_C), 0, -distance*cos(angle_C));
	//����
	glVertex3f(-distance*sin(angle_Y)* cos(angle_C), -distance * cos(angle_Y), -distance*sin(angle_Y)*sin(angle_C));
	glVertex3f(distance*sin(angle_Y)* cos(angle_C), distance * cos(angle_Y), distance*sin(angle_Y)*sin(angle_C));
	glEnd();
}

void drawShooting(int dt)//���������t�Ǵ��ݵ�ʱ�䣬��Χ0-1000,��ʾ1s
{
	float dx = shoot_x - c_x, dy = shoot_y - c_y, dz = shoot_z - c_z;
	float  k = (float)dt / 5;//����ϵ��,��dt��Χ�й�//��Ҫ�޸ĵ�ǡ�ó���

							 //����
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


//��������//

void reshape(int w, int h)
{
	win_x = w; win_y = h;
	mouse_x = w / 2;	mouse_y = h / 2;//����ڴ�������ʱΪ�����ӽ�
	glutPostRedisplay();
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);//ͶӰ�任//
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


	//��պ�//
	glPushMatrix();
	glColor3f(1., 1., 1.);
	drawSky(main_size);
	glPopMatrix();
	//����//
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
		//׼��//
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

		//��ը��Ӧ//
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

		//������춯��
		//int deltaT = ((t_time - shoot_time + 60000) % 60000);
		if (((t_time - shoot_time + 600000) % 600000) <= 100)//����ʱ��=0.1s
		{
			glPushMatrix();
			drawShooting((t_time - shoot_time + 600000) % 600000);
			glPopMatrix();
		}
		else shoot_time = -1;


		//д��
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


//�˵�//
void processMenu(int value)
{
	switch (value)
	{
	case 0:
		box_basic_Speed = 0.1;//��ģʽ
		break;
	case 1:
		box_basic_Speed = 0.8;//����ģʽ
		break;
	case 2:
		if (displayMOD == 2)
		{
			gameMOD = 1;//�̶�λ��
			resetPosition(gameMOD);
		}
		break;
	case 3:
		if (displayMOD == 2)
		{
			gameMOD = 2;//�ƶ�λ��
			resetPosition(gameMOD);
		}
		break;
	case 4:
		glutFullScreen();//ȫ����
		FullScreen = 1;
		break;
	case 5:
		reStart();//���¿�ʼ
		break;
	case 6:
		exit(0);//�˳���Ϸ
		break;
	}

	glutPostRedisplay();
}

void creatMenu()
{
	int menu;
	menu = glutCreateMenu(processMenu);
	glutAddMenuEntry("��ģʽ", 0);
	glutAddMenuEntry("����ģʽ", 1);
	glutAddMenuEntry("�̶�λ��ģʽ", 2);
	glutAddMenuEntry("�ƶ�λ��ģʽ", 3);
	glutAddMenuEntry("ȫ��ģʽ", 4);
	glutAddMenuEntry("���¿�ʼ", 5);
	glutAddMenuEntry("�˳���Ϸ", 6);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutPostRedisplay();

}

//��ʱ��//

void timerProc(int id)
{
	//ʱ�����
	if (t_time < 600000)	t_time += 10;
	else			t_time = 1;

	if (displayMOD == 2 && game_time < GAMETIME * 1000 && game_time >= 0)
	{
		game_time += 10;

		//������ת
		if (box_angle < 3600)	box_angle += 2;
		else				box_angle = 0;
		//�������������з�Ӧ
		for (int i = 0; i < NBOX; i++)
		{
			if (xbox[i].explosion&&xbox[i].exp_time < 0)
			{
				xbox[i].ex = xbox[i].x;
				xbox[i].ey = xbox[i].y;
				xbox[i].ez = xbox[i].z;

				//�˴����³�ʼ��xbox[i]//
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
	//�����Ӧ
	if (shooted&&shoot_time < 0)
	{
		shoot_x = aim_x;
		shoot_y = aim_y;
		shoot_z = aim_z;
		shoot_time = t_time;
		shooted = 0;
	}

	//�������ƶ��ӽ�
	//��λ��
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

	//����
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

//���//

void mousePassiveMove(int x, int y)//ֻ���ƶ����
{
	//����׼������
	float angle_C = pi*(c_angle) / 180;
	float angle_Y = pi*(eye_angle) / 180;
	aim_x = c_x - 2 * cos(angle_Y)* cos(angle_C);
	aim_y = c_y + 2 * sin(angle_Y);
	aim_z = c_z - 2 * cos(angle_Y)*sin(angle_C);

	int dx = x - mouse_x, dy = y - mouse_y;


	//���������ӽǵķ�����
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
	//std::cout << "�ӽ��ƶ���" << dx << "," << dy << "��\n";
	//std::cout << "�������λ�ã�" << mouse_x << "," << mouse_y << "��\n";
	//std::cout << "c_angle=" << c_angle << '\n';
	//std::cout << "eye_angle=" << eye_angle << '\n';
}

void mouseButton(int button, int state, int x, int y)//��갴�����ɿ�//��������������з�Ӧ
{
	if (state == GLUT_DOWN&&button == GLUT_LEFT_BUTTON&&displayMOD == 2)
	{
		shooted = 1;//������һ�����
		bool flag = 0;
		for (int i = 0; i < 10; i++)
		{
			if (inLine(xbox[i].x, xbox[i].y, xbox[i].z, 0.001))
			{
				xbox[i].explosion = 1;
				std::cout << "����������������\n����Ŀ�꣡��\n����������������\n";
				flag = 1;
				HitNum++;
			}
		}

		if (!flag)
			std::cout << "��������������������\nδ\t��\t��\t\n��������������������\n";
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

//����//
void processNormalKeys(unsigned char key, int x, int y)
{
	float angle_C = pi*(c_angle) / 180;//����ǰ���ƶ�
	float angle_D = pi*(c_angle - 90) / 180;//���������ƶ�
	bool move_able_w = 1, move_able_s = 1, move_able_a = 1, move_able_d = 1;//��ֵΪ0�����磬�����ƶ�
	float ratio = 0.6;//�ƶ���Χ
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
	case 27://�˳�ȫ��
		if (FullScreen)
		{
			glutReshapeWindow(750, 500);
			glutPositionWindow(100, 100);
			FullScreen = 0;
		}
		break;
	case 'r'://��λ
		resetPosition(gameMOD);
		break;
	case 'w': //��ǰ
		if ((gameMOD == 2) && move_able_w)
		{
			c_x -= speed*cos(angle_C);
			c_z -= speed*sin(angle_C);
		}
		break;
	case 's'://���
		if ((gameMOD == 2) && move_able_s)
		{
			c_x += speed*cos(angle_C);
			c_z += speed*sin(angle_C);
		}
		break;
	case 'a': //����
		if ((gameMOD == 2) && move_able_a)
		{
			c_x -= speed*cos(angle_D);
			c_z -= speed*sin(angle_D);
		}
		break;
	case 'd'://����
		if ((gameMOD == 2) && move_able_d)
		{
			c_x += speed*cos(angle_D);
			c_z += speed*sin(angle_D);
		}
		break;

	case'Q':
	case'q'://��ʼ��Ϸ
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
	case GLUT_KEY_LEFT: // ��ת
		c_angle -= 5.0;
		break;
	case GLUT_KEY_RIGHT: // ��ת
		c_angle += 5.0;
		break;
	case GLUT_KEY_UP: //��ǰ
		c_x -= speed*cos(angle_C);
		c_z -= speed*sin(angle_C);
		break;
	case GLUT_KEY_DOWN://���
		c_x += speed*cos(angle_C);
		c_z += speed*sin(angle_C);
		break;

		//������������
	case GLUT_KEY_F1://��ӭ����
		displayMOD = 1;
		resetPosition(1);
		break;
	case GLUT_KEY_F2://��Ϸ����
		displayMOD = 2;
		resetPosition(gameMOD);
		break;
	case GLUT_KEY_F3://��Ϸ��������
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

