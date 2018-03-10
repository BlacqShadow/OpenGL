#pragma once

class IndexBuffer
{
private:
	// OpenGL gives an ID to every object that is created in the spec, 
	// so we can use it to draw shit
	unsigned int m_RendererID;
	// To store the amount of indicies it has
	unsigned int m_Count;
public:
	// count: How many indicies are we supplying ? 
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	// Getter
	inline unsigned int GetCount() const { return m_Count; }
};