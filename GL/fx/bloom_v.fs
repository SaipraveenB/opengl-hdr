#version 400

out vec4 frag_colour;
in vec2 tex;

uniform sampler2D sampler;
//uniform float fThreshold;

/*const ivec2 offsets[9] = ivec2[9](
ivec2(0,0),
ivec2(0,1),
ivec2(1,0),
ivec2(1,1),
ivec2(0,-1),
ivec2(-1,1),
ivec2(-1,0),
ivec2(-1,-1),
ivec2(1,-1)
);*/


uniform int iRadius;

float weight( int i ){
  return ((iRadius/2.0) - abs(i - iRadius/2.0))/(iRadius/2.0);
}

void main(){
  frag_colour = vec4(0,0,0,0);
  ivec2 txsUV = ivec2(floor( tex * textureSize( sampler, 0 ) ));

  for( int i = 0; i < iRadius; i++ ){
    frag_colour += texelFetch( sampler, txsUV + ivec2(0,i - (iRadius/2)), 0 ) * ( weight(i) ) * 3.0 / iRadius;
  }
}
