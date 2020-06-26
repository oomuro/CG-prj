#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glut.h>

using namespace std;

// --------------- Global Data ---------------
int win_size_x;
int win_size_y;
double glut_x;
double glut_y;
GLuint vtxPosition;
GLuint a_Color;
GLuint buf[1];
GLfloat vertices[2][7] = {
    {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0},
    {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0},
};
int line_cnt = 0;
int start_color = 0;
int end_color = 0;
// -------------------------------------------

GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path) {
  //create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  GLint Result = GL_FALSE;
  int InfoLogLength;

  //Read the vertex shader code from the file
  string VertexShaderCode;
  ifstream VertexShaderStream(vertex_file_path, ios::in);
  if (VertexShaderStream.is_open()) {
    string Line = "";
    while (getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }

  //Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const *VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  //Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  vector<char> VertexShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

  //Read the fragment shader code from the file
  string FragmentShaderCode;
  ifstream FragmentShaderStream(fragment_file_path, ios::in);
  if (FragmentShaderStream.is_open()) {
    string Line = "";
    while (getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }

  //Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const *FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  //Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  vector<char> FragmentShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

  //Link the program
  fprintf(stdout, "Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
  glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
  fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

void lineMouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    if (line_cnt != 2) {
      glut_x = (2.0 * x) / win_size_x - 1.0;
      glut_y = 1.0 - (2.0 * y / win_size_y);
      printf("Left button pressed in x = %d, y = %d\n", x, y);
      printf("glut_x = %f, glut_y = %f\n", glut_x, glut_y);
      vertices[line_cnt][0] = glut_x;
      vertices[line_cnt][1] = glut_y;
      line_cnt++;
    }
  }
  if (line_cnt == 2) {
    line_cnt = 0;
  }
}

void lineKeyboard(unsigned char key, int x, int y) {
  if (key == 's') {
    printf("key 's' pressed\n");
    start_color++;
    if (start_color == 3) {
      vertices[0][3] = 1;
      vertices[0][5] = 0;
    } else if (start_color == 1) {
      vertices[0][3] = 0;
      vertices[0][4] = 1;
    } else if (start_color == 2) {
      vertices[0][4] = 0;
      vertices[0][5] = 1;
    }
  } else if (key == 'e') {
    printf("key 'e' pressed\n");
    end_color++;
    if (end_color == 3) {
      vertices[1][3] = 1;
      vertices[1][5] = 0;
    } else if (end_color == 1) {
      vertices[1][3] = 0;
      vertices[1][4] = 1;
    } else if (end_color == 2) {
      vertices[1][4] = 0;
      vertices[1][5] = 1;
    }
  }

  if (start_color == 3) {
    start_color = 0;
  }
  if (end_color == 3) {
    end_color = 0;
  }

  glutPostRedisplay();
}

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT);

  glutMouseFunc(lineMouse);
  glutKeyboardFunc(lineKeyboard);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, nullptr);
  glVertexAttribPointer(a_Color, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (GLvoid *) (sizeof(GLfloat) * 3));
  glEnableVertexAttribArray(vtxPosition);
  glEnableVertexAttribArray(a_Color);

  if (line_cnt == 0) {
    printf("vertices: (%f %f), (%f %f)\n", vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1]);
    printf("colors: (%d %d %d), (%d %d %d)\n",
           (int) vertices[0][3],
           (int) vertices[0][4],
           (int) vertices[0][5],
           (int) vertices[1][3],
           (int) vertices[1][4],
           (int) vertices[1][5]);
    glDrawArrays(GL_LINES, 0, 2);
  }

  glutSwapBuffers();
}

void init() {
  GLenum res;
  if ((res = glewInit()) != GLEW_OK) {
    fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
  }

  glGenBuffers(1, buf);
  glBindBuffer(GL_ARRAY_BUFFER, buf[0]);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

int main(int argc, char **argv) {
  win_size_x = 500;
  win_size_y = 500;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(win_size_x, win_size_y);
  glutCreateWindow("HW6-2");

  init();

  GLuint programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
  glUseProgram(programID);

  vtxPosition = glGetAttribLocation(programID, "vtxPosition");
  a_Color = glGetAttribLocation(programID, "a_Color");

  glutDisplayFunc(renderScene);
  glutMainLoop();

  return 1;
}