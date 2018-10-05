#include "volk.h"
#include <GLFW/glfw3.h>

#include <cstdint>
#include <cstdio>

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <queue>
#include <vector>

#include "mvk/Framebuffer.hpp"
#include "mvk/Instance.hpp"
#include "mvk/ImageView.hpp"

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

struct __attribute__ ((packed)) Vertex {
    float x, y;
    unsigned int color;
};

int main(int argc, char ** argv) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to init GLFW!");
    }

    //mvk::Instance::enableLayer(mvk::InstanceLayer::STANDARD_VALIDATION);
    //mvk::Instance::enableLayer(mvk::InstanceLayer::API_DUMP);
    mvk::Instance::enableRequiredGLFWExtensions();

    auto& instance = mvk::Instance::getCurrent();
    auto pPhysicalDevice = instance.getPhysicalDevice(0);
    auto pDevice = pPhysicalDevice->createDevice(std::set<std::string>({VK_KHR_SWAPCHAIN_EXTENSION_NAME}));

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    auto pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test Draw", nullptr, nullptr);
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    glfwCreateWindowSurface(instance, pWindow, nullptr, &surface);

    auto pQueueFamily = pDevice->getQueueFamily(0);

    if (!pQueueFamily->canPresent(surface)) {
        throw std::runtime_error("Selected QueueFamily cannot present image!");
    }

    auto swapchainCI = mvk::Swapchain::CreateInfo {};    
    swapchainCI.queueFamily = pQueueFamily;
    swapchainCI.surface = surface;

    auto pSwapchain = pDevice->createSwapchain(swapchainCI);
    pSwapchain->resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    auto imageCI = mvk::Image::CreateInfo {};
    imageCI.usage = mvk::ImageUsageFlag::COLOR_ATTACHMENT | mvk::ImageUsageFlag::TRANSFER_SRC;
    imageCI.extent.width = WINDOW_WIDTH;
    imageCI.extent.height = WINDOW_HEIGHT;
    imageCI.extent.depth = 1;
    imageCI.imageType = mvk::ImageType::IMAGE_2D;
    imageCI.format = mvk::Format::R8G8B8A8_UNORM;
    imageCI.arrayLayers = 1;
    imageCI.mipLevels = 1;
    imageCI.samples = 1;

    auto pImage = pDevice->createImage(imageCI, mvk::MemoryUsage::GPU_ONLY);
    auto imageView = mvk::ImageView(pImage);

    auto renderpassCI = mvk::RenderPass::CreateInfo {};

    {
        auto attachmentDescription = mvk::AttachmentDescription {};
        attachmentDescription.finalLayout = mvk::ImageLayout::COLOR_ATTACHMENT;
        attachmentDescription.format = pImage->getInfo().format;
        attachmentDescription.loadOp = mvk::LoadOp::CLEAR;
        attachmentDescription.storeOp = mvk::StoreOp::STORE;
        attachmentDescription.stencilLoadOp = mvk::LoadOp::DONT_CARE;
        attachmentDescription.stencilStoreOp = mvk::StoreOp::DONT_CARE;
        attachmentDescription.samples = 1;

        renderpassCI.attachments.push_back(attachmentDescription);
    }

    {
        auto subpassDescription = mvk::SubpassDescription {};

        {
            auto colorReference = mvk::AttachmentReference {};
            colorReference.attachment = 0;
            colorReference.layout = mvk::ImageLayout::COLOR_ATTACHMENT;
        
            subpassDescription.colorAttachments.push_back(colorReference);
        }

        renderpassCI.subpasses.push_back(subpassDescription);
    }

    {
        auto subpassDependency = mvk::SubpassDependency {};
        subpassDependency.srcSubpass = mvk::SUBPASS_EXTERNAL;
        subpassDependency.dstSubpass = 0;
        subpassDependency.srcStageMask = mvk::PipelineStageFlag::BOTTOM_OF_PIPE;
        subpassDependency.dstStageMask = mvk::PipelineStageFlag::COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.srcAccessMask = mvk::AccessFlag::MEMORY_READ | mvk::AccessFlag::MEMORY_WRITE;
        subpassDependency.dstAccessMask = mvk::AccessFlag::COLOR_ATTACHMENT_READ | mvk::AccessFlag::COLOR_ATTACHMENT_WRITE;

        renderpassCI.dependencies.push_back(subpassDependency);
    }

    auto pRenderPass = pDevice->createRenderPass(renderpassCI);

    auto framebufferCI = mvk::Framebuffer::CreateInfo {};
    framebufferCI.width = WINDOW_WIDTH;
    framebufferCI.height = WINDOW_HEIGHT;
    framebufferCI.layers = 1;

    auto framebuffer = mvk::Framebuffer(pRenderPass, framebufferCI, std::vector<const mvk::ImageView * > ({&imageView}));

    auto pipelineCI = mvk::GraphicsPipeline::CreateInfo {};
    
    {
        auto pushConstantRange = mvk::PushConstantRange {};
        pushConstantRange.stages = mvk::ShaderStage::VERTEX;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(float) * 16;

        pipelineCI.layoutInfo.pushConstantRanges.push_back(pushConstantRange);
    }

    {
        auto vertexStageCI = mvk::PipelineShaderStageCreateInfo {};
        vertexStageCI.stage = mvk::ShaderStage::VERTEX;
        vertexStageCI.moduleInfo.path = "shaders/testDraw/tris2D_colored.vert.spv";
        vertexStageCI.name = "main";

        auto fragmentStageCI = mvk::PipelineShaderStageCreateInfo {};
        fragmentStageCI.stage = mvk::ShaderStage::FRAGMENT;
        fragmentStageCI.moduleInfo.path = "shaders/testDraw/tris2D_colored.frag.spv";
        fragmentStageCI.name = "main";

        pipelineCI.stages.push_back(vertexStageCI);
        pipelineCI.stages.push_back(fragmentStageCI);
    }

    pipelineCI.inputAssemblyState.topology = mvk::PrimitiveTopology::TRIANGLE_LIST;
    pipelineCI.colorBlendState.blendConstants = mvk::colors::opaqueWhite();
    
    {
        auto attachment0Blend = mvk::PipelineColorBlendAttachmentState {};
        attachment0Blend.blendEnable = false;
        attachment0Blend.srcColorBlendFactor = mvk::BlendFactor::ONE;
        attachment0Blend.dstColorBlendFactor = mvk::BlendFactor::ZERO;
        attachment0Blend.srcAlphaBlendFactor = mvk::BlendFactor::ONE;
        attachment0Blend.dstAlphaBlendFactor = mvk::BlendFactor::ZERO;
        attachment0Blend.colorBlendOp = mvk::BlendOp::ADD;
        attachment0Blend.alphaBlendOp = mvk::BlendOp::ADD;
        attachment0Blend.colorWriteMask = mvk::ColorComponentFlag::A | mvk::ColorComponentFlag::R | mvk::ColorComponentFlag::G | mvk::ColorComponentFlag::B;

        pipelineCI.colorBlendState.attachments.push_back(attachment0Blend);
    }

    pipelineCI.rasterizationState.lineWidth = 1.0F;
    pipelineCI.multisampleState.rasterizationSamples = 1;
    
    {
        auto binding = mvk::VertexInputBindingDescription {};
        binding.binding = 0;
        binding.stride = sizeof(Vertex);
        
        pipelineCI.vertexInputState.vertexBindingDescriptions.push_back(binding);
    }

    {
        auto attribute = mvk::VertexInputAttributeDescription {};
        attribute.binding = 0;
        attribute.format = mvk::Format::R32G32_SFLOAT;
        attribute.offset = 0;
        attribute.location = 0;

        pipelineCI.vertexInputState.vertexAttributeDescriptions.push_back(attribute);

        attribute.format = mvk::Format::R8G8B8A8_UNORM;
        attribute.offset += 2 * sizeof(float);
        attribute.location = 1;

        pipelineCI.vertexInputState.vertexAttributeDescriptions.push_back(attribute);
    }

    auto pPipeline = pDevice->createPipeline(pipelineCI, pRenderPass);

    float constants[16];
    for (int i = 0; i < 16; i++) {
        constants[i] = static_cast<float> (i % 5 == 0);
    }

    auto bufferCI = mvk::Buffer::CreateInfo {};
    bufferCI.usage = mvk::BufferUsageFlag::VERTEX_BUFFER;
    bufferCI.size = 3 * sizeof(Vertex);

    auto pVertices = pDevice->createBuffer(bufferCI, mvk::MemoryUsage::CPU_TO_GPU);
    pVertices->mapping<Vertex>([](auto verts) {
        verts[0].x = -0.5F;
        verts[0].y = 0.0F;
        verts[0].color = 0xFF0000FF;

        verts[1].x = 0.5F;
        verts[1].y = -0.5F;
        verts[1].color = 0xFFFF00FF;

        verts[2].x = 0.0F;
        verts[2].y = 0.5F;
        verts[2].color = 0xFF00FF00;
    });

    bufferCI.usage = mvk::BufferUsageFlag::INDEX_BUFFER;
    bufferCI.size = 3 * 2;
    auto pIndices = pDevice->createBuffer(bufferCI, mvk::MemoryUsage::CPU_TO_GPU);
    pIndices->mapping<std::uint16_t>([](auto indices) {
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
    });

    auto pDrawCommand = pQueueFamily->getCurrentCommandPool()->allocate();
    pDrawCommand->begin(mvk::CommandBufferUsageFlag::SIMULTANEOUS_USE);
    pDrawCommand->beginRenderPass(framebuffer);
    pDrawCommand->setScissor(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    pDrawCommand->setViewport(0.0F, 0.0F, static_cast<float> (WINDOW_WIDTH), static_cast<float> (WINDOW_HEIGHT));
    pDrawCommand->bindPipeline(pPipeline);
    pDrawCommand->pushConstants(pPipeline, mvk::ShaderStage::VERTEX, 0, 16 * sizeof(float), constants);
    pDrawCommand->bindVertexBuffer(0, pVertices);
    pDrawCommand->bindIndexBuffer(pIndices, 0, mvk::IndexType::UINT16);
    pDrawCommand->drawIndexed(3);
    pDrawCommand->endRenderPass();
    pDrawCommand->end();

    auto pQueue = pQueueFamily->getQueue(0);

    auto presentInfo = mvk::Queue::PresentInfo {};
    presentInfo.image = pImage.get();
    presentInfo.imageAccess = mvk::AccessFlag::COLOR_ATTACHMENT_READ | mvk::AccessFlag::COLOR_ATTACHMENT_WRITE;
    presentInfo.imageLayout = mvk::ImageLayout::COLOR_ATTACHMENT;
    presentInfo.srcStageMask = mvk::PipelineStageFlag::COLOR_ATTACHMENT_OUTPUT_BIT;
    presentInfo.swapchain = pSwapchain.get();

    auto lastTime = glfwGetTime();
    int frames = 0;

    glfwShowWindow(pWindow);

    while (!glfwWindowShouldClose(pWindow)) {
        auto now = glfwGetTime();

        pQueue->submit(pDrawCommand.get());
        pQueue->present(presentInfo);

        glfwPollEvents();

        frames++;

        auto dTime = (now - lastTime) * 0.001;

        if (dTime > 5.0) {
            std::cout << "FPS: " << (frames / dTime) << std::endl;
            frames = 0;
            lastTime = now;
        }
    }

    pDevice->waitIdle();
    pDrawCommand = nullptr;
    pQueueFamily->detach();

    glfwDestroyWindow(pWindow);
    glfwTerminate();
}
