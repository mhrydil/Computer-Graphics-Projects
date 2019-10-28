/*
 * initShader.h
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */

#ifndef INITSHADER_H_
#define INITSHADER_H_

#ifdef __APPLE__  // include Mac OS X verions of headers
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif  // __APPLE__


struct Shader
{
    const char*  filename;
    GLenum       type;
    GLchar*      source;
};

GLuint initShader(const char* vertexShaderFile, const char* fragmentShaderFile);


#endif /* INITSHADER_H_ */
