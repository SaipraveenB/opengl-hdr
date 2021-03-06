#version 400

out vec4 frag_colour;
in vec3 view;
//in vec2 shadowUV;

uniform sampler2D shadow;

uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec4 lightConstants;
uniform float fSampleDensity;
uniform mat4 mWVPshm;
uniform mat4 mViewI;

float volumetricRadianceSample( vec3 pt ){
    vec4 temp = ( mWVPshm * ( mViewI * vec4( pt - lightPos , 0.0 ) ) );
    vec2 shadowUV = temp.xy / -temp.w;
    float lightDepth = texture( shadow, (shadowUV + vec2(1.0,1.0) / 2.0) ).z;

    if( lightDepth < length( pt - lightPos ) && ( lightDepth != 0 ) )
      return 0.0f;

    /*if( dot( normalize( pt - lightPos ), normalize( lightDir ) ) < 0.2f )
      return 0.0f;*/

    return 0.05f * clamp( exp( - 36.5f * (1 - dot( normalize( pt - lightPos ), normalize( lightDir ) ) ) ), 0.0, 1.0 ) * clamp( pow( dot( normalize( pt - lightPos ), normalize( -view ) ), 3 ), 0.0, 1.0 )  /(pow( length( pt - lightPos ), 2 ) + 0.5f);
}

void main(){

    float fSamplerLength = 3.0f;
    float fSamplerDelta = 0.03f;
    float fSamplerStart = 0.0f;

    vec3 ray = view;
    vec3 viewDir = normalize( view );

    float netRadiance = 0.0f;
    for( int i = 0; i < 75; i++ ){
      ray += viewDir * 0.1;
      netRadiance += volumetricRadianceSample( ray );
    }

    frag_colour = vec4( netRadiance, netRadiance, netRadiance, 1.0f );
    //frag_colour = vec4( 1,1,1,1 );

    //frag_colour = texture( shadow, normalize( view - lightPos ) );
    //frag_colour = vec4(normalize( view - lightPos ), 1.0);

}
 
