#pragma once
#include "map.h"

#include <gl\glew.h>
#include <GL\wglew.h>
#include <gl\GLU.h>

#include <string.h>
//#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_video.h>
#include <cstdio>
#include <iostream>

struct _render {
	
	
	SDL_Window* r_window;
	SDL_Surface* r_screenSurface;
	SDL_GLContext* r_context;

	GLuint r_vertexShader;
	GLuint r_fragmentShader;
	GLuint r_program;
	GLuint r_vertex_ar_obj;
	_buffer r_position;
	_buffer r_colors;
	uint32_t nvertices;


	_render() :
		r_window(NULL),
		r_screenSurface(NULL)
	{
		
		std::string vs_src = "vertex.glsl";
		std::string fs_src = "fragment.glsl";

		GLuint CompileShader(GLenum shader_type);


		r_window = SDL_CreateWindow(
			"BadTripPS", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			640,
			480,
			SDL_WINDOW_SHOWN);
		if (r_window == NULL)
		{
			printf("Windows don't exist SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//surface
			r_screenSurface = SDL_GetWindowSurface(r_window);

			//white surface
			SDL_FillRect(r_screenSurface, NULL, SDL_MapRGB(r_screenSurface->format, 0xFF, 0xFF, 0xFF));

			//follow white surface
			SDL_UpdateWindowSurface(r_window);

		};
	};
};

struct _buffer {
	GLuint object;
	//pscx_memory::_map t;
	//uint32_t size?

	//wtf?? what do with this shit?
	_buffer() :
		object(0)
	{
		glGenBuffers(1, &object);
		glBindBuffer(GL_ARRAY_BUFFER, object);
		
	//	GLsizeiptr elsize = sizeof<T>();
		GLsizeiptr buffersize = 32* pscx_memory::VERTEXBUFFERLEN;
		
		auto access = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT;
		
		glBufferStorage(
			GL_ARRAY_BUFFER
			, buffersize
			, nullptr
			, access);
		auto  memory =glMapBufferRange(
			GL_ARRAY_BUFFER
			, 0
			, buffersize
			, access);
		};
	//tbd
	//	auto s = slice
};