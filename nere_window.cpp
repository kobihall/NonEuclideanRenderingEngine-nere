//
//  nere_window.cpp
//  nere
//
//  Created by Kobi Hall on 2/17/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#include "nere_window.hpp"

//std
#include <stdexcept>

namespace nere {

NereWindow::NereWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
    // initialize glfw window
    initWindow();
}

NereWindow::~NereWindow() {
    // terminate glfw window
    glfwDestroyWindow(window);
    glfwTerminate();
}

// initializes glfw and then creates a window
void NereWindow::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void NereWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
    if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
        throw std::runtime_error("failed to create window surface");
    }
}

void NereWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto nereWindow = reinterpret_cast<NereWindow *>(glfwGetWindowUserPointer(window));
    nereWindow->framebufferResized = true;
    nereWindow->width = width;
    nereWindow->height = height;
}
}
