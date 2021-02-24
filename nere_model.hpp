//
//  nere_model.hpp
//  NonEuclideanRenderingEngine
//
//  Created by Kobi Hall on 2/24/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//
#pragma once

#include "nere_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace nere {
    class NereModel {
        public:

        struct Vertex {
            glm::vec2 position;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        NereModel(NereDevice &device, const std::vector<Vertex> &vertices);
        ~NereModel();

        NereModel(const NereModel &) = delete;
        NereModel &operator=(const NereModel &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        NereDevice& nereDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}