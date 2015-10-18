#version 400

out vec4 frag_colour;
in vec2 tex;

uniform sampler2D sampler;
uniform float fThreshold;

vec4 hdrTone2( vec4 hdrCol ){
  vec4 comp = ((hdrCol)*0.8f)/(vec4(1,1,1,1) + (hdrCol*0.8f));
  comp.a = 1.0;
  return comp;
}

void main(){

    vec4 sample64 = texture( sampler, tex );

    if( (sample64.r * 0.27 + sample64.g * 0.67 + sample64.b * 0.06) > fThreshold )
      frag_colour = hdrTone2( sample64 );
    else
      frag_colour = vec4(0,0,0,0);

}
