//
//  first_app.cpp
//  nere
//
//  Created by Kobi Hall on 2/17/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#include "first_app.hpp"

namespace nere {

void FirstApp::run() {
    
    while(!nereWindow.shouldClose()){
        // process all pending glfw events
        glfwPollEvents();
    }
}
}
