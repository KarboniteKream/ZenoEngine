#ifndef UTIL_H
#define UTIL_H

#include "globals.h"
#include "level.h"

void initWindow(SDL_Window **window, const char *windowTitle);
void loadExtensions();
void executeCommand(Level *level, const char *command);
void saveScreenshot();

void loadShader(GLuint *shaderProgram, const char *vertexShaderFilename, const char *fragmentShaderFilename);
void printInfoLog(GLuint shaderProgram);

void drawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void drawEmptyRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat lineWidth, GLfloat r, GLfloat g, GLfloat b);

#endif
