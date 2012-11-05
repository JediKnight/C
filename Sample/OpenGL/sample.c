#include <OpenGL/gl.h>
#include <GLUT/glut.h>

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void mouse(void)
{

}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutCreateWindow("OpenGL Sample");

  glutDisplayFunc(display);
  glutMouseFunc(mouse);

  glutMainLoop();
  return 0;
}
