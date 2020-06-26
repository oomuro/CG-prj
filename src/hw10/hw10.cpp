#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
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
glm::mat4 dummy;
glm::mat4 dummy2;
GLfloat o_size;
GLfloat p_angle;
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

void cameraKeyboard(unsigned char key, int x, int y) {
  if (key == 'o') { // orthogonal
    o_size = 10.0f;
    dummy = glm::ortho(-o_size, o_size, -o_size, o_size, -100.0f, 100.0f);
  } else if (key == 'p') { // perspective
    p_angle = 2.945f;
    dummy = glm::perspective(p_angle, 1.0f, 0.1f, 100.0f);
  } else if (key == 'r') { // pan ++
    dummy2 = glm::rotate(dummy2, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
  } else if (key == 'f') { // pan --
    dummy2 = glm::rotate(dummy2, -0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
  } else if (key == 't') { // tilt ++
    dummy2 = glm::rotate(dummy2, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
  } else if (key == 'g') { // tilt --
    dummy2 = glm::rotate(dummy2, -0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
  } else if (key == 'q') { // crab ++
    dummy2 = glm::translate(dummy2, glm::vec3(0.0f, 0.1f, 0.0f));
  } else if (key == 'a') { // crab --
    dummy2 = glm::translate(dummy2, glm::vec3(0.0f, -0.1f, 0.0f));
  } else if (key == 'w') { // ped ++
    dummy2 = glm::translate(dummy2, glm::vec3(0.1f, 0.0f, 0.0f));
  } else if (key == 's') { // ped --
    dummy2 = glm::translate(dummy2, glm::vec3(-0.1f, 0.0f, 0.0f));
  } else if (key == 'e') { // track ++
    dummy2 = glm::translate(dummy2, glm::vec3(0.0f, 0.0f, 0.1f));
  } else if (key == 'd') { // track --
    dummy2 = glm::translate(dummy2, glm::vec3(0.0f, 0.0f, -0.1f));
  } else if (key == 'z') { // zoom ++
    if (p_angle <= 3.13) {
      p_angle += 0.001f;
    }
    dummy = glm::perspective(p_angle, 1.0f, 0.1f, 100.0f);
  } else if (key == 'x') { // zoom --
    if (p_angle >= 2.9f) {
      p_angle -= 0.001f;
    }
    dummy = glm::perspective(p_angle, 1.0f, 0.1f, 100.0f);
  }

  glutPostRedisplay();
}

void cameraKeyboard2(int key, int x, int y) {
  if (key == GLUT_KEY_LEFT) { // min/max ++
    o_size += 0.1f;
    dummy = glm::ortho(-o_size, o_size, -o_size, o_size, -100.0f, 100.0f);
  } else if (key == GLUT_KEY_RIGHT) { // min/max --
    if (o_size >= 0.2f) {
      o_size -= 0.1f;
      dummy = glm::ortho(-o_size, o_size, -o_size, o_size, -100.0f, 100.0f);
    }
  } else if (key == GLUT_KEY_UP) { // fov ++
    if (p_angle <= 3.13) {
      p_angle += 0.001f;
    }
    dummy = glm::perspective(p_angle, 1.0f, 0.1f, 100.0f);
  } else if (key == GLUT_KEY_DOWN) { // fov --
    if (p_angle >= 2.9f) {
      p_angle -= 0.001f;
    }
    dummy = glm::perspective(p_angle, 1.0f, 0.1f, 100.0f);
  }

  glutPostRedisplay();
}

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutKeyboardFunc(cameraKeyboard);
  glutSpecialFunc(cameraKeyboard2);

  for (int idxX = -10; idxX <= 10; idxX++) {
    for (int idxY = -10; idxY <= 10; idxY++) {
      glm::mat4 wmat(glm::translate(glm::vec3(idxX, idxY, 0.0f)));
      glm::mat4 mvp = dummy * dummy2 * wmat;
      glm::vec4 c((1.0f - (idxX + 10) / 40.0f) - (idxY + 10) / 40.0f, (idxX + 10) / 20.0f, (idxY + 10) / 20.0f, 1.0f);

      glUniformMatrix4fv(matLoc, 1, GL_FALSE, &mvp[0][0]);

      glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
      glEnableVertexAttribArray(vtxPosition);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
      glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

      glUniform4fv(vtxColor, 1, &c[0]);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
  }

  glutSwapBuffers();
}

void init() {
  GLenum res;
  if ((res = glewInit()) != GLEW_OK) {
    fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
  }

  glGenBuffers(1, buf);
  glGenBuffers(1, colorBuf);
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
  glutCreateWindow("HW10");

  init();

  GLuint programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
  glUseProgram(programID);

  vtxPosition = glGetAttribLocation(programID, "vtxPosition");
  vtxColor = glGetUniformLocation(programID, "vtxColor");
  matLoc = glGetUniformLocation(programID, "matrix");

  dummy = glm::mat4(1.0f);
  dummy2 = glm::mat4(1.0f);

  glutDisplayFunc(renderScene);
  glutMainLoop();

  return 1;
}