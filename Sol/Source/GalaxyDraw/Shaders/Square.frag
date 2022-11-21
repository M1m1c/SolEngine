#version 450 core

layout(location = 0) out vec4 color;
in vec3 v_Position;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
	color = vec4(1.0);//texture(u_Texture,v_TexCoord);
}