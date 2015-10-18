#version 400

out vec4 frag_colour;
in vec2 tex;

uniform sampler2DMS sampler;
uniform int sampleCount;

void main(){

    frag_colour = vec4( 0, 0, 0, 0 );
    ivec2 txsUV = ivec2(floor( tex * textureSize( sampler ) ));
    for( int i = 0; i < sampleCount; i++ ){
      frag_colour += texelFetch( sampler, txsUV , i ) / sampleCount;
    }
    //vec4 col = texelFetch( sampler, txsUV , 0 );
    //frag_colour = vec4( tex, 1.0, 1.0 );

}
