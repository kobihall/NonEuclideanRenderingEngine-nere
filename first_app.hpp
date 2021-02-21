//
//  first_app.hpp
//  nere
//
//  Created by Kobi Hall on 2/17/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#pragma once

#include "nere_window.hpp"
#include "nere_pipeline.hpp"
#include "nere_device.hpp"
#include "nere_swap_chain.hpp"

// std
#include <memory>
#include <vector>

namespace nere {

    class FirstApp {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

            FirstApp();
            ~FirstApp();

            FirstApp(const NereWindow &) = delete;
            FirstApp &operator=(const NereWindow &) = delete;

            void run();

        private:
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void drawFrame();

            // instantialize a nereWindow
            NereWindow nereWindow{WIDTH, HEIGHT, "test"};

            NereDevice nereDevice{nereWindow};

            NereSwapChain nereSwapChain{nereDevice, nereWindow.getExtent()};

            // read compiled shader code
            std::unique_ptr<NerePipeline> nerePipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
    };
}
