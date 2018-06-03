#shader vertex
#version 330 core

// The defined layout of the vertex buffer
layout(location = 0) in vec4 position;	//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case
layout(location = 1) in vec4 normal;  // Texture co-ordinates of what sample of the texture to use
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

void main()
{
	gl_Position = u_MVP * position;
	
	// Send the texture and normal co-ordinates to the fragment shader
	v_TexCoord = texCoord;
	v_Normal = normal;
	// To get the fragment position of the model multiply it with the model matrix
	v_FragPos = u_Model * position;
};

#shader fragment
#version 330 core

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
	// ambient 
	float ambientStrength = 0.1f;
	vec4 ambient = u_LightColor * ambientStrength;
	
	// Diffuse - Get the dot product of the normal vector and the vector from 
	// frag to light source 
	// Vector from fragment to light source
	vec4 vectorToLight = normalize(u_LightPosition - v_FragPos);
	// The diffuse intensity will depend upon the dot product of these two vectors
	float diffStrength = max(normalize(dot(vectorToLight, v_Normal)), 0);
	vec4 diffuse = u_LightColor * diffStrength;


	// Color of this pixel will be the sum of ambient and diffuse times the color of the object
	vec4 final = (ambient + diffuse) * u_ObjectColor;
	color = final;
};