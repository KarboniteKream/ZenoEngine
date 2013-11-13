#version 120

attribute vec2 TexturePosition;

attribute vec2 TexCoord;
varying vec2 texCoord;

void main()
{
	texCoord = TexCoord;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(TexturePosition.x, TexturePosition.y, 0.0f, 1.0f);
}
