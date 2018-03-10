#pragma once

class VertexBuffer
{
private:
	// OpenGL gives an ID to every object that is created in the spec, 
	// so we can use it to draw shit
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};