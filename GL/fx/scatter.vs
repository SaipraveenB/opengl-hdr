
#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec2 texColorIn;
layout (location = 2) in vec3 normalIn;

uniform mat4 mWVP;
uniform mat4 mWorld;
uniform mat4 mView;
uniform mat4 mProj;

out vec3 view;
//out vec2 shadowUV;

void main(){
    vec4 temp = mWVP * vec4( vp, 1.0 );

    temp.x = temp.x;
    temp.y = temp.y;
    temp.z = temp.z;
    temp.w = temp.w;

    gl_Position = temp;

    view = ( mView * mWorld * vec4( vp, 1.0 ) ).xyz;
}
