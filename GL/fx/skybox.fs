#version 400

out vec4 frag_colour;
in vec3 pos;

uniform samplerCube sampler0;

void main(){

    frag_colour = texture( sampler0, normalize( pos ) );
}
