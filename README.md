# opengl-demo

modern OpenGL demo application for desktop

## Features:
1.	Model loading with Assimp library

2.	Applying phong lighting to imported models

3.	Flying through a scene to observe model from different perspectives

### additional dependencies:
glew,
glfw,
glm,
SOIL,
Assimp 3.1.1

###building on debian based distros (debian, ubuntu, mint)
assuming you have already installed all dependencies mentioned above, also make sure that your OpenGL version is 4.x or best is 4.5

if this is all true, then:

1. clone (or download) the repo
2. cd into the repo
3. firstly type `make clean` (because the build directory already contains built files)
4. Simply type `make` and executable will reside in a build directory
5. make sure `models` and `shaders` directories are placed within the same directory with executable (copy and paste them from root project directory other wise it won't run)
6. enjoy