#version 400

out vec4 frag_colour;
in vec3 pos;
void main(){

    frag_colour = length( pos ) * vec4( 1, 1, 1, 1 );
    frag_colour.w = 1.0;

}
