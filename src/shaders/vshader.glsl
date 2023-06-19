#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
attribute vec4 vertex_pos;
uniform vec4 color;
varying vec4 frag_color;
void main()
{
    gl_Position = mvp_matrix * vertex_pos;
    frag_color = color;
}
