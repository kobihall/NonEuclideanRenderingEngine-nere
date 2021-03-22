//
//  user_interface.cpp
//  NonEuclideanRenderingEngine
//
//  Created by Kobi Hall on 3/21/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#include "user_interface.hpp"

namespace nere {
    imGUI::imGUI(){
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
    }
}