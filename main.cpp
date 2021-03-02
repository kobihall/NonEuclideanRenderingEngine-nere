//
//  main.cpp
//  NonEuclideanRenderingEngine-nere main file
//
//  Created by Kobi Hall on 2/17/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#include "first_app.hpp"


//std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    UserSettings userSettings;
    userSettings.chosenPipeline = NERE_COMPUTE_PIPELINE;
    userSettings.width = 800;
    userSettings.height = 600;
    nere::FirstApp app{userSettings};
    
    // try&catch run from application
    try {
        app.run();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
