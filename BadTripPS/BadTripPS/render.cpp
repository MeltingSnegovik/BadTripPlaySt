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

void _render::PushTriangle(_tbuffer<_position> positions, _tbuffer<_color> colors) {
	if (r_nvertices + 3> pscx_memory::VERTEXBUFFERLEN) {
		std::cout << "Vertex attrib buffer full forcing draw" << std::endl;
	//	RDraw();
	}
	for (int i = 0;i <= 3;i++) {
//		r_positions.Set(r_nvertices, positions[i]);
//		r_colors.Set(r_nvertices, colors[i]);
	}
};

