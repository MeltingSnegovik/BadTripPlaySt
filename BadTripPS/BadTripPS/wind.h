#pragma once
#include "glad.h"
#include "glfw3.h"
#include <iostream>


namespace pscx_wind
{

	struct _wind {
		GLFWwindow* window;
		GLuint a;

		void InitW();
	};
};