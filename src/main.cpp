#include "VulkanApplication.h"


int main() {
    try {
        VulkanApplication app{};
        app.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}