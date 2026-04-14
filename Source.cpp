#include <gl/glut.h>
#include <GL/GL.h>
#include <math.h>
#include <iostream>
#include <windows.h>

GLuint  textura_id;

struct textura_struct {
	int W;
	int H;
	unsigned char *Image;
}get_textura;

double moveX = 0;
double moveY = 0;

int LoadTexture(char *FileName)
{
	FILE *F;
	if ((F = fopen(FileName, "rb")) == NULL) return 0;
	/*Перемещаемся в bmp-файле на нужную позицию, и считываем ширину и длинну */
	fseek(F, 18, SEEK_SET);
	fread(&(get_textura.W), 2, 1, F);
	fseek(F, 2, SEEK_CUR);
	fread(&(get_textura.H), 2, 1, F);

	printf("%d x %d\n", get_textura.W, get_textura.H);

	/* Выделяем память под изображение. Если память не выделилась, закрываем файл и выходим с ошибкой */
	if ((get_textura.Image = (unsigned char *)malloc(sizeof(unsigned char) * 3 * get_textura.W * get_textura.H)) == NULL) {
		fclose(F);
		return 0;
	}
	/* Считываем изображение в память по 3 бита, то бишь RGB для каждого пикселя */
	fseek(F, 30, SEEK_CUR);
	fread(get_textura.Image, 3, get_textura.W * get_textura.H, F);

	glGenTextures(1, &textura_id);
	glBindTexture(GL_TEXTURE_2D, textura_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE, get_textura.W, get_textura.H, GL_RGB, GL_UNSIGNED_BYTE, get_textura.Image);
	free(get_textura.Image);
	fclose(F);

	return 1;
}

void drawPolygon(float x, float y, float z) {
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 1.0, 0.0);
	glTexCoord2f(0, 0); glVertex3f(x, y, z);
	glTexCoord2f(1, 0); glVertex3f(x + 2.0, y + 0.0, z);
	glTexCoord2f(1, 1); glVertex3f(x + 2.0, y + 2.0, z);
	glTexCoord2f(0, 1); glVertex3f(x + 0.0, y + 2.0, z);
	glEnd();
}

void drawPUD(float x, float y, float z) {
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 1.0, 0.0);
	glTexCoord2f(0, 0); glVertex3f(x, y, z);
	glTexCoord2f(1, 0); glVertex3f(x, y, z - 2.0);
	glTexCoord2f(1, 1); glVertex3f(x - 2.0, y, z - 2.0);
	glTexCoord2f(0, 1); glVertex3f(x - 2.0, y, z);
	glEnd();
}

void drawPLR(float x, float y, float z) {
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 1.0, 0.0);
	glTexCoord2f(0, 0); glVertex3f(x, y, z);
	glTexCoord2f(1, 0); glVertex3f(x, y, z - 2.0);
	glTexCoord2f(1, 1); glVertex3f(x, y + 2.0, z - 2.0);
	glTexCoord2f(0, 1); glVertex3f(x, y + 2.0, z);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textura_id);	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glRotatef(moveX, 0, 1, 0);
	glRotatef(moveY, 1, 0, 0);	

	//front
	drawPolygon(1, 1, 0);
	drawPolygon(1, 3, 0);
	drawPolygon(1, -1, 0);
	drawPolygon(-1, 1, 0);
	drawPolygon(3, 1, 0);

	//back
	drawPolygon(1, 1, -2);
	drawPolygon(1, 3, -2);
	drawPolygon(1, -1, -2);
	drawPolygon(-1, 1, -2);
	drawPolygon(3, 1, -2);

	drawPUD(1, 1, 0);
	drawPUD(5, 1, 0);

	drawPUD(1, 3, 0);
	drawPUD(5, 3, 0);

	drawPUD(3, -1, 0);
	drawPUD(3, 5, 0);

	drawPLR(-1, 1, 0);
	drawPLR(1, 3, 0);
	drawPLR(1, -1, 0);

	drawPLR(5, 1, 0);
	drawPLR(3, 3, 0);
	drawPLR(3, -1, 0);

	glutSwapBuffers();
	glFinish;
}
void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		moveX -= 10;
		break;
	case GLUT_KEY_LEFT:
		moveX += 10;
		break;
	case GLUT_KEY_UP:
		moveY += 10;
		break;
	case GLUT_KEY_DOWN:
		moveY -= 10;
		break;
	}
	glutPostRedisplay();
}
void mouseMotion(int x, int y) {
	moveX = x;
	moveY = y;
	glutPostRedisplay();
}

int main(int argc, char * argv[]) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Плюс");
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);	
	glOrtho(-6, 6, -6, 6, -6, 6);
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);
	glutPassiveMotionFunc(mouseMotion);
	if (LoadTexture((char *)"test2.bmp") != 1) printf("ERROR\n");
	glutMainLoop();
}