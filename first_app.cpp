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
#include <array>

namespace nere {

    FirstApp::FirstApp(UserSettings &userSettings) : userSettings_{userSettings} {
        //userSettings_ = userSettings;
        loadModels();
        createDescriptorSet();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() {
        for(size_t i = 0; i < nereSwapChain.imageCount(); i++){
            vkFreeMemory(nereDevice.device(), descriptorBuffersMemory[i], nullptr);
            vkDestroyBuffer(nereDevice.device(), descriptorBuffers[i], nullptr);
        }
        vkDestroyDescriptorPool(nereDevice.device(), descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(nereDevice.device(), descriptorSetLayout, nullptr);
        vkDestroyPipelineLayout(nereDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {

        while(!nereWindow.shouldClose()){
        //for(int i=0; i<2; i++){
            // process all pending glfw events
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(nereDevice.device());
    }

    void FirstApp::loadModels() {
        std::vector<NereModel::Vertex> vertices;
        switch(userSettings_.chosenPipeline) {
        case NERE_GRAPHICS_PIPELINE:
            vertices.push_back({{0.0f, -0.5f}});
            vertices.push_back({{0.5f, 0.5f}});
            vertices.push_back({{-0.5f, 0.5f}});
            break;
        case NERE_COMPUTE_PIPELINE:
            vertices.push_back({{0.5f, -0.5f}});
            vertices.push_back({{0.5f, 0.5f}});
            vertices.push_back({{-0.5f, 0.5f}});
            vertices.push_back({{-0.5f, 0.5f}});
            vertices.push_back({{-0.5f,-0.5f}});
            vertices.push_back({{0.5, -0.5f}});
            break;
        throw std::runtime_error("no render pipeline specified in UserSettings!");
        }

        nereModel = std::make_unique<NereModel>(nereDevice, vertices);
    }

    void FirstApp::createDescriptorSet() {
        VkDescriptorSetLayoutBinding descriptorSetLayoutBinding{};
        descriptorSetLayoutBinding.binding = 0;
        descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorSetLayoutBinding.descriptorCount = 1;
        descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL;

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.bindingCount = 1; 
        descriptorSetLayoutCreateInfo.pBindings = &descriptorSetLayoutBinding; 

        // Create the descriptor set layout. 
        if(vkCreateDescriptorSetLayout(nereDevice.device(), &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }

        VkDescriptorPoolSize descriptorPoolSize = {};
        descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorPoolSize.descriptorCount = static_cast<uint32_t>(nereSwapChain.imageCount());//1;

        VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
        descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCreateInfo.maxSets = static_cast<uint32_t>(nereSwapChain.imageCount());//1; // we only need to allocate one descriptor set from the pool.
        descriptorPoolCreateInfo.poolSizeCount = 1;
        descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;

        if(vkCreateDescriptorPool(nereDevice.device(), &descriptorPoolCreateInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

        std::vector<VkDescriptorSetLayout> layouts(nereSwapChain.imageCount(), descriptorSetLayout);
        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
        descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO; 
        descriptorSetAllocateInfo.descriptorPool = descriptorPool; // pool to allocate from.
        descriptorSetAllocateInfo.descriptorSetCount = static_cast<uint32_t>(nereSwapChain.imageCount());//1; // allocate a single descriptor set.
        descriptorSetAllocateInfo.pSetLayouts = layouts.data();//&descriptorSetLayout;
        descriptorSets.resize(nereSwapChain.imageCount());

        if(vkAllocateDescriptorSets(nereDevice.device(), &descriptorSetAllocateInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        VkDeviceSize bufferSize = sizeof(Pixel) * WIDTH * HEIGHT;
        descriptorBuffers.resize(nereSwapChain.imageCount());
        descriptorBuffersMemory.resize(nereSwapChain.imageCount());
        for(size_t i = 0; i < nereSwapChain.imageCount(); i++){
            nereDevice.createBuffer(
                bufferSize, 
                VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                descriptorBuffers[i],
                descriptorBuffersMemory[i]);
        }

        for(size_t i=0; i<nereSwapChain.imageCount(); i++){
            VkDescriptorBufferInfo descriptorBufferInfo = {};
            descriptorBufferInfo.buffer = descriptorBuffers[i];
            descriptorBufferInfo.offset = 0;
            descriptorBufferInfo.range = bufferSize;
        
            VkWriteDescriptorSet writeDescriptorSet = {};
            writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet.dstSet = descriptorSets[i]; // write to this descriptor set.
            writeDescriptorSet.dstBinding = 0; // write to the first, and only binding.
            writeDescriptorSet.descriptorCount = 1; // update a single descriptor.
            writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; // storage buffer.
            writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;

            vkUpdateDescriptorSets(nereDevice.device(), 1, &writeDescriptorSet, 0, nullptr);
        }
    }

    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        switch(userSettings_.chosenPipeline) {
                case NERE_GRAPHICS_PIPELINE:
                    pipelineLayoutInfo.setLayoutCount = 0;
                    pipelineLayoutInfo.pSetLayouts = nullptr;
                    break;
                case NERE_COMPUTE_PIPELINE:
                    pipelineLayoutInfo.setLayoutCount = 1;
                    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
                    break;
                throw std::runtime_error("no render pipeline specified in UserSettings!");
            }
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
        pipelineConfig.userSettings = userSettings_;
        nerePipeline = std::make_unique<NerePipeline>(
            nereDevice,
            "../shaders/",
            pipelineConfig
        );
    }

    void FirstApp::createCommandBuffers(){
        commandBuffers.resize(nereSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = nereDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(nereDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for(int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) !=VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording in command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = nereSwapChain.getRenderPass();
            renderPassInfo.framebuffer = nereSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = nereSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            nerePipeline->bind(commandBuffers[i]);
            
            switch(userSettings_.chosenPipeline) {
                case NERE_GRAPHICS_PIPELINE:
                    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
                    nereModel->bind(commandBuffers[i]);
                    nereModel->draw(commandBuffers[i]);
                    vkCmdEndRenderPass(commandBuffers[i]);
                    break;
                case NERE_COMPUTE_PIPELINE:
                    vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
                    vkCmdDispatch(commandBuffers[i], (uint32_t)ceil(WIDTH / 32), (uint32_t)ceil(HEIGHT / 32), 1);
                    vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
                    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
                    nereModel->bind(commandBuffers[i]);
                    nereModel->draw(commandBuffers[i]);
                    vkCmdEndRenderPass(commandBuffers[i]);
                    break;
                throw std::runtime_error("no render pipeline specified in UserSettings!");
            }

            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }
    void FirstApp::drawFrame(){
        // find the next image's index to write to
        uint32_t imageIndex;
        auto result = nereSwapChain.acquireNextImage(&imageIndex);

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to aqcuire next swapchain image!");
        }
        switch(userSettings_.chosenPipeline) {
            case NERE_GRAPHICS_PIPELINE:
                result = nereSwapChain.submitGraphicsCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
                break;
            case NERE_COMPUTE_PIPELINE:
                result = nereSwapChain.submitGraphicsCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
                break;
            throw std::runtime_error("no render pipeline specified in UserSettings!");
        }
        if(result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }
}
