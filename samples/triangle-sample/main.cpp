#include "triangle_application.hpp"

#include <memory>

int main() {
	const auto app = std::make_shared<wrapper::samples::triangle_application>(
		"triangle-sample", 800, 600);

	app->run_loop();
}