#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec2 texColorIn;
layout (location = 2) in vec3 normalIn;

uniform mat4 mWorld;
uniform mat4 mView;
uniform mat4 mProj;
uniform mat4 mWVP;
//uniform vec3 light0Dir;
uniform vec4 light0Col;
//uniform float ambient;

out vec4 pos;
out vec2 texColor;
out vec4 diffuse;
out vec3 normal;
out vec3 view;
out vec2 ssPos;

void main(){
    vec4 temp = mWVP * vec4( vp, 1.0 );

    temp.x = temp.x;
    temp.y = temp.y;
    temp.z = temp.z;
    temp.w = temp.w;

    gl_Position = temp;
    pos = vec4( vp, 1.0 );

    vec4 ssPosTemp = mWVP * vec4( vp, 1.0 );
    ssPos = vec2((ssPosTemp.x / ssPosTemp.w)+1.0f , (ssPosTemp.y / ssPosTemp.w)+1.0f )/2.0f;



    vec4 vsNormal = mView * mWorld * vec4( normalIn, 0.0 );
    //vec4 vsLight0Dir = mView * mWorld * vec4( light0Dir, 0.0 );
    //diffuse.x = diffuse.y = diffuse.z = clamp( clamp( dot( normalize( normalIn ), normalize( light0Dir ) ), 0.0, 1.0 ) + ambient, 0.0, 1.0 );
    //diffuse.w = 1.0;

    view =  (mView * mWorld * vec4( vp, 1.0 ) ).xyz;
    normal = vsNormal.xyz;
    texColor = texColorIn;
}
