#Cone makefile

proj: proj.c matLib.o
	gcc -o proj proj.c matLib.o initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations

matLib.o: matLib.h matLib.c
	gcc -c ../../common/matLib.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL

sphere: sphere.c matLib.o
	gcc -o sphere sphere.c matLib.o initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations

torus: torus.c matLib.o
	gcc -o torus torus.c matLib.o initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations

spring: spring.c matLib.o
	gcc -o spring spring.c matLib.o initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations

sphere2: sphere.c matLib.o
	gcc -o sphere sphere.c matLib.o initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations