#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec2 texColorIn;
layout (location = 2) in vec3 normalIn;

out vec2 tex;

void main(){
  tex = texColorIn;
  gl_Position = vec4( vp, 1.0 );
}
