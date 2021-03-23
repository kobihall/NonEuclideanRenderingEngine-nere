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
            bool wasWindowResized() { return framebufferResized;}
            void resetWindowResizedFlag() { framebufferResized = false;}

            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
            
        private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
            void initWindow();
            
            int width;
            int height;
            bool framebufferResized = false;
            
            std::string windowName;
            GLFWwindow *window;
    };
}
