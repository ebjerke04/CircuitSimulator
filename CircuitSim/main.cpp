#include "Application.h"

int main() {
	std::unique_ptr<Application> application = std::make_unique<Application>();
	application->Begin();

	return 0;
}
