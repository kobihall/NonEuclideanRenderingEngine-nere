//
//  nere_Window.hpp
//  nere
//
//  Created by Kobi Hall on 2/17/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace nere {
        
    class NereWindow {
        public:
            NereWindow(int w, int h, std::string name);
            ~NereWindow();
            
            NereWindow(const NereWindow &) = delete;
            NereWindow &operator=(const NereWindow &) = delete;
            
            bool shouldClose() { return glfwWindowShouldClose(window); }
            VkExtent2D getExtent() { return {static_cast<uint32_t>(width),static_cast<uint32_t>(height)};}

            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
            
        private:
            void initWindow();
            
            const int width;
            const int height;
            
            std::string windowName;
            GLFWwindow *window;
    };
}
