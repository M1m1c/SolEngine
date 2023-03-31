#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec3 Normal;
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput v_Input;
layout (location = 4) in flat int v_EntityID;

layout(binding = 0) uniform sampler2DArray u_diffuseTexArray;
//TODO add more bindings for different texture types we might want

void main()
{
	vec4 texColor = v_Input.Color;

	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
	o_EntityID = v_EntityID;
}