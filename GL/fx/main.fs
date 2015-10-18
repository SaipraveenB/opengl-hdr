#version 400

out vec4 frag_colour;
in vec4 pos;
in vec2 texColor;
in vec4 diffuse;
in vec3 normal;
in vec3 view;
in vec2 ssPos;

uniform sampler2D sampler0;
uniform samplerCube env;
uniform sampler2D shadow;
uniform sampler2D noise;

uniform vec4 col;

uniform vec3 lightPos[8];
uniform vec3 lightDir[8];
uniform vec4 lightPColor[8];
uniform vec4 lightDColor[8];
uniform int numPLights;
uniform int numDLights;
uniform float fAmbient;
uniform float fSpecularExponent;
uniform float fIntensityPackingConstant;

uniform mat4 mViewI;  // Inverse view matrix. To convert the reflected view vector into world coordinates.

uniform vec3 tex0U;
uniform vec3 tex0V;

//uniform vec3 gradients[0];



vec4 modulateColor( float intensity ){
    vec4 colAt0 = vec4(59.0/255,18.0/255,0,1.0);  // Marble dark brown
    vec4 colAt1 = vec4(0.89,0.69,0.55,1.0);// Marble light brown.
    return (1.0f - intensity) * colAt0 + intensity * ( colAt1 );
}

ivec3 vecToIndex( ivec3 voxelID ){
  return voxelID;
}

float sCurve( float t ){
  return 3 * t * t - 2 * t * t * t;
}
float sCombine( float x, float y, float t ){
  return sCurve(t) * y + sCurve( 1 - t ) * x;
}

vec3 gradients( ivec3 index ){
  return (texelFetch( sampler0, ivec2( index.x, index.y * 50 + index.z ), 0 ).xyz - 0.5f )*2.0f;
}

float computePerlinNoise( vec4 vPos ){
  vec3 csVec = mod( ( vPos.xyz + 1.5f ) * 20.0f, 50.0 );
  ivec3 voxelID = ivec3(floor( csVec ));
  vec3 frac = csVec - voxelID;

  vec3 v000 = gradients( vecToIndex( voxelID + ivec3(0,0,0) ) );
  vec3 v001 = gradients( vecToIndex( voxelID + ivec3(0,0,1) ) );
  vec3 v010 = gradients( vecToIndex( voxelID + ivec3(0,1,0) ) );
  vec3 v100 = gradients( vecToIndex( voxelID + ivec3(1,0,0) ) );
  vec3 v110 = gradients( vecToIndex( voxelID + ivec3(1,1,0) ) );
  vec3 v101 = gradients( vecToIndex( voxelID + ivec3(1,0,1) ) );
  vec3 v011 = gradients( vecToIndex( voxelID + ivec3(0,1,1) ) );
  vec3 v111 = gradients( vecToIndex( voxelID + ivec3(1,1,1) ) );

  float f000 = (dot( v000, voxelID + ivec3(0,0,0) - csVec ) + 0.5f)/2.0f;
  float f001 = (dot( v001, voxelID + ivec3(0,0,1) - csVec ) + 0.5f)/2.0f;
  float f010 = (dot( v010, voxelID + ivec3(0,1,0) - csVec ) + 0.5f)/2.0f;
  float f100 = (dot( v100, voxelID + ivec3(1,0,0) - csVec ) + 0.5f)/2.0f;
  float f110 = (dot( v110, voxelID + ivec3(1,1,0) - csVec ) + 0.5f)/2.0f;
  float f101 = (dot( v101, voxelID + ivec3(1,0,1) - csVec ) + 0.5f)/2.0f;
  float f011 = (dot( v011, voxelID + ivec3(0,1,1) - csVec ) + 0.5f)/2.0f;
  float f111 = (dot( v111, voxelID + ivec3(1,1,1) - csVec ) + 0.5f)/2.0f;


  return sCombine( sCombine( sCombine( f000, f001, frac.z ), sCombine( f010, f011, frac.z ), frac.y ), sCombine( sCombine( f100, f101, frac.z ), sCombine( f110, f111, frac.z ), frac.y ), frac.x );
  //return f000;
}

vec4 computeNoise(){

  //vec4 col0 = texture( sampler0, pos.xz ) + texture( sampler0, pos.xz*2.0f ) * 0.5 + texture( sampler0, pos.xz * 4.0f ) * 0.25 + texture( sampler0, pos.xz * 8.0f ) * 0.125;
  //vec4 col1 = texture( sampler0, pos.yx ) + texture( sampler0, pos.yx*2.0f ) * 0.5 + texture( sampler0, pos.yx * 4.0f ) * 0.25 + texture( sampler0, pos.yx * 8.0f ) * 0.125;
  float col2 = computePerlinNoise( pos/8.0f ) + computePerlinNoise( pos/4.0f )* 0.5 + computePerlinNoise( pos * 0.5 ) * 0.25 + computePerlinNoise( pos * 1.0f ) * 0.125 + computePerlinNoise( pos * 2.0f ) * 0.0625;
  float intensity = (sin( pos.y*7.0 + col2*5.0 ) + 1.0)/2.0;

  //return modulateColor( intensity );
  return vec4(1.0f,1.0f,1.0f,0.0f);
}

void main(){


    // TODO: Instead of computing reflected light vector for each pixel,  compute view reflection vector.

    //vec3 dNormal = normalize( normalize(normal) + dT * normalize( tex0U ) + dCT * normalize( tex0V ) );
    vec3 dNormal = normalize(normal);

    //float fSurfColor = computePerlinNoise( pos/2.0 );
    //vec4 surfColor = vec4( fSurfColor, fSurfColor, fSurfColor, 1.0 );

    vec4 surfColor = computeNoise();

    vec4 netIllumination = vec4(0,0,0,0);

    vec3 reflectedView = normalize( view ) + ( 2 * dot( dNormal, -normalize( view ) ) * dNormal );

    vec4 shadowIntensity = texture( shadow, ssPos );

    for( int i = 0; i < numPLights; i++ ){
      vec3 light0Dir = normalize( view - lightPos[i] );
      float light0Length = length( view - lightPos[i] );

      float light0Power = abs( 4.0/(pow(light0Length,2) * 6.0 + light0Length * 2.0 - 20.0) );
      vec4 lightColor = lightPColor[i];

      float lightIntensity = lightColor.a * fIntensityPackingConstant;// Assume alpha channel is the compressed light intensity.
      lightColor.a = 1.0; // Reset Alpha channel to 1.0

      // per-pixel Diffuse & Ambient terms.
      vec4 this_lumi = clamp( dot( dNormal , normalize( -light0Dir ) ) , 0.0, 1.0 ) * 0.1 * light0Power * lightColor * surfColor;

      // per-pixel Specular
      vec3 reflected = normalize( light0Dir ) + ( 2 * dot( dNormal, -normalize( light0Dir ) ) * dNormal );
      float specular = pow( abs( dot( reflected, normalize( view ) ) ), fSpecularExponent ) * (sign( dot( -reflected, normalize(view) ) ) + 1)/2;

      this_lumi += specular * 0.3 * lightColor;

      netIllumination += (this_lumi) * lightIntensity * shadowIntensity;

    }

    for( int i = 0; i < numDLights; i++ ){

      vec3 light0Dir = lightDir[i];
      vec4 lightColor = lightDColor[i];

      // per-pixel Diffuse & Ambient terms.
      float this_lumi = clamp( dot( dNormal , normalize( -light0Dir ) ) , 0.0, 1.0 ) * 0.8;

      // per-pixel Specular
      vec3 reflected = normalize( light0Dir ) + ( 2 * dot( dNormal, -normalize( light0Dir ) ) * dNormal );
      float specular = pow( abs( dot( reflected, normalize( view ) ) ), fSpecularExponent ) * (sign( dot( -reflected, normalize(view) ) ) + 1)/2;

      this_lumi += specular * 0.5;

      netIllumination += ( this_lumi );

    }

    netIllumination += 0.0 * surfColor;

    frag_colour = netIllumination;

    vec3 temp = ( mViewI * vec4( normalize(reflectedView), 0.0 ) ).xyz;

    frag_colour += 0.8 * texture( env, temp ) * surfColor;

    frag_colour.w = 1.0;


    //frag_colour = vec4(ssPos, 0.0, 1.0);
    //frag_colour = shadowIntensity;
    //float lumi = 1/(pow(light0Length,2) * 4.0 + light0Length * 2.0 - 5.0);
    //frag_colour = vec4( lumi, lumi, lumi, 1.0 );
    //frag_colour = vec4( normalize( light0Dir ), 1.0 );
    //frag_colour = texture2D( sampler0, texColor );
    //frag_colour = vec4( texColor.xy, 1.0 , 1.0 );
}
