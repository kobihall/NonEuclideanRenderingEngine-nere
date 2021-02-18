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

namespace nere {

    class FirstApp {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;


            void run();

        private:
            // instantialize a nereWindow
            NereWindow nereWindow{WIDTH, HEIGHT, "test"};

            NereDevice nereDevice{nereWindow};

            // read compiled shader code
            NerePipeline nerePipeline{
                nereDevice,
                "../shaders/shader.vert.spv",
                "../shaders/shader.frag.spv",
                NerePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}
