#include "config.h"

#include <stdlib.h>
#include <stdint.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

static GLuint tex;

#define TEX_SIZE 256
#define WINDOW_SIZE TEX_SIZE

static GLuint
create_tex(void)
{
        GLuint tex;
        GLuint buf;
        uint8_t *data;
        int y, x;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR);

        glGenBuffers(1, &buf);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buf);
        glBufferData(GL_PIXEL_UNPACK_BUFFER,
                     TEX_SIZE * TEX_SIZE * 4,
                     NULL,
                     GL_STATIC_DRAW);
        data = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

        for (y = 0; y < TEX_SIZE; y++) {
                for (x = 0; x < TEX_SIZE; x++) {
                        *(data++) = x;
                        *(data++) = y;
                        *(data++) = 0;
                        *(data++) = 255;
                }
        }

        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

        glTexImage2D(GL_TEXTURE_2D,
                     0, /* level */
                     GL_RGBA,
                     TEX_SIZE, TEX_SIZE,
                     0, /* border */
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     NULL /* data */);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        glDeleteBuffers(1, &buf);

        glBindTexture(GL_TEXTURE_2D, 0);

        return tex;
}

static void
display_cb(void)
{
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, tex);
        glEnable(GL_TEXTURE_2D);

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2i(0, 0);
        glVertex2f(-1.0f, -1.0f);
        glTexCoord2i(1, 0);
        glVertex2f(1.0f, -1.0f);
        glTexCoord2i(0, 1);
        glVertex2f(-1.0f, 1.0f);
        glTexCoord2i(1, 1);
        glVertex2f(1.0f, 1.0f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glutSwapBuffers();
}

int
main(int argc, char **argv)
{
        glutInit(&argc, argv);
        glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
        glutCreateWindow("Look! A triangle!");
        glutDisplayFunc(display_cb);
        glewInit();

        tex = create_tex();

        glutMainLoop();

        return EXIT_SUCCESS;
}
