//
//  first_app.cpp
//  nere
//
//  Created by Kobi Hall on 2/17/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#include "first_app.hpp"

// std
#include <stdexcept>

namespace nere {

    FirstApp::FirstApp() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() {
        vkDestroyPipelineLayout(nereDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {

        while(!nereWindow.shouldClose()){
            // process all pending glfw events
            glfwPollEvents();
        }
    }

    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if(vkCreatePipelineLayout(nereDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void FirstApp::createPipeline() {
        auto pipelineConfig = NerePipeline::defaultPipelineConfigInfo(nereSwapChain.width(), nereSwapChain.height());
        pipelineConfig.renderPass = nereSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        nerePipeline = std::make_unique<NerePipeline>(
            nereDevice,
            "../shaders/shader.vert.spv",
            "../shaders/shader.frag.spv",
            pipelineConfig
        );
    }

    void FirstApp::createCommandBuffers(){}
    void FirstApp::drawFrame(){}
}
