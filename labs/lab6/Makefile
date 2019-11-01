#Cone makefile

lab6: lab6.c matLib.o
	gcc -Wall -o lab6 lab6.c matLib.o initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations

matLib.o: ../../common/matLib.h ../../common/matLib.c
	gcc -c ../../common/matLib.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL
