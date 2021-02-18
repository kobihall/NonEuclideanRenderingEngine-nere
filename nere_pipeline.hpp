//
//  nere_pipeline.hpp
//  NonEuclideanRenderingEngine-nere main file
//
//  Created by Kobi Hall on 2/18/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//
#pragma once

#include <string>
#include <vector>

namespace nere {

    class NerePipeline {
        public:
        NerePipeline(const std::string& vertFilepath);

        private:
        // reads .spv shader file and outputs the code to a buffer
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicsPipeline(const std::string& vertFilepath);
    };
}