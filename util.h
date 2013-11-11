#ifndef UTIL_H
#define UTIL_H

#include "globals.h"

void loadExtensions();
void drawRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void drawEmptyRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat lineWidth, GLfloat r, GLfloat g, GLfloat b);
void saveScreenshot();
void loadShader(GLuint *shaderProgram, const char *vertexShaderFilename, const char *fragmentShaderFilename);
void printShaderLog(GLuint shader);
void printLog(char **logString, int *logIndex, const char *text);

#endif
