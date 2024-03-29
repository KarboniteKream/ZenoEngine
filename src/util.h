#ifndef UTIL_H
#define UTIL_H

#include "globals.h"
#include "level.h"

void initWindow(SDL_Window **window, const char *windowTitle);
void loadExtensions();
void executeCommand(Level *level, char *command);
void saveScreenshot();

void loadShader(GLuint *shaderProgram, const char *vertexShaderFilename, const char *fragmentShaderFilename);
void printInfoLog(GLuint shaderProgram);

void drawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, uint32_t rgb_color, GLfloat alpha);
void drawEmptyRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat lineWidth, uint32_t rgb_color);

#endif
