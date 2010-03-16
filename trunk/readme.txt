Naive implementation of ray tracing by umut r. e.
For now:
* brute force ray-object intersection testing
* sphere and plane primitives
* multithreaded, creates one thread per cpu can be customised from Main.cpp line 28
* only reflections
* hard coded objects and materials in Main.cpp@initScene function
* specular and diffuse lighting
* only point light source
* iterative ray tracing
* sends one ray per pixel by default however can be customised from Main.cpp line 28