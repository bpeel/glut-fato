#include "config.h"

#include <stdlib.h>
#include <epoxy/gl.h>
#include <GL/freeglut.h>

static void
display_cb(void)
{
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 0.0f, 0.0f);

        glBegin(GL_TRIANGLES);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(0.0f, 1.0f);
        glEnd();

        glutSwapBuffers();
}

int
main(int argc, char **argv)
{
        glutInit(&argc, argv);
        glutCreateWindow("Triangle");
        glutDisplayFunc(display_cb);

        glutMainLoop();

        return EXIT_SUCCESS;
}
