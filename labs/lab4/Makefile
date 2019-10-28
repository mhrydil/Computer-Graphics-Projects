#Cone makefile

cone: cone.c matLib.o
	gcc -o cone cone.c matLib.o initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations

matLib.o: matLib.h matLib.c
	gcc -c matLib.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL
