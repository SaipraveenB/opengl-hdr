#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec2 texColorIn;
layout (location = 2) in vec3 normalIn;

uniform mat4 mWVP;
uniform mat4 mWorld;
uniform mat4 mView;
uniform mat4 mProj;

out vec3 pos;

void main(){
    vec4 temp = mView * mWorld * vec4( vp, 1.0 );
    vec4 temp2 = mWVP * vec4( vp, 1.0 );
    temp.x = temp.x / temp.z;
    temp.y = temp.y / temp.z;
    temp.z = temp2.z / temp2.w;
    temp.w = 1.0;

    gl_Position = temp;

    pos = ( mView * mWorld * vec4( vp, 1.0 ) ).xyz;

}
