#version 400

out vec4 frag_colour;
in vec2 tex;

uniform sampler2D sampler;

void main(){

    frag_colour = texture( sampler, tex );

}
