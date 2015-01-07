#ifndef FATO_UTIL_H
#define FATO_UTIL_H

#include <epoxy/gl.h>
#include <stdbool.h>

GLuint
fato_create_shader(GLenum type,
                   const char *source);

GLuint
fato_create_shader_from_file(GLenum type,
                             const char *filename);

GLuint
fato_create_program(const char *vertex_shader_source,
                    const char *fragment_shader_source);

GLuint
fato_create_program_from_shaders(GLuint vertex_shader,
                                 GLuint fragment_shader);

GLuint
fato_create_program_from_files(const char *vertex_shader_file,
                               const char *fragment_shader_file);

#endif /* FATO_UTIL_H */
