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
layout(location = 5) in flat int v_TextureID;

layout(binding = 0) uniform sampler2DArray u_Textures;
//TODO add more bindings for different texture types we might want

void main()
{
	vec4 texColor = v_Input.Color*texture(u_Textures, vec3(v_Input.TexCoord,float(0)));
	//TODO make sure this below is correct and works across instances
	//texColor *= texture(u_diffuseTexture[v_TextureID], v_Input.TexCoord);

	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
	o_EntityID = v_EntityID;
}