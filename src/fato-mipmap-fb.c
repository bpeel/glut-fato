#include "config.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <epoxy/gl.h>
#include <GL/freeglut.h>

#define TEX_SIZE 1024

static GLuint tex;
static int width, height;

static uint8_t *
create_tex_data(int size,
                int color_index)
{
        uint8_t color[4];
        uint8_t *data, *p;
        int i;

        data = malloc(size * 4 * size);

        memset(color, 0, sizeof color);
        color[color_index % 3] = 255;
        color[3] = 255;

        for (i = 0, p = data; i < size * size; i++, p += 4)
                memcpy(p, color, sizeof color);

        return data;
}

static GLuint
create_texture(void)
{
        GLuint tex;
        int size = TEX_SIZE;
        uint8_t *data;
        int level = 0;

        glGenTextures(1, &tex);

        glBindTexture(GL_TEXTURE_2D, tex);

        while (size > 0) {
                data = create_tex_data(size, level);
                glTexImage2D(GL_TEXTURE_2D,
                             level,
                             GL_RGBA,
                             size, size,
                             0, /* border */
                             GL_RGBA,
                             GL_UNSIGNED_BYTE,
                             data);
                free(data);

                size /= 2;
                level++;
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        return tex;
}

static void
render_to_texture(GLuint tex,
                  int level)
{
        GLuint fb;

        glGenFramebuffers(1, &fb);
        glBindFramebuffer(GL_FRAMEBUFFER, fb);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               tex,
                               level);

        glViewport(0.0, 0.0, TEX_SIZE >> level, TEX_SIZE >> level);

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(0.0f, 1.0f);
        glEnd();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void
display_cb(void)
{
        glViewport(0.0, 0.0, width, height);

        glBindTexture(GL_TEXTURE_2D, tex);

        glColor3f(1.0f, 1.0f, 1.0f);

        glEnable(GL_TEXTURE_2D);

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2i(0, 0);
        glVertex2i(-1, -1);
        glTexCoord2i(1, 0);
        glVertex2i(1, -1);
        glTexCoord2i(0, 1);
        glVertex2i(-1, 1);
        glTexCoord2i(1, 1);
        glVertex2i(1, 1);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        glutSwapBuffers();
}

static void
reshape_cb(GLint new_width, GLint new_height)
{
        width = new_width;
        height = new_height;
}

int
main(int argc, char **argv)
{
        glutInit(&argc, argv);
        glutCreateWindow("Triangle");
        glutDisplayFunc(display_cb);
        glutReshapeFunc(reshape_cb);

        tex = create_texture();
        render_to_texture(tex, 1);

        glutMainLoop();

        glDeleteTextures(1, &tex);

        return EXIT_SUCCESS;
}
