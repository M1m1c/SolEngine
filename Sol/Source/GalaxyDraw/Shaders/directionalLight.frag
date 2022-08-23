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
    // vec3 position;
	vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;  
uniform vec3 camPos;

void main()
{
	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(-light.direction);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	vec3 viewDirection = normalize(camPos - FragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularGradient = max(dot(viewDirection, reflectionDirection), 0.0f);
	float specAmount = pow(specularGradient, material.shininess);

	float specularLight = texture(tex1, texCoord).r; // Specular map
	float specular = specAmount * specularLight;

	vec4 texColor = texture(tex0, texCoord); // Albedo map

	vec3 ambientColor =  light.ambient * material.ambient * texColor.xyz; 
	vec3 diffuseColor =  light.diffuse * diffuse * material.diffuse * texColor.xyz;
	vec3 specularColor = light.specular * specular * material.specular;

	vec3 result = (ambientColor + diffuseColor + specularColor); 
	FragColor = vec4(result,1.0f);
}
