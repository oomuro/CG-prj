#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
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
glm::vec3 cam_vec;
glm::mat4 matrix;
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
GLfloat cubeColors[] = {
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,

    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,

    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,

    1.0, 1.0, 0.0, 1.0,
    1.0, 1.0, 0.0, 1.0,
    1.0, 1.0, 0.0, 1.0,
    1.0, 1.0, 0.0, 1.0,
    1.0, 1.0, 0.0, 1.0,
    1.0, 1.0, 0.0, 1.0,

    1.0, 0.0, 1.0, 1.0,
    1.0, 0.0, 1.0, 1.0,
    1.0, 0.0, 1.0, 1.0,
    1.0, 0.0, 1.0, 1.0,
    1.0, 0.0, 1.0, 1.0,
    1.0, 0.0, 1.0, 1.0,

    0.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0,
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

glm::mat3 rotation(glm::vec3 v1, glm::vec3 v2) {
  glm::vec3 nv1 = glm::normalize(v1);
  glm::vec3 nv2 = glm::normalize(v2);
  GLfloat angle = glm::acos(glm::dot(nv1, nv2));
  cout << "angle: " << angle << '\n';
  GLfloat s = glm::cos(angle / 2);
  glm::vec4 cr = glm::vec4(glm::normalize(glm::cross(nv1, nv2)), s);
  GLfloat n = glm::sqrt(1 - s * s);
  cr.x *= n;
  cr.y *= n;
  cr.z *= n;
  cout << "cr: " << cr.x << ' ' << cr.y << ' ' << cr.z << ' ' << cr.w << '\n';
  glm::mat3 rot3 = glm::transpose(glm::mat3(
      1 - 2 * cr.y * cr.y - 2 * cr.z * cr.z, 2 * cr.x * cr.y - 2 * cr.w * cr.z, 2 * cr.x * cr.z + 2 * cr.w * cr.y,
      2 * cr.x * cr.y + 2 * cr.w * cr.z, 1 - 2 * cr.x * cr.x - 2 * cr.z * cr.z, 2 * cr.y * cr.z - 2 * cr.w * cr.x,
      2 * cr.x * cr.z - 2 * cr.w * cr.y, 2 * cr.y * cr.z + 2 * cr.w * cr.x, 1 - 2 * cr.x * cr.x - 2 * cr.y * cr.y
  ));

  return glm::transpose(rot3);
}

void cameraMouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    glut_x = (2.0 * x) / win_size_x - 1.0;
    glut_y = 1.0 - (2.0 * y / win_size_y);
    cout << "x: " << glut_x << " y: " << glut_y << '\n';
    matrix = glm::mat4(-rotation(cam_vec, glm::vec3(glut_x, glut_y, 1)));
  }
}

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutMouseFunc(cameraMouse);

  glUniformMatrix4fv(matLoc, 1, GL_FALSE, &matrix[0][0]);

  glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
  glEnableVertexAttribArray(vtxPosition);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, colorBuf[0]);
  glEnableVertexAttribArray(vtxColor);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);
  glVertexAttribPointer(vtxColor, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

  glDrawArrays(GL_TRIANGLES, 0, 36);

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
  glutCreateWindow("HW9");

  init();

  GLuint programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
  glUseProgram(programID);

  vtxPosition = glGetAttribLocation(programID, "vtxPosition");
  vtxColor = glGetAttribLocation(programID, "vtxColor");
  matLoc = glGetUniformLocation(programID, "matrix");
  matrix = glm::mat4(1.0f);
  cam_vec = glm::vec3(0, 0, 1);

  matrix = glm::mat4(-rotation(cam_vec, glm::vec3(1, 2, 3)));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cout << matrix[i][j] << ' ';
    }
    cout << '\n';
  }

  glutDisplayFunc(renderScene);
  glutMainLoop();

  return 1;
}