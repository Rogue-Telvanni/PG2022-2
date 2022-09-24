#version 450

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D tex_buffer;

out vec4 color;

void main()
{
	color = texture(tex_buffer,texCoord);
}