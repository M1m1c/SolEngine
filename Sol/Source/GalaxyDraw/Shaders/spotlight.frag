#version 330 core

// out in frag shaders means its the pixel color output
out vec4 FragColor;

// the "in" keyword means these values are taken from the vertex shader within this particular shader program
in vec3 FragPos;
in vec2 texCoord;
in vec3 Normal;

// uniforms represent the _same value_ for all fragments within a shader instace, 
// hard-push to use "uniform" instead of "varying" variables, as those can take up significantly more time to process
uniform sampler2D texture_diffuse1;
//uniform sampler2D tex1;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 spotDirection;
    float innerCutoff;
    float outerCutoff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float linearFalloff;
    float quadraticFalloff;

};

uniform Material material;
uniform Light spotlight;  
uniform vec3 camPos;

void main()
{
	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(spotlight.position - FragPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	vec3 viewDirection = normalize(camPos - FragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularGradient = max(dot(viewDirection, reflectionDirection), 0.0f);
	float specAmount = pow(specularGradient, material.shininess);

	float specularLight = 1.0f; // texture(tex1, texCoord).r; // Specular map // (!) Removed temporarily for flat surface debugging
	float specular = specAmount * specularLight;

	// spotlight effect
    float theta = dot(lightDirection, normalize(-spotlight.spotDirection)); 
    float epsilon = (spotlight.innerCutoff - spotlight.outerCutoff);
    float intensity = clamp((theta - spotlight.outerCutoff) / epsilon, 0.0, 1.0);
    
	diffuse  *= intensity;
    specular *= intensity;

	// attenuation - the keyword "distance" is already taken by glsl
    float dist = length(spotlight.position - FragPos);
    float attenuation = 1.0f / (1.0f + spotlight.linearFalloff * dist + spotlight.quadraticFalloff * (dist * dist));    

	vec4 texColor = vec4(1.0f,0.1f,0.8f,1.0f);// texture(texture_diffuse1, 5*texCoord); // (!) Removed temporarily for flat surface debugging

	// ambient light is assumed to be full white always
	vec3 ambientColor  = spotlight.ambient * material.ambient * texColor.xyz; 
	vec3 diffuseColor  = spotlight.diffuse * diffuse * material.diffuse * texColor.xyz;
	vec3 specularColor = spotlight.specular * specular * material.specular;

	vec3 result = (ambientColor + diffuseColor + specularColor) * attenuation;
	FragColor = vec4(result,1.0f);
}
