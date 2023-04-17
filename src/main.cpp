#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "App.h"

int main()
{
    FVulkanEngine::App engine;
    try
    {
        engine.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}