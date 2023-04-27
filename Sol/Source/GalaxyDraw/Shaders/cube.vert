#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;

//_____________Instanced attributes
layout(location = 4) in int a_EntityID;
//TODO add materialIndex as a attribute or uniform block
layout(location = 5) in vec4 a_MeshColor;
layout(location = 6) in mat4 a_EntityTransform;//when using mat4 the next location is +4 from this location
layout(location = 10) in mat4 a_MeshTransform;//when using mat4 the next location is +4 from this location

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};


struct VertexOutput
{
    vec3 Normal;
	vec4 Color;
	vec2 TexCoord;
};

struct AmbientLight
{
	float Intentsity;
	vec3 Color;
};

layout (location = 0) out VertexOutput v_VertOutput;
layout (location = 4) out AmbientLight v_LightOutput;
layout (location = 6) out flat int v_EntityID;

void main()
{
	v_VertOutput.Normal = a_Normal;  
	v_VertOutput.Color = a_MeshColor;
	v_VertOutput.TexCoord = a_TexCoord;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * a_EntityTransform * a_MeshTransform * vec4(a_Position, 1.0);
}