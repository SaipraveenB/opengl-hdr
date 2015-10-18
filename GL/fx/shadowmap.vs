#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec2 texColorIn;
layout (location = 2) in vec3 normalIn;

out vec2 tex;
out vec3 pos;

uniform vec3 center;
uniform vec3 center

void main(){
  gl_Position = vec4( vp, 1.0 );
}
