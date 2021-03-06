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

    // configInfo for fixed function pipeline stages
    struct PipelineConfigInfo {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class NerePipeline {
        public:
        NerePipeline(
            NereDevice& device, 
            const std::string& vertFilepath, 
            const std::string& fragFilepath,
            const PipelineConfigInfo& configInfo);
        ~NerePipeline();

        // not sure why this is needed
        NerePipeline(const NerePipeline&) = delete;
        NerePipeline& operator=(const NerePipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);

        // fills configInfo members for each pipeline stage
        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

        private:
        // reads .spv shader file and outputs the code to a buffer
        static std::vector<char> readFile(const std::string& filepath);

        // reads all different shaders in the /shaders folder
        void createGraphicsPipeline(
            const std::string& vertFilepath,
            const std::string& fragFilepath, 
            const PipelineConfigInfo& configInfo);

        // fills configInfo members for shaderModule
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        NereDevice& nereDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}