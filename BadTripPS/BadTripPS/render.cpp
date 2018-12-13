#include "render.h"

GLuint _render::CompileShader(const GLchar** src,const GLenum shader_type) {
	GLuint shader = glCreateShader(shader_type);
	//передача указателя на строку?
	glShaderSource(shader, 1,src,nullptr);
	glCompileShader(shader);
	auto status = (GLint)GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != (GLint)GL_TRUE)
		std::cout << "Shader Compilation failed" << std::endl;

	return shader;
};

//перебор по элементам в shader
GLuint _render::LinkProgram(GLuint v_shader, GLuint f_shader) {
	auto program = glCreateProgram();
	glAttachShader(program, v_shader);
	glAttachShader(program, f_shader);
	auto status = (GLint)GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status!=(GLint)GL_TRUE)
		std::cout << "OpenGL program linking failed" << std::endl;
	return program;
};

GLuint _render::FindProgramAttrib(GLuint program,const GLchar* attr) {
	
	auto index = glGetAttribLocation(program, attr);
	if (index <0)
		std::cout << "Attribute not found in program" << attr<< std::endl;
	return (GLuint)index;
};

template<class sometype> void _tbuffer<sometype>::Set(uint32_t index, sometype val) {
	if (index >= pscx_memory::VERTEXBUFFERLEN) {
		std::cout << "Buffer overflow"  << std::endl;
	};
	//mb
	*(map + sizeof(long)) = val;
};

void _render::PushTriangle(_position positions[], _color colors[]) {
	if (r_nvertices + 3> pscx_memory::VERTEXBUFFERLEN) {
		std::cout << "Vertex attrib buffer full forcing draw" << std::endl;
		RDraw();
	}
	for (int i = 0;i < 3;i++) {
	r_positions.Set(r_nvertices, positions[i]);
	r_colors.Set(r_nvertices, colors[i]);
	r_nvertices++;
	};
};
void _render::PushQuad(_position positions[], _color colors[]) {
	if (r_nvertices + 6 > pscx_memory::VERTEXBUFFERLEN) {
		RDraw();
	}
	for (int i = 0;i < 3;i++) {
		r_positions.Set(r_nvertices, positions[i]);
		r_colors.Set(r_nvertices, colors[i]);
		r_nvertices++;
	};
	for (int i = 1;i < 4;i++) {
		r_positions.Set(r_nvertices, positions[i]);
		r_colors.Set(r_nvertices, colors[i]);
		r_nvertices++;
	};
};


void _render::RDraw() {
	glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
	glDrawArrays(GL_TRIANGLES, 0,(GLsizei)r_nvertices);
	auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	while (true) {
		auto r = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 10000000);
		if (r == GL_ALREADY_SIGNALED || GL_CONDITION_SATISFIED){
			break;
		};
	};
	r_nvertices = 0;
};

void _render::Display() {
	_render::RDraw();
	glfwSwapBuffers(wind.window);
};

void _render::Gp0DrawingOffset() {

	_render::Display();
};


void _render::CheckForErrors() {
	bool fatal = false;
	while (true) {
		std::vector<GLchar> qbuffer(4096,0);
		GLenum severety = 0;
		GLenum source = 0;
		GLsizei message_size = 0;
		GLenum mtype = 0;
		GLuint id = 0;
//		auto count = glGetDebugMessageLog(1, qbuffer.size, &source, &mtype, &id, &severety, &message_size, &qbuffer[0]); //tbd
//		if (count == 0)
//			break;
		std::cout << severety << " " << source << " " << message_size << " " << mtype << " " << id << std::endl;
	};
};

