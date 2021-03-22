//
//  user_settings.hpp
//  user settings
//
//  Created by Kobi Hall on 2/21/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//
#pragma once

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

//enum availablePipelines{NERE_GRAPHICS_PIPELINE, NERE_COMPUTE_PIPELINE};

struct UserSettings final {
    //availablePipelines chosenPipeline;
    ImGui_ImplVulkan_InitInfo imguiInitInfo = {};
    int width;
    int height;
};