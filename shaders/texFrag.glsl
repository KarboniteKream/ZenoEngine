#version 120

uniform sampler2D TextureUnit;
uniform vec4 TextureColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

varying vec2 texCoord;

void main()
{
	gl_FragColor = texture2D(TextureUnit, texCoord) * TextureColor;
}
