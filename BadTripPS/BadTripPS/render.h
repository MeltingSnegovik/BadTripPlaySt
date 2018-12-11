#pragma once
#include "map.h"

//#include <SDL.h>
#include <windows.h>
#include <string.h>

/*
#include <GL\glew.h>
#include <GL\wglew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glext.h>
#include <gl\wglext.h>
#include <SDL_opengl.h>
#include <SDL_video.h>
#define GL_GLEXT_PROTOTYPES
#define GLEW_STATIC 
*/
#include "glad.h"
#include "glfw3.h"

#include <cstdio>
#include <iostream>
#include <map>


struct _position {
	GLshort p_Ox;
	GLshort p_Oy;

	_position(uint32_t val) :
		p_Ox((int16_t)val),
		p_Oy((int16_t)(val >> 16))
	{};
};

struct _color {
	GLubyte p_Red;
	GLubyte p_Blue;
	GLubyte p_Green;
	_color(uint32_t val) :
		p_Red((uint8_t)val),
		p_Blue((uint8_t)(val >> 8)),
		p_Green((uint8_t)(val >> 16))
	{};
};

template<class sometype> struct _tbuffer {
	GLuint object;
	sometype* map;
	uint32_t elsize = sizeof(sometype);
	//uint32_t size?
		
	//wtf?? what do with this shit?
	_tbuffer() :
		object(0)
	{
		glGenBuffers(1, &object);
		glBindBuffer(GL_ARRAY_BUFFER, object);


		GLsizeiptr buffersize = elsize* pscx_memory::VERTEXBUFFERLEN;
		
		GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT;

		
		glBufferStorage(
			GL_ARRAY_BUFFER
			, buffersize
			, nullptr
			, access);
		auto  memory = glMapBufferRange(
			GL_ARRAY_BUFFER
			, 0
			, buffersize
			, access);

		/*
		let s = slice::from_raw_parts_mut(memory,(uint32_t)pscx::VERTEXBUFFERLEN);
		for x in s.iter_mut(){
		*x= Default::default();
		}
		*/
	};



	void Set(uint32_t index, sometype val);
	~_tbuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, object);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glDeleteBuffers(1, &object);
	}
	//tbd
	//	auto s = slice
};

struct _render {
	
	GLFWwindow* r_window;

	GLuint r_vertexShader;
	GLuint r_fragmentShader;
	GLuint r_program;
	GLuint r_vertex_ar_obj;


	_tbuffer<_position> r_positions;
	_tbuffer<_color> r_colors;
	

	uint32_t r_nvertices;

	_render() :
		r_window(NULL)
	{		
			//surface

			std::string vs_src = "vertex.glsl";
			std::string fs_src = "fragment.glsl";
			GLchar const* f_vs_src[] = { vs_src.c_str() };
			GLchar const* f_fs_src[] = { fs_src.c_str() };
								 
			auto vertex_shader = CompileShader(f_vs_src, GL_VERTEX_SHADER);
			auto fragment_shader = CompileShader(f_vs_src, GL_FRAGMENT_SHADER);

			auto program = LinkProgram(vertex_shader, fragment_shader);
			
			glUseProgram(program);

			GLuint vao = 0;
			
			glGenVertexArrays(1,&vao);
			glBindVertexArray(vao);
			
			_tbuffer<_position> positions;
			GLchar try_attr_p[] = "vertex_position";
			GLuint index_1 = FindProgramAttrib(program,&try_attr_p[0]);
			glEnableVertexAttribArray(index_1);
			glVertexAttribLPointer(index_1, 2, GL_SHORT, 0, nullptr);

			_tbuffer<_color> colors;
			GLchar try_attr_c[] = "vertex_color";
			GLuint index_2 = FindProgramAttrib(program, &try_attr_c[0]);
			glEnableVertexAttribArray(index_2);
			glVertexAttribLPointer(index_2, 3, GL_UNSIGNED_BYTE, 0, nullptr);

			r_vertex_ar_obj = vao;

	
	};
	
	GLuint CompileShader(const GLchar** src,const GLenum shader_type);
	GLuint LinkProgram(GLuint v_shader, GLuint f_shader);
	GLuint FindProgramAttrib(GLuint program,const GLchar* attr);
	void PushTriangle(_tbuffer<_position> positions, _tbuffer<_color> colors);

	~_render()	{
		glDeleteVertexArrays(1, &r_vertex_ar_obj);
		glDeleteShader(r_vertexShader);
		glDeleteShader(r_fragmentShader);
		glDeleteProgram(r_program);
	};

};
