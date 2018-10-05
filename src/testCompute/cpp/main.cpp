#include <algorithm>
#include <iostream>
#include <vector>

#include "mvk/Instance.hpp"

constexpr unsigned int WORKGROUP_SIZE = 32;
constexpr unsigned int DATA_SIZE = 32;
constexpr int INPUT_BUFFER_BINDING = 0;
constexpr int OUTPUT_BUFFER_BINDING = 1;

int main(int argc, char ** argv) {
    mvk::Instance::enableLayer(mvk::InstanceLayer::STANDARD_VALIDATION);
    //mvk::Instance::enableLayer(mvk::InstanceLayer::API_DUMP);

    auto& instance = mvk::Instance::getCurrent();
    auto pPhysicalDevice = instance.getPhysicalDevice(0);
    auto pDevice = pPhysicalDevice->createDevice();

    auto bufferCI = mvk::Buffer::CreateInfo {};
    bufferCI.usage = mvk::BufferUsageFlag::STORAGE_BUFFER;
    bufferCI.size = DATA_SIZE * sizeof(float);

    auto pInputBuffer = pDevice->createBuffer(bufferCI, mvk::MemoryUsage::CPU_ONLY);

    std::cout << "Compute Shader Squaring\n";
    std::cout << "Inputs: [";

    pInputBuffer->mapping<float> ([&](auto pData) {
        for (unsigned int i = 0; i < DATA_SIZE; i++) {
            pData[i] = static_cast<float> (i);
            std::cout << std::dec << i << ", ";
        }
    });

    std::cout << "]" << std::endl;

    auto pOutputBuffer = pDevice->createBuffer(bufferCI, mvk::MemoryUsage::CPU_ONLY);

    auto pipelineCI = mvk::ComputePipeline::CreateInfo {};
    pipelineCI.stage.name = "main";
    pipelineCI.stage.stage = mvk::ShaderStage::COMPUTE;
    pipelineCI.stage.moduleInfo.path = "shaders/testCompute/square.comp.spv";

    {
        auto setLayoutInfo = mvk::DescriptorSetLayout::CreateInfo {};
    
        {
            auto dslBinding = mvk::DescriptorSetLayout::Binding {};
            dslBinding.stages = mvk::ShaderStage::COMPUTE;
            dslBinding.descriptorType = mvk::DescriptorType::STORAGE_BUFFER;
            dslBinding.descriptorCount = 1;
            dslBinding.binding = INPUT_BUFFER_BINDING;

            setLayoutInfo.bindings.push_back(dslBinding);

            dslBinding.binding = OUTPUT_BUFFER_BINDING;

            setLayoutInfo.bindings.push_back(dslBinding);
        }

        pipelineCI.layoutInfo.setLayoutInfos.push_back(setLayoutInfo);
    }

    auto pPipeline = pDevice->createPipeline(pipelineCI);
    auto pDescriptorSet = pPipeline->getDescriptorSetLayout(0)->allocate();

    pDescriptorSet->writeBuffer(mvk::DescriptorType::STORAGE_BUFFER, INPUT_BUFFER_BINDING, pInputBuffer);
    pDescriptorSet->writeBuffer(mvk::DescriptorType::STORAGE_BUFFER, OUTPUT_BUFFER_BINDING, pOutputBuffer);

    auto pQueueFamily = pDevice->getQueueFamily(0);
    auto pQueue = pQueueFamily->getQueue(0);
    auto pFence = pDevice->acquireFence();

    {
        auto pCommandBuffer = pQueueFamily->getCurrentCommandPool()->allocate();

        pCommandBuffer->begin(mvk::CommandBufferUsageFlag::ONE_TIME_SUBMIT);
        pCommandBuffer->bindPipeline(pPipeline);
        pCommandBuffer->bindDescriptorSet(pPipeline, 0, pDescriptorSet);

        auto gX = std::max(1u, DATA_SIZE / WORKGROUP_SIZE);

        pCommandBuffer->dispatch(gX);
        pCommandBuffer->end();

        pQueue->submit(pCommandBuffer.get(), pFence);
        pFence->waitFor();
        pFence->reset();
    }

    std::cout << "Output: [";

    pOutputBuffer->mapping<float>([&](auto pData) {
        std::for_each(pData, pData + DATA_SIZE, [](auto data) {
            std::cout << data << ", ";
        });
    });

    std::cout << "]" << std::endl;

    pQueueFamily->detach();

    return 0;
}
