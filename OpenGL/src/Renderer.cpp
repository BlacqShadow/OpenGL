#include "Renderer.h"
#include "Model.h"
/* Clear all the errors */
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

/* Print the errors */
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]: " << std::hex << error << std::dec << ": " << function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	//GLCall(glClearColor(0.7f, 0.7f, 0.7f, 1.0f));
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
	/* Bind all the buffers again before issuing a draw call */
	shader.Bind();
	va.Bind();
	ib.Bind();
	/* glDrawElements is used to draw from the index buffers */
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // Nullpointer because the buffer has already been bound
}

void Renderer::Draw(const Model& model, Shader& shader) const
{
	const std::vector<Mesh*> meshes = model.GetMeshes();
	/* Bind all the buffers again before issuing a draw call */
	shader.Bind();
	// Draw all the meshes in the model
	// Draw the models, I should probably have the Models have their own draw function and call render on them
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		// Get all the textures stored in this mesh
		const std::vector<Texture *> textures = meshes[i]->Textures();
		// Set the appropriate textures in the shader
		unsigned int diffuseCount = 0;
		unsigned int specularCount = 0;

		// Setup all the appropriate textures in the shader for this mesh
		for (unsigned int j = 0; j < textures.size(); j++)
		{
			// Bind the texture to the right slot
			textures[j]->Bind(j);
			// Used to determine which texture will be bound in the shader
			std::string number;
			std::string name = textures[j]->m_Type;
			if (name == "Texture_Diffuse")
				number = std::to_string(diffuseCount++);
			else if (name == "Texture_Specular")
				number = std::to_string(specularCount++);
			else
				std::cout << "Incorrect Texture Found in the model";
			shader.SetUniform1i(("u_Material." + name + number), j);
		}
		// Get the Index buffer and vao of the meshbeing rendered
		meshes[i]->VA().Bind();
		meshes[i]->IB().Bind();
		/* glDrawElements is used to draw from the index buffers */
		GLCall(glDrawElements(GL_TRIANGLES, model.GetMeshes()[i]->IB().GetCount(), GL_UNSIGNED_INT, nullptr)); // Nullpointer because the buffer has already been bound
	}
}
