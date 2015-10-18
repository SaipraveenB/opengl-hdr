#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec2 texColorIn;
layout (location = 2) in vec3 normalIn;

uniform mat4 mWorld;
uniform mat4 mView;
uniform mat4 mProj;
uniform mat4 mWVP;
uniform mat4 mWVPi;

out vec3 pos;
void main(){

    // Run an inverse transform to get back the world coordinates of the screen quad's corners.
    pos = ( mWVPi * vec4( vp, 1.0 ) ).xyz;
    gl_Position = vec4( vp, 1.0 );
}
