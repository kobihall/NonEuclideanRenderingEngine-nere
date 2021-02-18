#include "nere_pipeline.hpp"

//std
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace nere {

    NerePipeline::NerePipeline(const std::string& vertFilepath){
        createGraphicsPipeline(vertFilepath);
    }

    std::vector<char> NerePipeline::readFile(const std::string& filepath){
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};

        if(!file.is_open()){
            throw std::runtime_error("failed to open file:" + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void NerePipeline::createGraphicsPipeline(const std::string& vertFilepath){
        auto vertCode = readFile(vertFilepath);

        std::cout << "Vertex Shader Code Size:" << vertCode.size() << '\n';
    }
}