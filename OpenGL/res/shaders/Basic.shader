#shader vertex
#version 330 core

// Open GL converts our vec2 from attrib pointer to vec 4 as we told it, that it is a vec2 
layout(location = 0) in vec4 position;	//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case
layout(location = 1) in vec2 texCoord;  // Texture co-ordinates of what sample of the texture to use

out vec2 v_TexCoord;

// Take in the projection matrix
// MODEL VIEW PROJECTION MATRIX
uniform mat4 u_MVP; 

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

// Open GL converts our vec2 from attrib pointer to vec 4 as we told it, that it is a vec2 
//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case
layout(location = 0) out vec4 color;	

in vec2 v_TexCoord; 

/* Define a uniform to recieve data from the c++ side (CPU) */
/* Convention: anything beginning with u_ is a uniform variable */
uniform vec4 u_Color;
uniform sampler2D u_Texture; // The actual color of the texture 
void main()
{
	/* Specify the output color of each pixel */
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};