
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

enum { InitMode = 0, SingleRotationMode = 1, AnimationMode = 2};
int Mode = InitMode;
const int NumVertices = 12; 
const int n = 8;
GLfloat rotatingDegree = 0.0;
GLfloat radius = 0.4; 

GLuint ww = 500;
GLuint wh = 500;

point4 points[NumVertices];
color4 colors[NumVertices];

point4 LShape[6] = {
/*
	point4(-1.0,-1.0 ,0.0, 1.0), //a0
	point4(-1.0,-0.6 ,0.0, 1.0), //a1
	point4(-0.9,-0.6 ,0.0, 1.0), //a2
	point4(-0.9,-0.9 ,0.0, 1.0), //a3
	point4(-0.8,-0.9 ,0.0, 1.0), //a4
	point4(-0.8,-1.0 ,0.0, 1.0), //a5*/
	point4(0.0, 0.0 ,0.0, 1.0), //a0
	point4(0.0, 0.4 ,0.0, 1.0), //a1
	point4(0.1, 0.4 ,0.0, 1.0), //a2
	point4(0.1, 0.1 ,0.0, 1.0), //a3
	point4(0.2, 0.1 ,0.0, 1.0), //a4
	point4(0.2, 0.0 ,0.0, 1.0), //a5
};

//point4 referencePoint = point4(-0.9, -1.0, 0.0, 1.0);
point4 referencePoint = point4(0.1, 0.0, 0.0, 1.0);

// RGBA colors
color4 vertex_colors[6] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
}; 

GLfloat  ThetaValue[3] = { 0.0, 0.0, 0.0 };
GLfloat  TranslateOriginValue[3] = { 0.0, 0.0, 0.0 };
GLfloat  TranslateMousePosValue[3] = { 0.0, 0.0, 0.0 };
GLfloat  TranslateRotatingValue[3] = { 0.0, 0.0, 0.0 };

GLfloat degree = 30;
int mouseX = 0;
int mouseY = 0;

GLuint  theta;  // The location of the "theta" shader uniform variable
GLuint translateOriginValue;// The location of the "translate" shader uniform variable
GLuint translateMousePosValue;
GLuint translateRotatingValue;
GLuint vao;

int Index = 0;
//----------------------------------------------------------------------------
void singleRotationMode();
void animationMode(int id);
void quad(int a, int b, int c, int d);
void fillColorsandPoints();
// quad generates two triangles for each face and assigns colors
//    to the vertices

void quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = LShape[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = LShape[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = LShape[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = LShape[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = LShape[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = LShape[d]; Index++;
}

//----------------------------------------------------------------------------

void fillColorsandPoints()
{
	quad(0, 1, 2, 3);
	quad(3, 4, 5, 0);
}

//----------------------------------------------------------------------------


// OpenGL initialization
void init()
{
	fillColorsandPoints();

	//printf("Pressing left, middle and right  mouse buttons will change the rotation axis to X, Y, and Z, respectively.\n");;

	// Create a vertex array object
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	theta = glGetUniformLocation(program, "theta");
	translateOriginValue = glGetUniformLocation(program, "translateOriginValue");
	translateMousePosValue = glGetUniformLocation(program, "translateMousePosValue");
	translateRotatingValue = glGetUniformLocation(program, "translateRotatingValue");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vao);
	glUniform3fv(translateRotatingValue, 1, TranslateRotatingValue);
	glUniform3fv(translateMousePosValue, 1, TranslateMousePosValue);
	glUniform3fv(theta, 1, ThetaValue);
	glUniform3fv(translateOriginValue, 1, TranslateOriginValue);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glutSwapBuffers();
	//printf("OpenGL version supported %f %f\n", LShape[0][0], LShape[1][1]);
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'r': case 'R':
		if (Mode == SingleRotationMode)
		{
			degree = degree + 5;
			singleRotationMode();
		}
		break;
	case 'a': case 'A':
		Mode = AnimationMode;
		rotatingDegree = 0;
		TranslateMousePosValue[0] = 0;
		TranslateMousePosValue[1] = 0;
		glutTimerFunc(750, animationMode, 0);
		break;
	}

}

//----------------------------------------------------------------------------
void animationMode(int id)
{
	if (Mode != AnimationMode) return;
	rotatingDegree += 360.0 / n;
	if (rotatingDegree >= 360) rotatingDegree = 0;
	//translate ref point to (0,0)
	TranslateOriginValue[0] = 0 - referencePoint[0];
	TranslateOriginValue[1] = 0 - referencePoint[1];
	TranslateOriginValue[2] = 0;

	//rotate 360/n clockwise direction
	
	ThetaValue[0] = 0;
	ThetaValue[1] = 0;
	ThetaValue[2] = rotatingDegree * -1;

	//translate to rotating location

	//printf("OpenGL version supported %f \n", sin(rotatingDegree) * radius);
	TranslateRotatingValue[0] =  sin(rotatingDegree * DegreesToRadians) * radius;
	TranslateRotatingValue[1] =  cos(rotatingDegree * DegreesToRadians) * radius;
	TranslateRotatingValue[2] = 0;

	glutPostRedisplay();
	glutTimerFunc(750, animationMode, 0);

	
}

//----------------------------------------------------------------------------

void singleRotationMode()
{
	//translate ref point to (0,0)

	TranslateOriginValue[0] = 0 - referencePoint[0];
	TranslateOriginValue[1] = 0 - referencePoint[1];
	TranslateOriginValue[2] = 0;

	//rotate 30 clockwise counter direction
	ThetaValue[0] = 0;
	ThetaValue[1] = 0;
	ThetaValue[2] = degree;

	//translate ref point to mouse coordination

/*
	GLfloat x = mouseX / 250.0;
	GLfloat y = mouseY / 250.0;
	TranslateMousePosValue[0] = referencePoint[0] + x - 0.1;
	TranslateMousePosValue[1] = referencePoint[1] + y;
	TranslateMousePosValue[2] = 0;*/
	GLfloat x = mouseX / 250.0;
	GLfloat y = mouseY / 250.0;
	TranslateMousePosValue[0] = referencePoint[0] - 1 + x - 0.1;
	TranslateMousePosValue[1] = referencePoint[1] - 1 + y;
	TranslateMousePosValue[2] = 0;
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:     break;
		case GLUT_MIDDLE_BUTTON:   break;
		case GLUT_RIGHT_BUTTON: 
			Mode = SingleRotationMode;
			TranslateRotatingValue[0] = 0;
			TranslateRotatingValue[1] = 0;
			degree = 30;

			mouseX = x;
			mouseY = wh - y;
			singleRotationMode();
			break;
		}
	}
}

void reshapeFunc(GLsizei w, GLsizei h)
{
	/* adjust clipping box */


	/* adjust viewport and clear */

	glViewport(0, 0, ww*h / wh, h);

	/* set global size for use by drawing routine */

	//ww = w;
	//wh = h;
	//resetGame();
	glutPostRedisplay();
}
//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(ww, wh);

	// set OpenGL context to 3.1 or 3.2 and 
	// set profile to core
	// deprecated functions will not be available in core profile
	// the other option is GLUT_COMPATIBILITY PROFILE which let's us use deprecated functionality if
	// vendor implementation provide it

	//glutInitContextVersion(3, 1);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	//create graphics window
	glutCreateWindow("L Shape");

	//include the following statement due to an error in GLEW library
	glewExperimental = GL_TRUE;

	glewInit();

	init();
	glutReshapeFunc(reshapeFunc);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutTimerFunc(750, animationMode, 0);
	glutMainLoop();
	return 0;
}
