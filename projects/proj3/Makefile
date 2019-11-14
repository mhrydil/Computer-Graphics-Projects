#Cone makefile

proj3: proj3.c matLib.o
	gcc -o proj3 proj3.c matLib.o initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations

matLib.o: ../../common/matLib.h ../../common/matLib.c
	gcc -c ../../common/matLib.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL
