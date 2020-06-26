#include <stdlib.h>
#include <GL/glut.h>


void renderScene(void)
{
    //Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT);
    //Let's draw something here
    //...
    
    //Double buffer
    glutSwapBuffers();
    
    //If using SingleBuffer
    //glFlush();
}

void init()
{
    //select the background color
    glClearColor(1.0, 1.0, 0.0, 1.0);
    
}

int main(int argc, char **argv)
{
    //init GLUT and create Window
    //initialize the GLUT
    glutInit(&argc, argv);
    //GLUT_DOUBLE enables double buffering (drawing to a background buffer while the other buffer is displayed)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    //These two functions are used to define the position and size of the window.
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(480, 480);
    //This is used to define the name of the window.
    glutCreateWindow("Simple OpenGL Window");
    
    //call initization function
    init();
    
    //register callbacks
    glutDisplayFunc(renderScene);
    
    //enter GLUT event processing cycle
    glutMainLoop();
    return 1;
}

