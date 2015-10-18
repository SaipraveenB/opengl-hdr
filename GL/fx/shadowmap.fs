#version 400

out vec4 frag_colour;
in vec2 tex;

uniform sampler2D sampler0;
uniform sampler2D sampler1;

void main(){
  vec4 col0 = texture( sampler0, tex );
  vec4 col1 = texture( sampler1, tex );
  frag_colour = col0 * 0.8 + col1 * 1.0;
  //frag_colour = col0 * 0.0 + col1 * 3.0;
}
