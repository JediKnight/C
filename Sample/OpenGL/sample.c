#include <stdio.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void mouse(int button, int state, int x, int y)
{
  char b[5 + 1], s[4 + 1];
  switch(button)
    {
    case GLUT_LEFT_BUTTON:
      sprintf(b, "LEFT");
      break;

    case GLUT_RIGHT_BUTTON:
      sprintf(b, "RIGHT");
      break;
    }

  switch(state)
    {
    case GLUT_DOWN:
      sprintf(s, "DOWN");
      break;

    case GLUT_UP:
      sprintf(s, "UP");
      break;
    }

  fprintf(stdout, "button:%6s, state:%5s, x:%6d, y:%6d\n", b, s, x, y);
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
