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
GLuint inNormal;
GLuint lightPos;
GLuint lightColor;
GLuint camPos;
GLuint vao;
GLuint vboIdx;
GLuint vboVtx;
GLuint vboNorm;
GLuint stackCount, sectorCount;
const GLdouble pi = 3.14159265358979323846;
std::vector<GLfloat> vertices, normals;
std::vector<GLint> indices;
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

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::vec4 c(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 m(1.0f);
  glUniform4fv(vtxColor, 1, &c[0]);
  glUniformMatrix4fv(matLoc, 1, GL_FALSE, &m[0][0]);
  glUniform3fv(lightPos, 1, &glm::vec3(1.0f, 1.0f, -1.0f)[0]);
  glUniform4fv(lightColor, 1, &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)[0]);
  glUniform3fv(camPos, 1, &glm::vec3(0.0f, 0.0f, -1.0f)[0]);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vboVtx);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
  glEnableVertexAttribArray(vtxPosition);

  glBindBuffer(GL_ARRAY_BUFFER, vboNorm);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
  glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
  glEnableVertexAttribArray(inNormal);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIdx);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIdx);
  glDrawElements(GL_QUAD_STRIP, indices.size(), GL_UNSIGNED_INT, nullptr);

  glutSwapBuffers();
}

void init() {
  GLenum res;
  if ((res = glewInit()) != GLEW_OK) {
    fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
  }

  glGenBuffers(1, &vao);
  glGenBuffers(1, &vboVtx);
  glGenBuffers(1, &vboNorm);
  glGenBuffers(1, &vboIdx);
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
  glutCreateWindow("HW11-2");

  init();

  GLuint programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
  glUseProgram(programID);

  vtxPosition = glGetAttribLocation(programID, "vtxPosition");
  inNormal = glGetAttribLocation(programID, "inNormal");
  vtxColor = glGetUniformLocation(programID, "vtxColor");
  matLoc = glGetUniformLocation(programID, "matrix");
  lightPos = glGetUniformLocation(programID, "lightPos");
  lightColor = glGetUniformLocation(programID, "lightColor");
  camPos = glGetUniformLocation(programID, "camPos");

  // make sphere vertices
  stackCount = 100;
  sectorCount = 100;
  GLfloat rad = 0.7f;
  GLfloat x, y, z, xy;
  GLfloat nx, ny, nz, lengthInv = 1.0f / rad;
  GLfloat sectorStep = 2 * pi / sectorCount;
  GLfloat stackStep = pi / stackCount;
  GLfloat sectorAngle, stackAngle;

  for (int i = 0; i <= stackCount; i++) {
    stackAngle = pi / 2 - i * stackStep;
    xy = rad * cosf(stackAngle);
    z = rad * sinf(stackAngle);

    for (int j = 0; j <= sectorCount; j++) {
      sectorAngle = j * sectorStep;

      x = xy * cosf(sectorAngle);
      y = xy * sinf(sectorAngle);
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);

      nx = x * lengthInv;
      ny = y * lengthInv;
      nz = z * lengthInv;
      normals.push_back(nx);
      normals.push_back(ny);
      normals.push_back(nz);
    }
  }

  int k1, k2;
  for (int i = 0; i < stackCount; i++) {
    k1 = i * (sectorCount + 1);
    k2 = k1 + sectorCount + 1;

    for (int j = 0; j < sectorCount; j++, k1++, k2++) {
      if (i != 0) {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }

      if (i != (stackCount - 1)) {
        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }

  glutDisplayFunc(renderScene);
  glutMainLoop();

  return 0;
}