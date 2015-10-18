#version 400

out vec4 frag_colour;
in vec2 tex;

uniform sampler2D sampler;
uniform float fExposure;


vec4 hdrTone( vec4 hdrCol ){
  return ( hdrCol - vec4(fDarkPoint,fDarkPoint,fDarkPoint,0.0) )/( fBrightPoint - fDarkPoint );
}

vec4 hdrTone2( vec4 hdrCol ){
  vec4 comp = ((hdrCol)*fExposure)/(vec4(1,1,1,1) + (hdrCol*fExposure));
  comp.a = 1.0;
  return comp;
}

vec4 hdrTone3( vec4 hdrCol ){
  return hdrCol;
}

void main(){

    vec4 sample64 = textureLod( sampler, tex, 0.0f );

    vec4 sample32 = hdrTone2( sample64 );
    frag_colour = sample32;

    //frag_colour = vec4( tex, 1.0, 1.0 );
}
