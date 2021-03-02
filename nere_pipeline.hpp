//
//  nere_pipeline.hpp
//  NonEuclideanRenderingEngine
//
//  Created by Kobi Hall on 2/18/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//
#pragma once

#include "nere_device.hpp"
#include "user_settings.hpp"

// std
#include <string>
#include <vector>

namespace nere {

    // configInfo for fixed function pipeline stages
    struct PipelineConfigInfo {
        UserSettings userSettings;
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class NerePipeline {
        public:
        NerePipeline(
            NereDevice& device, 
            const std::string& shaderFilepath,
            const PipelineConfigInfo& configInfo);
        ~NerePipeline();

        // not sure why this is needed
        NerePipeline(const NerePipeline&) = delete;
        void operator=(const NerePipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);

        // fills configInfo members for each pipeline stage
        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

        private:
        // reads .spv shader file and outputs the code to a buffer
        static std::vector<char> readFile(const std::string& filepath);

        // reads all different shaders in the /shaders folder
        void createGraphicsPipeline(
            const std::string& vertFilepath,
            const std::string& fragFilepath, 
            const PipelineConfigInfo& configInfo);

        void createComputePipeline(
            const std::string& compFilepath, 
            const PipelineConfigInfo& configInfo);

        // fills configInfo members for shaderModule
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        UserSettings userSettings_{};

        NereDevice& nereDevice;
        VkPipeline graphicsPipeline;
        VkPipeline computePipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
        VkShaderModule compShaderModule;
    };
}