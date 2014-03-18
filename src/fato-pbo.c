#include "config.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

static GLuint tex;

#define TEX_WIDTH 32
#define TEX_HEIGHT 128
#define WINDOW_WIDTH TEX_WIDTH
#define WINDOW_HEIGHT TEX_HEIGHT

static GLuint
create_tex(void)
{
        GLuint tex;
        GLuint buf;
        uint8_t *data;
        int i;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR);

        glGenBuffers(1, &buf);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buf);
        glBufferData(GL_PIXEL_UNPACK_BUFFER,
                     TEX_WIDTH * TEX_HEIGHT * 4,
                     NULL,
                     GL_STATIC_DRAW);
        data = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

        for (i = 0; i < TEX_WIDTH * TEX_HEIGHT; i++) {
                *(data++) = 0;
                *(data++) = i >> 8;
                *(data++) = i & 0xff;
                *(data++) = 255;
        }

        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

        glTexImage2D(GL_TEXTURE_2D,
                     0, /* level */
                     GL_RGBA,
                     TEX_WIDTH, TEX_HEIGHT,
                     0, /* border */
                     GL_BGRA,
                     GL_UNSIGNED_BYTE,
                     NULL /* data */);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        glDeleteBuffers(1, &buf);

        glBindTexture(GL_TEXTURE_2D, 0);

        return tex;
}

static void
dump_image(void)
{
        uint8_t buf[WINDOW_WIDTH * 4 * WINDOW_HEIGHT], *p = buf;
        int y, x;

        glReadPixels(0, 0, /* x/y */
                     WINDOW_WIDTH, WINDOW_HEIGHT,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     buf);

        for (y = 0; y < WINDOW_HEIGHT; y++) {
                for (x = 0; x < WINDOW_WIDTH; x++) {
                        printf("%04x ", p[0] | (p[1] << 8));
                        p += 4;
                }
                fputc('\n', stdout);
        }
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

        dump_image();

        glutSwapBuffers();

        glutLeaveMainLoop();
}

int
main(int argc, char **argv)
{
        glutInit(&argc, argv);
        glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        glutCreateWindow("Look! A triangle!");
        glutDisplayFunc(display_cb);
        glewInit();

        tex = create_tex();

        glutMainLoop();

        return EXIT_SUCCESS;
}
