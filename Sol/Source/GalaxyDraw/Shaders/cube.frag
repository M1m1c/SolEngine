#version 450 core

layout(std140, binding = 4) uniform AmbientLight
{
	vec3 u_AmbientColor;
	float u_Intentsity;
};

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec3 Normal;
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput v_VertInput;
layout (location = 4) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_Texture;
//TODO add more bindings for different texture types we might want

void main()
{

	vec4 texColor = texture(u_Texture, v_VertInput.TexCoord) *
	vec4(v_VertInput.Color.x * u_AmbientColor.x * u_Intentsity,
	v_VertInput.Color.y * u_AmbientColor.y * u_Intentsity,
	v_VertInput.Color.z * u_AmbientColor.z * u_Intentsity,
	v_VertInput.Color.w);
	//vec4(u_Color,1.0);

	//TODO make sure this below is correct and works across instances
	//texColor *= texture(u_diffuseTexture[v_TextureID], v_Input.TexCoord);

	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
	o_EntityID = v_EntityID;
}