//
//  main.cpp
//  NonEuclideanRenderingEngine-nere main file
//
//  Created by Kobi Hall on 2/17/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#include "first_app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

//std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    nere::FirstApp app{};
    
    // try&catch run from application
    try {
        app.run();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
