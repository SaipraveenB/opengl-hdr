
#CCARGS = -g -fopenmp -I/usr/include/ImageMagick

#LLARGS = /usr/lib/nvidia-331/libGL.so.1 -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lglut -lMagick++ -lMagickCore
CCARGS = -framework OpenGL -g -fopenmp -I/usr/local/homebrew/Cellar/glew/1.11.0/include -I/usr/local/include -I/usr/local/homebrew/Cellar/imagemagick/6.9.0-10/include/ImageMagick-6 

LLARGS =  -L/usr/local/homebrew/Cellar/glfw3/3.1.1/lib -L/usr/local/homebrew/Cellar/glew/1.11.0/lib -L/usr/local/homebrew/Cellar/imagemagick/6.9.0-10/lib -lglfw3 -lGLEW -lMagick++-6.Q16 -lMagickCore-6.Q16 -framework OpenGL

all: object.o mygl.o texture.o camera.o scene.o loader.o fx_phong.o technique.o light.o skybox.o cube_texture.o render_target.o fx_directdraw.o fx_hdr.o fx_lpass.o fx_bloom.o fx_combine.o fx_project.o fx_shadow.o noise.o fx_scatter.o
	g++ -g $^ $(LLARGS)

object.o: object.cpp object.h
	g++ -c $< -o $@ $(CCARGS)

scene.o: scene.cpp scene.h
	g++ -c $< -o $@ $(CCARGS)

mygl.o: mygl.cpp
	g++ -c $< -o $@ $(CCARGS)

camera.o: camera.cpp
	g++ -c $< -o $@ $(CCARGS)

texture.o: texture.cpp texture.h
	g++ -c $< -o $@ $(CCARGS)

loader.o: loader.cpp loader.h
	g++ -c $< -o $@ $(CCARGS)

technique.o: technique.cpp technique.h
	g++ -c $< -o $@ $(CCARGS)

fx_phong.o: fx_phong.cpp fx_phong.h
	g++ -c $< -o $@ $(CCARGS)

light.o: light.cpp light.h
	g++ -c $< -o $@ $(CCARGS)

skybox.o: skybox.cpp skybox.h
	g++ -c $< -o $@ $(CCARGS)

cube_texture.o: cube_texture.cpp cube_texture.h
	g++ -c $< -o $@ $(CCARGS)

render_target.o: render_target.cpp render_target.h
	g++ -c $< -o $@ $(CCARGS)

fx_directdraw.o: fx_directdraw.cpp fx_directdraw.h
	g++ -c $< -o $@ $(CCARGS)

fx_hdr.o: fx_hdr.cpp fx_hdr.h
	g++ -c $< -o $@ $(CCARGS)

fx_lpass.o: fx_lpass.cpp fx_lpass.h
	g++ -c $< -o $@ $(CCARGS)

fx_bloom.o: fx_bloom.cpp fx_bloom.h
	g++ -c $< -o $@ $(CCARGS)

fx_combine.o: fx_combine.cpp fx_combine.h
	g++ -c $< -o $@ $(CCARGS)

fx_project.o: fx_project.cpp fx_project.h
	g++ -c $< -o $@ $(CCARGS)

fx_shadow.o: fx_shadow.cpp fx_shadow.h
	g++ -c $< -o $@ $(CCARGS)

fx_scatter.o: fx_scatter.cpp fx_scatter.h
	g++ -c $< -o $@ $(CCARGS)

noise.o: noise.cpp noise.h
	g++ -c $< -o $@ $(CCARGS)
