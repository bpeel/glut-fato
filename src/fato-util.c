#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "fato-util.h"

GLuint
fato_create_shader(GLenum type,
                   const char *source)
{
        GLuint shader;
        GLint info_log_length;
        GLsizei real_info_log_length;
        GLint status;
        char *info_log;

        shader = glCreateShader(type);
        glShaderSource(shader, 1, (const GLchar **) &source, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

        if (info_log_length) {
                info_log = malloc(info_log_length + 1);
                glGetShaderInfoLog(shader,
                                   info_log_length + 1,
                                   &real_info_log_length,
                                   info_log);
                if (real_info_log_length > 0) {
                        fprintf(stderr,
                                "%s shader log:\n",
                                type == GL_FRAGMENT_SHADER ? "fragment"
                                : "vertex");
                        fwrite(info_log, 1, real_info_log_length, stderr);
                }
                free(info_log);
        }

        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (!status) {
                glDeleteShader(shader);
                fprintf(stderr, "shader compilation failed\n");
                return 0;
        }

        return shader;
}

GLuint
fato_create_shader_from_file(GLenum type,
                             const char *filename)
{
        struct stat statbuf;
        char *source;
        FILE *file;
        GLuint shader = 0;

        file = fopen(filename, "r");

        if (file == NULL) {
                fprintf(stderr,
                        "%s: %s\n",
                        filename,
                        strerror(errno));
                return 0;
        }

        if (fstat(fileno(file), &statbuf) == -1) {
                fprintf(stderr,
                        "%s: %s\n",
                        filename,
                        strerror(errno));
                goto done_file;
        }

        source = malloc(statbuf.st_size + 1);
        if (fread(source, 1, statbuf.st_size, file) != statbuf.st_size) {
                fprintf(stderr,
                        "%s: %s\n",
                        filename,
                        strerror(errno));
                goto done_source;
        }

        shader = fato_create_shader(type, source);

done_source:
        free(source);
done_file:
        fclose(file);

        return shader;
}

GLuint
fato_create_program(const char *vertex_shader_source,
                    const char *fragment_shader_source)
{
        GLuint fragment_shader, vertex_shader;
        GLuint program;

        vertex_shader = fato_create_shader(GL_VERTEX_SHADER,
                                           vertex_shader_source);

        if (vertex_shader == 0)
                return 0;

        fragment_shader = fato_create_shader(GL_FRAGMENT_SHADER,
                                             fragment_shader_source);

        if (fragment_shader == 0) {
                glDeleteShader(vertex_shader);
                return 0;
        }

        program = fato_create_program_from_shaders(vertex_shader,
                                                   fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
}

GLuint
fato_create_program_from_files(const char *vertex_shader_file,
                               const char *fragment_shader_file)
{
        GLuint fragment_shader, vertex_shader;
        GLuint program;

        vertex_shader = fato_create_shader_from_file(GL_VERTEX_SHADER,
                                                     vertex_shader_file);

        if (vertex_shader == 0)
                return 0;

        fragment_shader = fato_create_shader_from_file(GL_FRAGMENT_SHADER,
                                                       fragment_shader_file);

        if (fragment_shader == 0) {
                glDeleteShader(vertex_shader);
                return 0;
        }

        program = fato_create_program_from_shaders(vertex_shader,
                                                   fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
}

GLuint
fato_create_program_from_shaders(GLuint vertex_shader,
                                 GLuint fragment_shader)
{
        GLint status;
        GLint info_log_length;
        GLsizei real_info_log_length;
        char *info_log;
        GLuint program;

        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);

        glLinkProgram(program);

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

        if (info_log_length) {
                info_log = malloc(info_log_length + 1);
                glGetProgramInfoLog(program,
                                    info_log_length + 1,
                                    &real_info_log_length,
                                    info_log);
                if (real_info_log_length > 0) {
                        fprintf(stderr, "program log:\n");
                        fwrite(info_log, 1, real_info_log_length, stderr);
                }
                free(info_log);
        }

        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (!status) {
                fprintf(stderr, "program link failed\n");
                glDeleteProgram(program);
                return 0;
        }

        return program;
}
