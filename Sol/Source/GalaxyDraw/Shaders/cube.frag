#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec3 Normal;
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_EntityID;

void main()
{
	vec4 texColor = Input.Color;

	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
	o_EntityID = v_EntityID;
}