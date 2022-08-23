#version 330 core

// out in frag shaders means its the pixel color output
out vec4 FragColor;

// the "in" keyword means these values are taken from the vertex shader within this particular shader program
in vec3 FragPos;
in vec2 texCoord;
in vec3 Normal;

struct Material {
    sampler2D albedo;
    sampler2D specular;

    float shininess;
}; 

struct Spotlight {
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

struct PointLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float linearFalloff;
    float quadraticFalloff;
};

struct DirectionalLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// uniforms represent the _same value_ for all fragments within a shader instace, 
// hard-push to use "uniform" instead of "varying" variables, as those can take up significantly more time to process
uniform Material material;
uniform Spotlight spotlight;
uniform vec3 camPos;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diffuse = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // material Textures
    vec4 texColor = texture(material.albedo, texCoord);
    float specularMap = texture(material.specular, texCoord).r;

    // combine results
    vec3 ambientColor  = light.ambient * texColor.xyz; 
	vec3 diffuseColor  = light.diffuse * diffuse * texColor.xyz;
	vec3 specularColor = light.specular * specular * specularMap;

    return (ambientColor + diffuseColor + specularColor);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0f + light.linearFalloff * dist + light.quadraticFalloff * (dist * dist));

    // combine results
    vec3 ambientColor  = light.ambient  * texture(material.albedo, texCoord).xyz;
    vec3 diffuseColor  = light.diffuse  * diff * texture(material.albedo, texCoord).xyz;
    vec3 specularColor = light.specular * spec * texture(material.specular, texCoord).xyz;

    ambientColor  *= attenuation;
    diffuseColor  *= attenuation;
    specularColor *= attenuation;

    return (ambientColor + diffuseColor + specularColor);
}


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

	float specularLight = texture(material.specular, texCoord).r; // Specular map
	float specular = specAmount * specularLight;

	// spotlight effect
    float theta = dot(lightDirection, normalize(-spotlight.spotDirection)); 
    float epsilon = (spotlight.innerCutoff - spotlight.outerCutoff);
    float intensity = clamp((theta - spotlight.outerCutoff) / epsilon, 0.0, 1.0);
    
	diffuse  *= intensity;
    specular *= intensity;

	// attenuation - the keyword "distance" is already taken by glsl
    float dist = length(spotlight.position - FragPos);
    float attenuation = 1.0 / (1.0f + spotlight.linearFalloff * dist + spotlight.quadraticFalloff * (dist * dist));    

	vec4 texColor = texture(material.albedo, texCoord);

	// ambient light is assumed to be full white always
	vec3 ambientColor  = spotlight.ambient * texColor.xyz; 
	vec3 diffuseColor  = spotlight.diffuse * diffuse * texColor.xyz;
	vec3 specularColor = spotlight.specular * specular;

	vec3 result = (ambientColor + diffuseColor + specularColor) * attenuation;
	FragColor = vec4(result,1.0f);
}
