#include "Renderer.h"
#include <iostream>


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

Renderer::Renderer()
{
	bool demo_window = true;
	bool another_window = false; 
	ImVec4 color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	m_show_demo_window = &demo_window;
	m_show_another_window = &another_window;
	m_clear_color = &color;
}

Renderer::~Renderer()
{
}

void Renderer::Clear() const
{

	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
	/* Bind all the buffers again before issuing a draw call */
	shader.Bind();
	va.Bind();
	ib.Bind();
	/**********************************************IMGUI CODE*********************************************************************************/
	ImGui_ImplGlfwGL3_NewFrame();

	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)m_clear_color); // Edit 3 floats representing a color

		ImGui::Checkbox("Demo Window", m_show_demo_window);      // Edit bools storing our windows open/close state
		ImGui::Checkbox("Another Window", m_show_another_window);

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if(*m_show_another_window)
	{
		ImGui::Begin("Another Window", m_show_another_window);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			*m_show_another_window = false;
		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if (*m_show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(m_show_demo_window);
	}

	/****************************************************************************************************************************************/
	/* glDrawElements is used to draw from the index buffers */
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // Nullpointer because the buffer has already been bound
}
