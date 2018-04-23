/* VERTEX ARRAY*/
 
//	A way to bind our memory layout i.e. vertex buffer layout (Vertex attrib pointer) to the actual vertex buffer.
//	The CORE OPENGL profile does not provide a default VAO object, which is what causes this code to crash on macOS as it uses the core openGL profile and not Compatibility profile
//	Changes the process from: Bind Shader -> Bind Vertex Buffer -> Setup Vertex Layout -> Bind Index Buffer -> Issue a draw call TO
//	Bind Shader -> Bind Vertex Array -> Bind Index Buffer -> Issue a draw call 
//	Sometimes it is usefull to store the vertex layout on the CPU side, for debuggind and stuff like that. 

/* Abstracting the code */

//	RENDERER - Give it a command and render the thing for me Draw(attributes) - Draws the thing for us 

/* POINTERS */

// Just an integer, that stores 1 memory address, all pointers are the same. Its just an integer that holds a memory address. 
// References are just aliases for variables. 
// Arrays - Collection of variables, grouped together. (Usually of the same type)
// Arrays in itself are just memory pointers 


/* ABSTRACTION PROCESS OF THE SHADER */

// 1. A way to compile the shader by passing in a file or a string
// 2. Bind/Unbind the shader. 
// 3. Set all the uniforms.
// Querying the shader for the type of uniforms it has should be added while creating a new game engine. 


/* WRITING YOUR OWN RENDERER */
//	Give someone some data, and it automatically draws it to the screen. 
//	OpenGL Draw call - Telling OpenGL that take whatever is bound and then draw it to the screen. 

/* TEXTURES */
//	Having a image on a 3d object in a game object
//	Having an image on any type of surface in your graphical application
//	Image that we can use, when we are rendering something. Upload the image to your gpu memory and use it in your shader.
//	GPU's can hold multiple textures and hence they have multiple texture slots, e.g. windows might have 32 texture slots,
//	whereas mobile devices might only have 8. 

/* TEXTURE CO-ORDINATES 
*	Used with a texture co-ordinate system to tell our geometry, which part of the texture should it sample from
*/

/* BLENDING 
*	A way to render something that is partially or fully transparet. 
*	How do you tell the computer the concept of transparency ? 
*	What is blending ? 
*		Combining our output color with what is already in our target buffer, where: 
*			Output color is the color that we want to display on screen (Remember the blue square glass being rendered over the red square? )
*			Target buffer would be the color we want to draw it on... the destination 
*		Controlling Blending: 
*			1. Enable Blending: glEnable(GL_BLENND) - glDisable(GL_BLEND)
			2. Define how it is going to be blended: glBlendFunc(src, dest) 
				src = how the src RGBA factor is computed (Default - GL_ONE i.e. 1) multiply the RGBA color values with one.
				dest = how the desetination RGBA factor is computed (default - GL_Zero) mult the RGBA color values with zero. 
			3. How do you combine the src and destination: (Add by default) : glBlendEquation(mode) 
*/

/* MATHEMATICS 
*	MATRICES: Array of numbers that can be manipulated in different ways.  Used for positioning objects in 3D world 
*		Transformation: Getting our vertex buffer into some sort of form on the screen. 
*	PROJECTION MATRICIES: A way for us to tell our window, the way we want to map the vertices. 
*						: Drawing 2-d stuff in a 3d world
*	VECTORS:	CAN also be just a position (co - ordinate basically) 
*		Directional: 
*
*/

/* PROJECTION MATRICES
*	Projection - how to go from having co-ordinates to map them to the window (2d object), project the 3d world on a 2d space. 
*	Maths that converts 3d points in space into a 2D window. 
*	Normalized space - Co-ordinate system between -1 to 1 in x, y, z
*	Projection Matrix - converts our co-ordinates to the normalized co-ordinate space. 
*/

/* PROJECTIONS 
*	ORTHOGRAPHIC - Usually* used for 2D rendering
*		* 
*	PERSCPECTIVE - Usually* used for 3D stuff
*		*
/

/* CAMERA
	What is a camera ? 
	It doesn't actually exist,  so you just move the geometry around, that is what happens in game engines aswell 
	It is just more intuitive this way. 
	To create a camera: 
	1. Need the location of the camera in the world space
	2. Need a position where the camera is pointing at 
	3. To get the position the camera is pointing at you just the position of the camera and the position of the object it is trying to look at, substract those two vectors
		and you get a direction vector to where the camera is pointing towards. 
*/