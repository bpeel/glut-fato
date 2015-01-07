#include "config.h"

#include <stdlib.h>
#include <stdint.h>
#include <epoxy/gl.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include <stdio.h>

#include "fato-util.h"

#define TEX_SIZE 512
#define TEX_LAYERS 64

struct ffs_data {
        GLuint program;
        GLuint position_attrib;

        GLuint tex;
        GLuint pbo;

        GLuint layer_uniform;
};

static bool
init_gl(struct ffs_data *data)
{
        GLuint uniform;
        uint8_t *buf;
        int i;

        data->program =
                fato_create_program_from_files("fato-array-vertex.glsl",
                                               "fato-array-fragment.glsl");

        if (data->program == 0)
                return false;

        data->position_attrib = glGetAttribLocation(data->program, "position");

        glGenTextures(1, &data->tex);
        glBindTexture(GL_TEXTURE_2D_ARRAY, data->tex);
        glTexImage3D(GL_TEXTURE_2D_ARRAY,
                     0, /* level */
                     GL_RGBA,
                     TEX_SIZE, TEX_SIZE,
                     TEX_LAYERS,
                     0, /* border */
                     GL_RGBA, GL_UNSIGNED_BYTE,
                     NULL);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        uniform = glGetUniformLocation(data->program, "tex");
        glUseProgram(data->program);
        glUniform1i(uniform, 0);

        data->layer_uniform = glGetUniformLocation(data->program, "layer");

        glGenBuffers(1, &data->pbo);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, data->pbo);
        glBufferData(GL_PIXEL_UNPACK_BUFFER,
                     TEX_SIZE * TEX_SIZE * TEX_LAYERS * 4,
                     NULL, /* data */
                     GL_STATIC_DRAW);

        buf = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        for (i = 0; i < TEX_SIZE * TEX_SIZE * TEX_LAYERS; i++) {
                *(buf++) = rand();
                *(buf++) = rand();
                *(buf++) = (i / (TEX_SIZE * TEX_SIZE)) * 255 / (TEX_LAYERS - 1);
                *(buf++) = 255;
        }
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

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
        int i, src_layer;

        glClear(GL_COLOR_BUFFER_BIT);

        for (i = 0; i < TEX_LAYERS; i++) {
                /* Replace all of the layers of the texture with random layers
                 * from the pbo */

                src_layer = rand() % TEX_LAYERS;

                glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                                0, /* level */
                                0, 0, /* x/y offset */
                                i, /* zoffset */
                                TEX_SIZE, TEX_SIZE,
                                1, /* depth */
                                GL_RGBA, GL_UNSIGNED_BYTE,
                                (GLvoid *) (intptr_t)
                                (src_layer * TEX_SIZE * TEX_SIZE * 4));
        }

        /* Render a random layer */
        glUniform1f(data->layer_uniform, rand() % TEX_LAYERS);

        glEnableVertexAttribArray(data->position_attrib);
        glVertexAttribPointer(data->position_attrib,
                              2, /* size */
                              GL_FLOAT,
                              GL_FALSE, /* normalized */
                              0, /* stride */
                              vertices);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

        glDisableVertexAttribArray(data->position_attrib);

        glutSwapBuffers();

        glutPostRedisplay();
}

int
main(int argc, char **argv)
{
        struct ffs_data data;

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE);
        glutCreateWindow("Texture array");
        glutDisplayFunc(display_cb);

        glutSetWindowData(&data);

        if (!init_gl(&data))
                return EXIT_FAILURE;

        glutMainLoop();

        glDeleteTextures(1, &data.tex);

        glUseProgram(0);
        glDeleteProgram(data.program);

        return EXIT_SUCCESS;
}
