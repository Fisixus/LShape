
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 12; // 6 vertex
GLuint ww = 500;
GLuint wh = 500;

point4 points[NumVertices];
color4 colors[NumVertices];

point4 LShape[6] = {
	point4(-1.0,-1.0 ,0.0, 1.0), //a0
	point4(-1.0,-0.6 ,0.0, 1.0), //a1
	point4(-0.9,-0.6 ,0.0, 1.0), //a2
	point4(-0.9,-0.9 ,0.0, 1.0), //a3
	point4(-0.8,-0.9 ,0.0, 1.0), //a4
	point4(-0.8,-1.0 ,0.0, 1.0), //a5
};

// RGBA colors
color4 vertex_colors[6] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
};

point4 referencePoint = point4(-0.9, -0.9, 0.0, 1.0);
GLuint  theta = 5;  // The location of the "theta" shader uniform variable

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
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
	GLuint vao;
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

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUniform3fv(theta, 1, Theta);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:     break;
		case GLUT_MIDDLE_BUTTON:   break;
		case GLUT_RIGHT_BUTTON:    break;
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

	glutMainLoop();
	return 0;
}
