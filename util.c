#include "util.h"

void draw_rectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	Coordinate vertex_data[4];

	vertex_data[0].x = x1;
	vertex_data[0].y = y1;
	vertex_data[1].x = x2;
	vertex_data[1].y = y1;
	vertex_data[2].x = x2;
	vertex_data[2].y = y2;
	vertex_data[3].x = x1;
	vertex_data[3].y = y2;

	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertex_data);
		glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_empty_rectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat line_width)
{
	glLineWidth(line_width);

	Coordinate vertex_data[8];

	vertex_data[0].x = x1;
	vertex_data[0].y = y1;
	vertex_data[1].x = x2;
	vertex_data[1].y = y1;
	vertex_data[2].x = x2 - 1.0f;
	vertex_data[2].y = y1;
	vertex_data[3].x = x2 - 1.0f;
	vertex_data[3].y = y2;
	vertex_data[4].x = x2 - 1.0f;
	vertex_data[4].y = y2 - 1.0f;
	vertex_data[5].x = x1;
	vertex_data[5].y = y2 - 1.0f;
	vertex_data[6].x = x1 + 1.0f;
	vertex_data[6].y = y2 - 1.0f;
	vertex_data[7].x = x1 + 1.0f;
	vertex_data[7].y = y1;

	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertex_data);
		glDrawArrays(GL_LINES, 0, 8);
	glDisableClientState(GL_VERTEX_ARRAY);

	glLineWidth(1.0f);
}
