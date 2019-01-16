#include "Window.h"
#include "Shader.h"

int main(void)
{
	Window window(1366, 768, "Project");
	window.run();

	glfwTerminate();
	return 0;
}