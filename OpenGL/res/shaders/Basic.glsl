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

	// Make sure normals are not translatable
	v_Normal = u_Normal * vec4(normal, 0.0f);
	// To get the fragment position of the model multiply it with the model matrix
	v_FragPos = u_Model * position;
	gl_Position = u_MVP * position;
};

#shader fragment
#version 420 core

// Open GL converts our vec2 from attrib pointer to vec 4 as we told it, that it is a vec2 
//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case
layout(location = 0) out vec4 color;	


// Store the light properties affecting the object
struct Light {
	vec3 lightPos;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
uniform Light u_Light;

// Store the material properties of the object 
// Color the object will reflect under light
struct Material {
	// Allow the shader to take in multiple textures. 
	sampler2D Texture_Diffuse0;
	sampler2D Texture_Diffuse1;
	sampler2D Texture_Diffuse2;
	sampler2D Texture_Specular0;
	sampler2D Texture_Specular1;
	vec4 diffuse;
	vec4 specular;
	float shine;
};

// Get material from CPU side
uniform Material u_Material;


in vec2 v_TexCoord; 
in vec4 v_Normal;
in vec4 v_FragPos;

/* Define a uniform to recieve data from the c++ side (CPU) */
/* Convention: anything beginning with u_ is a uniform variable */
uniform vec4 u_ObjectColor;
uniform vec4 u_LightPosition;
// Get the position where the user is looking from for specular lighting
uniform vec3 u_CameraPos;

uniform sampler2D u_Texture; // The actual color of the texture 

							 


void main()
{
	/* Specify the output color of each pixel */
	//vec4 texColor = texture(u_Texture, v_TexCoord);
	
	// Lighting
	// AMBIENT 
	//float ambientStrength = 0.1f;
	vec4 ambient = u_Light.ambient * u_Material.diffuse;
	

	// DIFFUSE - Get the dot product of the normal vector and the vector from 
	// Normalize the incoming normal vector
	vec4 norm = normalize(v_Normal);
	// frag to light source 
	// Vector from fragment to light source
	vec4 vectorToLight = normalize(vec4(u_Light.lightPos,1.0f) - v_FragPos);
	// The diffuse intensity will depend upon the dot product of these two vectors
	float diffStrength = max(dot(norm, vectorToLight), 0.0f);
	vec4 diffuse = u_Light.diffuse * (diffStrength * u_Material.diffuse);
	
	// SPECULAR
	float specularIntensity = 0.5f;
	float shine = 64.0f;
	// Required: reflected vector, direction vector the user is looking from, angle between the fragment and view position
	//vec4 normCamera = normalize(vec4(u_CameraPos, 1.0f));
	vec4 viewDir = normalize(vec4(u_CameraPos, 1.0f) - v_FragPos);
	vec4 reflectedVec = reflect(-vectorToLight, norm);
	float spec = pow(max(dot(viewDir, reflectedVec), 0.0f), u_Material.shine);
	vec4 specular = u_Light.specular * (spec * u_Material.specular);


	// Color of this pixel will be the sum of ambient and diffuse times the color of the object
	vec4 final = ambient + diffuse + specular;

	color = final;
	//color = u_Material.diffuse;
}