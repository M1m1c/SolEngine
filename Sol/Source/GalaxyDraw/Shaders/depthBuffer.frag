#version 330 core
out vec4 FragColor;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC (Normalized Device Coordinates)
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    // gl_FragCoord apparently fetches the current fragment's depth value, I mean wtf???
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for visually scaling it to 0~1 range
    FragColor = vec4(vec3(depth), 1.0);
}
