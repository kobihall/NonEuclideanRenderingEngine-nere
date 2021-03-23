//
//  first_app.cpp
//  nere
//
//  Created by Kobi Hall on 2/17/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#include "first_app.hpp"

#include "vulkan_initializers.hpp"

// std
#include <cassert>
#include <stdexcept>
#include <array>

namespace nere {

    FirstApp::FirstApp() {
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() {
        vkDestroyPipelineLayout(nereDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {

        while(!nereWindow.shouldClose()){
            // process all pending glfw events
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(nereDevice.device());
    }

    void FirstApp::loadModels() {
        std::vector<NereModel::Vertex> vertices {
            {{0.0f, -0.5f},{1,0,0}},
            {{0.5f, 0.5f},{0,1,0}},
            {{-0.5f, 0.5f},{0,0,1}}
        };

        nereModel = std::make_unique<NereModel>(nereDevice, vertices);
    }

    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        
        auto result = vkCreatePipelineLayout(nereDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout);
        initializers::check_vk_result(result);
    }

    void FirstApp::recreateSwapChain() {
        auto extent = nereWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = nereWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(nereDevice.device());
        
        if(nereSwapChain == nullptr){
            nereSwapChain = std::make_unique<NereSwapChain>(nereDevice, extent);
        } else {
            nereSwapChain = std::make_unique<NereSwapChain>(nereDevice, extent, std::move(nereSwapChain));
            if(nereSwapChain->imageCount() != commandBuffers.size()){
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        
        createPipeline();
    }

    void FirstApp::createPipeline() {
        assert(nereSwapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        NerePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = nereSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        nerePipeline = std::make_unique<NerePipeline>(
            nereDevice,
            "../shaders/shader.vert.spv",
            "../shaders/shader.frag.spv",
            pipelineConfig
        );
    }

    void FirstApp::createCommandBuffers(){
        commandBuffers.resize(nereSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = nereDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(nereDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void FirstApp::freeCommandBuffers(){
        vkFreeCommandBuffers(nereDevice.device(), nereDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    void FirstApp::recordCommandBuffer(int imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) !=VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording in command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = nereSwapChain->getRenderPass();
        renderPassInfo.framebuffer = nereSwapChain->getFrameBuffer(imageIndex);
        
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = nereSwapChain->getSwapChainExtent();
        
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(nereSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(nereSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0,0}, nereSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);
        
        nerePipeline->bind(commandBuffers[imageIndex]);
        nereModel->bind(commandBuffers[imageIndex]);
        nereModel->draw(commandBuffers[imageIndex]);
        
        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void FirstApp::drawFrame(){
        uint32_t imageIndex;
        auto result = nereSwapChain->acquireNextImage(&imageIndex);

        if(result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapChain();
            return;
        }

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to aqcuire next swapchain image!");
        }

        recordCommandBuffer(imageIndex);
        result = nereSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || nereWindow.wasWindowResized() ){
            nereWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if(result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }
}
