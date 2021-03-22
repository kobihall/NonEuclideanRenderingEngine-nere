//
//  user_interface.hpp
//  NonEuclideanRenderingEngine
//
//  Created by Kobi Hall on 3/21/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//
#pragma once

#include "nere_device.hpp"
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace nere {
class imGUI {
    private:
    VkSampler sampler;
    public:
    imGUI();
    ~imGUI();
};
}