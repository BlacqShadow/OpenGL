#shader vertex
#version 420 core

// The defined layout of the vertex buffer
layout(location = 0) in vec4 position;	//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case

out vec3 v_TexCoord;
// Send the fragment position to fragment shader

// Take in the projection matrix
// MODEL VIEW PROJECTION MATRIX
uniform mat4 u_MVP;
// Model Matrix
uniform mat4 u_Model;

void main()
{
	// Send the texture and normal co-ordinates to the fragment shader
	v_TexCoord = position.xyz;
	gl_Position = u_MVP * position;
};

#shader fragment
#version 420 core

// Open GL converts our vec2 from attrib pointer to vec 4 as we told it, that it is a vec2 
//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case
layout(location = 0) out vec4 color;

in vec3 v_TexCoord;
uniform samplerCube u_SkyBox;


void main()
{
	// Final output color
	color = texture(u_SkyBox, v_TexCoord);
}