#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glut.h>

using namespace std;

// --------------- Global Data ---------------
GLint win_size_x;
GLint win_size_y;
GLdouble glut_x;
GLdouble glut_y;
GLuint vtxPosition;
GLuint vtxColor;
GLuint matLoc;
GLuint buf[1];
GLuint colorBuf[1];
GLfloat color_red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat color_green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat color_blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat matrix[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, 0.0f, 1.0f,
};
GLfloat matrix2[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.0f, 0.0f, 1.0f,
};
GLfloat cubeVertices[] = {
    0.1f, 0.1f, -0.1f,
    -0.1f, -0.1f, -0.1f,
    -0.1f, 0.1f, -0.1f,
    0.1f, 0.1f, -0.1f,
    0.1f, -0.1f, -0.1f,
    -0.1f, -0.1f, -0.1f,

    -0.1f, -0.1f, -0.1f,
    -0.1f, -0.1f, 0.1f,
    -0.1f, 0.1f, 0.1f,
    -0.1f, -0.1f, -0.1f,
    -0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, -0.1f,

    -0.1f, 0.1f, 0.1f,
    -0.1f, -0.1f, 0.1f,
    0.1f, -0.1f, 0.1f,
    0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, 0.1f,
    0.1f, -0.1f, 0.1f,

    0.1f, 0.1f, 0.1f,
    0.1f, -0.1f, -0.1f,
    0.1f, 0.1f, -0.1f,
    0.1f, -0.1f, -0.1f,
    0.1f, 0.1f, 0.1f,
    0.1f, -0.1f, 0.1f,

    0.1f, -0.1f, 0.1f,
    -0.1f, -0.1f, -0.1f,
    0.1f, -0.1f, -0.1f,
    0.1f, -0.1f, 0.1f,
    -0.1f, -0.1f, 0.1f,
    -0.1f, -0.1f, -0.1f,

    0.1f, 0.1f, 0.1f,
    0.1f, 0.1f, -0.1f,
    -0.1f, 0.1f, -0.1f,
    0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, -0.1f,
    -0.1f, 0.1f, 0.1f,
};

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

void lineKeyboard(unsigned char key, int x, int y) {
  if (key == 'd') {
    matrix[12] += 0.01;
  } else if (key == 'w') {
    matrix[13] += 0.01;
  } else if (key == 'x') {
    matrix[14] += 0.01;
  } else if (key == 's') {
    matrix[12] -= 0.01;
  } else if (key == 'a') {
    matrix[13] -= 0.01;
  } else if (key == 'z') {
    matrix[14] -= 0.01;
  } else if (key == 'i') {
    matrix2[13] += 0.01;
  } else if (key == 'j') {
    matrix2[13] -= 0.01;
  } else if (key == 'k') {
    matrix2[12] -= 0.01;
  } else if (key == 'l') {
    matrix2[12] += 0.01;
  } else if (key == 'n') {
    matrix2[14] -= 0.01;
  } else if (key == 'm') {
    matrix2[14] += 0.01;
  }
  glutPostRedisplay();
}

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutKeyboardFunc(lineKeyboard);

  glEnableVertexAttribArray(vtxPosition);
//  glEnableVertexAttribArray(vtxColor);

  // Left
  glUniformMatrix4fv(matLoc, 1, GL_FALSE, matrix);
  glUniform4fv(vtxColor, 1, color_red);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

//  glBufferData(GL_ARRAY_BUFFER, 4, color_red_array.data(), GL_STATIC_DRAW);
//  glVertexAttribPointer(vtxColor, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Right
  glUniformMatrix4fv(matLoc, 1, GL_FALSE, matrix2);
  glUniform4fv(vtxColor, 1, color_blue);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

//  glBufferData(GL_ARRAY_BUFFER, 4, color_blue_array.data(), GL_STATIC_DRAW);
//  glVertexAttribPointer(vtxColor, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  glutSwapBuffers();
}

void init() {
  GLenum res;
  if ((res = glewInit()) != GLEW_OK) {
    fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
  }

  glGenBuffers(1, buf);
  glBindBuffer(GL_ARRAY_BUFFER, buf[0]);

  glGenBuffers(1, colorBuf);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuf[0]);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

int main(int argc, char **argv) {
  win_size_x = 500;
  win_size_y = 500;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(win_size_x, win_size_y);
  glutCreateWindow("HW8");

  init();

  GLuint programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
  glUseProgram(programID);

  vtxPosition = glGetAttribLocation(programID, "vtxPosition");
  vtxColor = glGetUniformLocation(programID, "vtxColor");
  matLoc = glGetUniformLocation(programID, "matrix");

  glutDisplayFunc(renderScene);
  glutMainLoop();

  return 1;
}