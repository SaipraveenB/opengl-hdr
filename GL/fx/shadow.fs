#version 400

out vec4 frag_colour;
in vec3 view;

uniform samplerCube shadow;
uniform vec3 lightPos;

void main(){

    if( ( texture( shadow, normalize( view - lightPos ) ).r ) < length( view - lightPos ) )
      frag_colour = vec4(0,0,0,1);
    else
      frag_colour = vec4(1,1,1,1);

    //frag_colour = texture( shadow, normalize( view - lightPos ) );
    //frag_colour = vec4(normalize( view - lightPos ), 1.0);

}
