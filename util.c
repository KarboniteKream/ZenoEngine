#include "util.h"

void drawRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	Coordinate vertexData[4];

	vertexData[0].X = x1;
	vertexData[0].Y = y1;
	vertexData[1].X = x2;
	vertexData[1].Y = y1;
	vertexData[2].X = x2;
	vertexData[2].Y = y2;
	vertexData[3].X = x1;
	vertexData[3].Y = y2;

	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertexData);
		glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawEmptyRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat lineWidth)
{
	glLineWidth(lineWidth);

	Coordinate vertexData[8];

	vertexData[0].X = x1;
	vertexData[0].Y = y1;
	vertexData[1].X = x2;
	vertexData[1].Y = y1;
	vertexData[2].X = x2 - 1.0f;
	vertexData[2].Y = y1;
	vertexData[3].X = x2 - 1.0f;
	vertexData[3].Y = y2;
	vertexData[4].X = x2 - 1.0f;
	vertexData[4].Y = y2 - 1.0f;
	vertexData[5].X = x1;
	vertexData[5].Y = y2 - 1.0f;
	vertexData[6].X = x1 + 1.0f;
	vertexData[6].Y = y2 - 1.0f;
	vertexData[7].X = x1 + 1.0f;
	vertexData[7].Y = y1;

	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertexData);
		glDrawArrays(GL_LINES, 0, 8);
	glDisableClientState(GL_VERTEX_ARRAY);

	glLineWidth(1.0f);
}
