#version 330 core

// out in frag shaders means its the pixel color output
out vec4 FragColor;

// the "in" keyword means these values are taken from the vertex shader within this particular shader program
in vec3 FragPos;
in vec2 texCoord;
in vec3 Normal;

// uniforms represent the _same value_ for all fragments within a shader instace, 
// hard-push to use "uniform" instead of "varying" variables, as those can take up significantly more time to process
uniform sampler2D tex0;
uniform sampler2D tex1;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
}; 

struct Light {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	// Point Lights require linear falloff and quadratic falloff values 
    float linearFalloff;
    float quadraticFalloff;
}; 

uniform Material material;
uniform Light light;  
uniform vec3 camPos;

void main()
{
	vec3 ambient = material.ambient;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(light.position - FragPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	vec3 viewDirection = normalize(camPos - FragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularGradient = max(dot(viewDirection, reflectionDirection), 0.0f);
	float specAmount = pow(specularGradient, material.shininess);

	float specularMap = texture(tex1, texCoord).r;
	float specular = specAmount * specularMap;

	// outputs final color
	vec4 texColor = texture(tex0, texCoord);

	// attenuation - the keyword "distance" is already taken by glsl
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (1.0f + light.linearFalloff * dist + light.quadraticFalloff * (dist * dist));    

	// ambient light is assumed to be full white always
	vec3 ambientColor =  light.ambient * material.ambient * texColor.xyz; 
	vec3 diffuseColor =  light.diffuse * diffuse * material.diffuse * texColor.xyz;
	vec3 specularColor = light.specular * specular * material.specular;

	vec3 result=(ambientColor + diffuseColor + specularColor) * attenuation;
	FragColor = vec4(result,1.0f);
}
