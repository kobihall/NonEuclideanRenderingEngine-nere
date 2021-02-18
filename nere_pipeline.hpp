//
//  nere_pipeline.hpp
//  NonEuclideanRenderingEngine-nere main file
//
//  Created by Kobi Hall on 2/18/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//
#pragma once

#include "nere_device.hpp"

// std
#include <string>
#include <vector>

namespace nere {

    struct PipelineConfigInfo {};

    class NerePipeline {
        public:
        NerePipeline(
            NereDevice& device, 
            const std::string& vertFilepath, 
            const std::string& fragFilepath,
            const PipelineConfigInfo& configInfo);
        ~NerePipeline() {}

        NerePipeline(const NerePipeline&) = delete;
        void operator=(const NerePipeline&) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

        private:
        // reads .spv shader file and outputs the code to a buffer
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicsPipeline(
            const std::string& vertFilepath,
            const std::string& fragFilepath, 
            const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        NereDevice& nereDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}