/* VERTEX ARRAY*/
 
//	A way to bind our memory layout i.e. vertex buffer layout (Vertex attrib pointer) to the actual vertex buffer.
//	The CORE OPENGL profile does not provide a default VAO object, which is what causes this code to crash on macOS as it uses the core openGL profile and not Compatibility profile
//	Changes the process from: Bind Shader -> Bind Vertex Buffer -> Setup Vertex Layout -> Bind Index Buffer -> Issue a draw call TO
//	Bind Shader -> Bind Vertex Array -> Bind Index Buffer -> Issue a draw call 

/* Abstracting the code */

//	RENDERER - Give it a command and render the thing for me Draw(attributes) - Draws the thing for us 

/* POINTERS */

// Just an integer, that stores 1 memory address, all pointers are the same. Its just an integer that holds a memory address. 
// References are just aliases for variables. 
// Arrays - Collection of variables, grouped together. (Usually of the same type)
// Arrays in itself are just memory pointers 

