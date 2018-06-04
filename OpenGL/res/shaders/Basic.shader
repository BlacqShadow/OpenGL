#shader vertex
#version 420 core

// The defined layout of the vertex buffer
layout(location = 0) in vec4 position;	//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case
layout(location = 1) in vec3 normal;  // Texture co-ordinates of what sample of the texture to use
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 v_Normal;
// Send the fragment position to fragment shader
out vec4 v_FragPos;

// Take in the projection matrix
// MODEL VIEW PROJECTION MATRIX
uniform mat4 u_MVP; 
// Model Matrix
uniform mat4 u_Model;
// Normal Matrix - to perform the normal transformaitons in the world space 
uniform mat4 u_Normal;


void main()
{

	
	// Send the texture and normal co-ordinates to the fragment shader
	v_TexCoord = texCoord;

	v_Normal = u_Normal * vec4(normal,0.0f);
	// To get the fragment position of the model multiply it with the model matrix
	v_FragPos = u_Model * position;
	gl_Position = u_MVP * position;
};

#shader fragment
#version 420 core

// Open GL converts our vec2 from attrib pointer to vec 4 as we told it, that it is a vec2 
//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case
layout(location = 0) out vec4 color;	

in vec2 v_TexCoord; 
in vec4 v_Normal;
in vec4 v_FragPos;

/* Define a uniform to recieve data from the c++ side (CPU) */
/* Convention: anything beginning with u_ is a uniform variable */
uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;
uniform vec4 u_LightPosition;
// Get the position where the user is looking from for specular lighting
uniform vec3 u_CameraPos;

uniform sampler2D u_Texture; // The actual color of the texture 

							 
// Allow the shader to take in multiple textures. 
uniform sampler2D u_Texture_Diffuse1;
uniform sampler2D u_Texture_Diffuse2;
uniform sampler2D u_Texture_Diffuse3;
uniform sampler2D u_Texture_Specular1;
uniform sampler2D u_Texture_Specular2;

void main()
{
	/* Specify the output color of each pixel */
	//vec4 texColor = texture(u_Texture, v_TexCoord);
	// Lighting
	// AMBIENT 
	float ambientStrength = 0.1f;
	vec4 ambient = u_LightColor * ambientStrength;
	

	// DIFFUSE - Get the dot product of the normal vector and the vector from 
	// Normalize the incoming normal vector
	vec4 norm = normalize(v_Normal);
	// frag to light source 
	// Vector from fragment to light source
	vec4 vectorToLight = normalize(u_LightPosition - v_FragPos);
	// The diffuse intensity will depend upon the dot product of these two vectors
	float diffStrength = max(dot(norm, vectorToLight), 0.0f);
	vec4 diffuse = u_LightColor * diffStrength;

	// SPECULAR
	float specularIntensity = 0.5f;
	// Required: reflected vector, direction vector the user is looking from, angle between the fragment and view position
	//vec4 normCamera = normalize(vec4(u_CameraPos, 1.0f));
	vec4 viewDir = normalize(vec4(u_CameraPos, 1.0f) - v_FragPos);
	vec4 reflectedVec = reflect(vectorToLight, norm);
	/*float spec = 1.0f;
	for (int i = 0; i < 32; i++)
	{
		spec = spec * max(dot(viewDir, reflectedVec), 0.0f);
	}*/
	float spec = pow(max(dot(viewDir, reflectedVec), 0.0f), 64.0f);
	vec4 specular = spec * specularIntensity * u_LightColor;


	// Color of this pixel will be the sum of ambient and diffuse times the color of the object
	vec4 final = (ambient + diffuse + specular) * u_ObjectColor;

	//color = vec4(spec, 0.0f, 0.0f, 1.0f);
	color = final;
}