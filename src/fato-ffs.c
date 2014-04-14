#include "config.h"

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include <stdio.h>

#include "fato-util.h"

struct ffs_data {
        GLuint program;
        GLuint position_attrib;
};

static bool
init_gl(struct ffs_data *data)
{
        data->program =
                fato_create_program_from_files("fato-ffs-vertex.glsl",
                                               "fato-ffs-fragment.glsl");

        if (data->program == 0)
                return false;

        data->position_attrib = glGetAttribLocation(data->program, "position");

        return true;
}

static void
display_cb(void)
{
        struct ffs_data *data = glutGetWindowData();
        static GLfloat vertices[] = {
                -1.0f, -1.0f,
                1.0f, -1.0f,
                0.0f, 1.0f
        };

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(data->program);

        glEnableVertexAttribArray(data->position_attrib);
        glVertexAttribPointer(data->position_attrib,
                              2, /* size */
                              GL_FLOAT,
                              GL_FALSE, /* normalized */
                              0, /* stride */
                              vertices);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

        glDisableVertexAttribArray(data->position_attrib);

        glUseProgram(0);

        glutSwapBuffers();
}

int
main(int argc, char **argv)
{
        struct ffs_data data;

        glutInit(&argc, argv);
        glutCreateWindow("Triangle");
        glutDisplayFunc(display_cb);
        glewInit();

        glutSetWindowData(&data);

        if (!init_gl(&data))
                return EXIT_FAILURE;

        glutMainLoop();

        return EXIT_SUCCESS;
}
