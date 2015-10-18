#version 400

out vec4 frag_colour;
in vec2 tex;

uniform sampler2D sampler;
uniform vec3 lightPos;

void main(){

    if( texture( shadow, normalize( ( mViewI * vec4( normalize( view - lightPos ), 0.0 ) ) ).xyz ).r + 0.01 < length( view - lightPos ) )
      frag_colour = vec4(0,0,0,1);
    else
      frag_colour = vec4(1,1,1,1);

}
