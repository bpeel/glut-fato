#version 130

uniform sampler2DArray tex;
uniform float layer;

in vec2 tex_coord;

void
main()
{
        gl_FragColor = texture(tex, vec3(tex_coord, layer));
}
