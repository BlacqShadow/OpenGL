#shader vertex
#version 330 core

// Open GL converts our vec2 from attrib pointer to vec 4 as we told it, that it is a vec2 
layout(location = 0) in vec4 position;	//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case

void main()
{
	gl_Position = position;
};

#shader fragment
#version 330 core

// Open GL converts our vec2 from attrib pointer to vec 4 as we told it, that it is a vec2 
//create position co-ordinate and tell open gl at what index is it located i.e. 0 in our case
layout(location = 0) out vec4 color;	

/* Define a uniform to recieve data from the c++ side (CPU) */
/* Convention: anything beginning with u_ is a uniform variable */
uniform vec4 u_Color;

void main()
{
	/* Specify the output color of each pixel */
	color = u_Color;
};