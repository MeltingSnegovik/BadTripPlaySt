#include "wind.h"


	void pscx_wind::_wind::InitW()
	{

		if (!glfwInit()) {
			std::cout << "failed glfw" << std::endl;
		};

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		window = glfwCreateWindow(800, 600, "pscxbt", NULL, NULL);
		if (window == NULL) {
			std::cout << "windowisnotcreate" << std::endl;
			glfwTerminate();
		};
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "glad is not init" << std::endl;
		}
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glfwSwapBuffers(window);
		glfwPollEvents();

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};

		glViewport(0, 0, 800, 600);
		GLuint vboID;
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		GLuint vaoID;
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(vaoID);
		glDrawArrays(GL_TRIANGLES, 0, (sizeof(vertices) / sizeof(*vertices) / 3));
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	};
