#Cone makefile

textureLab: textureLab.c
	gcc -o textureLab textureLab.c initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations

matLib.o: matLib.h matLib.c
	gcc -c matLib.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL
