#Cone makefile

proj4: proj4.c matLib.o
	gcc -o proj4 proj4.c solve_rc.c matLib.o initShader.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated-declarations

matLib.o: ../../common/matLib.h ../../common/matLib.c
	gcc -c ../../common/matLib.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL
