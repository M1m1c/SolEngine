#version 450 core

layout(std140, binding = 4) uniform AmbientLight
{
	vec3 u_AmbientColor;
	float u_Intensity;
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
layout (location = 5) in vec3 v_LocalLightDirection;

layout(binding = 0) uniform sampler2D u_Texture;
//TODO add more bindings for different texture types we might want

void main()
{

	vec4 textureSample = texture(u_Texture, v_VertInput.TexCoord);


	vec3 textureColor = vec3(textureSample.x,textureSample.y,textureSample.z);
	vec3 meshColor = vec3(v_VertInput.Color.x,v_VertInput.Color.y,v_VertInput.Color.z);

	vec3 normal = normalize(v_VertInput.Normal);
	vec3 lightDirection = normalize(-v_LocalLightDirection);
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuseColor = vec3(diffuse);

	vec3 ambientColor = u_AmbientColor * u_Intensity; 
	
	vec3 finalColor = (textureColor * meshColor) * (ambientColor + diffuseColor) ;

	float alpha = textureSample.w * v_VertInput.Color.w;


	if (textureSample.a == 0.0)
		discard;

	
	o_Color = vec4(finalColor,alpha);
	o_EntityID = v_EntityID;
}