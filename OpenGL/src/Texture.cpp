#include "stb_image\stb_image.h"
#include "Texture.h"
#include <string>

Texture::Texture(const std::string& path, const std::string& type)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), 
	m_Width(0), m_Height(0), m_BPP(0), m_Type(type)
{

	if (m_Type == "cubemap")
	{
		// Generate One Texture and assign it to renderer ID
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

		for (unsigned int i = 0; i < sizeof(m_Sides) / sizeof(m_Sides[0]); i++)
		{
			m_LocalBuffer = stbi_load((path + m_Sides[i]).c_str(), &m_Width, &m_Height, &m_BPP, 4);
			if (!m_LocalBuffer)
			{
				std::cout << stbi_failure_reason() << std::endl;
				ASSERT(m_LocalBuffer);
			}
			// Send the data in m_LocalBuffer to Open GL
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
			if (m_LocalBuffer)
				stbi_image_free(m_LocalBuffer);
		}

		// Set texture parameters
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); // If the canvas is larger than the texture size
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // Wrap mode horizontal (Don't extend the area) 
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		// Unbind after doing stuff
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
	else
	{
		// Flips the texture image as a lot of png's start at top left
		// Unlike OpenGL which likes to do things from the bottom left 

		stbi_set_flip_vertically_on_load(0);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
		if (!m_LocalBuffer)
		{
		std::cout << stbi_failure_reason() << std::endl;
		ASSERT(m_LocalBuffer);
		}
		// Generate One Texture and assign it to renderer ID
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		/* BLENDING */
		// Enable blending
		//GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Set texture parameters
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); // If the canvas is larger than the texture size
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)); // Wrap mode horizontal (Don't extend the area) 
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		// Send the data in m_LocalBuffer to Open GL
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		// Unbind the buffer
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		// If the buffer contains data, free the local buffer 
		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}
}


Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
	std::cout << "Texture Deleted" << std::endl;
}

void Texture::Bind(unsigned int slot) const
{
	if (m_Type == "cubemap")
	{
		GLCall((GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
	}
	else
	{
		// Assign a texture slot to the Texture
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
