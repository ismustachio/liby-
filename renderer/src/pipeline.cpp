#include "pipeline.hpp"
#include "utils.hpp"
#include <shaderc/shaderc.hpp>
#include <sys/endian.h>
#include <vulkan/vulkan_core.h>

namespace liby {
namespace renderer {
Pipeline::Pipeline(const std::string &vertFilePath,
                   const std::string &fragFilePath, Device &device,
                   const PipelineConfigInfo &configInfo)
    : device_{device} {
  createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
}

Pipeline::~Pipeline() {
  vkDestroyShaderModule(device_.device(), vertShaderModule_, nullptr);
  vkDestroyShaderModule(device_.device(), fragShaderModule_, nullptr);
  vkDestroyPipeline(device_.device(), graphicsPipeline_, nullptr);
}

void Pipeline::createShaderModule(const std::vector<char> &code,
                                  VkShaderModule *shaderModule) {
  VkShaderModuleCreateInfo createInfo{
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .codeSize = code.size(),
      .pCode = reinterpret_cast<const uint32_t *>(code.data()),
  };

  if (vkCreateShaderModule(device_.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module!");
  }
}

void Pipeline::createGraphicsPipeline(const std::string &vertFilePath,
                                      const std::string &fragFilePath,
                                      const PipelineConfigInfo &configInfo) {
  assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
         "Cannot create graphics pipeline: no pipelineLayout provided in "
         "configInfo");
  assert(configInfo.renderPass != VK_NULL_HANDLE &&
         "Cannot create graphics pipeline: no renderPass provided in "
         "configInfo");
  auto vertShaderCode = readShaderFile(vertFilePath);
  auto fragShaderCode = readShaderFile(fragFilePath);
  createShaderModule(vertShaderCode, &vertShaderModule_);
  createShaderModule(fragShaderCode, &fragShaderModule_);

  VkPipelineShaderStageCreateInfo shaderStages[]{
      {
          .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .stage = VK_SHADER_STAGE_VERTEX_BIT,
          .module = vertShaderModule_,
          .pName = "main",
          .pSpecializationInfo = nullptr,
      },
      {
          .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
          .module = fragShaderModule_,
          .pName = "main",
          .pSpecializationInfo = nullptr,
      },
  };

  auto bindingDescriptions = Model::Vertex::getBindingDescriptions();
  auto attributeDescriptions = Model::Vertex::getAttributeDescriptions();
  VkPipelineVertexInputStateCreateInfo vertexInputInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount =
          static_cast<uint32_t>(bindingDescriptions.size()),
      .pVertexBindingDescriptions = bindingDescriptions.data(),
      .vertexAttributeDescriptionCount =
          static_cast<uint32_t>(attributeDescriptions.size()),
      .pVertexAttributeDescriptions = attributeDescriptions.data(),
  };

  VkGraphicsPipelineCreateInfo pipelineInfo{
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .stageCount = 2,
      .pStages = shaderStages,
      .pVertexInputState = &vertexInputInfo,
      .pInputAssemblyState = &configInfo.inputAssemblyInfo,
      .pViewportState = &configInfo.viewportInfo,
      .pRasterizationState = &configInfo.rasterizationInfo,
      .pMultisampleState = &configInfo.multisampleInfo,
      .pDepthStencilState = &configInfo.depthStencilInfo,
      .pColorBlendState = &configInfo.colorBlendInfo,
      .pDynamicState = &configInfo.dynamicStateInfo,
      .layout = configInfo.pipelineLayout,
      .renderPass = configInfo.renderPass,
      .subpass = configInfo.subpass,
      .basePipelineHandle = VK_NULL_HANDLE,
      .basePipelineIndex = -1,
  };

  if (vkCreateGraphicsPipelines(device_.device(), VK_NULL_HANDLE, 1,
                                &pipelineInfo, nullptr,
                                &graphicsPipeline_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create graphics pipeline");
  }
}

void Pipeline::bind(VkCommandBuffer commandBuffer) {
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline_);
}

PipelineConfigInfo PipelineConfigInfo::defaultPipelineConfig(void) {
  PipelineConfigInfo configInfo{};
  configInfo.inputAssemblyInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  // every 3 vertices are grouped together into a triangle
  configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

  configInfo.viewportInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  configInfo.viewportInfo.viewportCount = 1;
  configInfo.viewportInfo.pViewports = nullptr;
  configInfo.viewportInfo.scissorCount = 1;
  configInfo.viewportInfo.pScissors = nullptr;

  configInfo.rasterizationInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
  configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
  configInfo.rasterizationInfo.lineWidth = 1.0f;
  configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
  configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
  configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
  configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
  configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
  configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

  configInfo.multisampleInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
  configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  configInfo.multisampleInfo.minSampleShading = 1.0f;
  configInfo.multisampleInfo.pSampleMask = nullptr;
  configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
  configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

  configInfo.colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
  configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  configInfo.colorBlendInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
  configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
  configInfo.colorBlendInfo.attachmentCount = 1;
  configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
  configInfo.colorBlendInfo.blendConstants[0] = 0.0f;
  configInfo.colorBlendInfo.blendConstants[1] = 0.0f;
  configInfo.colorBlendInfo.blendConstants[2] = 0.0f;
  configInfo.colorBlendInfo.blendConstants[3] = 0.0f;

  configInfo.depthStencilInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
  configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
  configInfo.depthStencilInfo.minDepthBounds = 0.0f;
  configInfo.depthStencilInfo.maxDepthBounds = 1.0f;
  configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
  configInfo.depthStencilInfo.front = {};
  configInfo.depthStencilInfo.back = {};

  configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT,
                                    VK_DYNAMIC_STATE_SCISSOR};
  configInfo.dynamicStateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  configInfo.dynamicStateInfo.pDynamicStates =
      configInfo.dynamicStateEnables.data();
  configInfo.dynamicStateInfo.dynamicStateCount =
      static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
  configInfo.dynamicStateInfo.flags = 0;
  return configInfo;
}

// size_t Pipeline::compileShader(glslang_stage_t stage,
//                                const std::vector<char> &shaderSource,
//                                ShaderModule &shaderModule) {
//   const glslang_input_t input = {
//       .language = GLSLANG_SOURCE_GLSL,
//       .stage = stage,
//       .client = GLSLANG_CLIENT_VULKAN,
//       .client_version = GLSLANG_TARGET_VULKAN_1_2,
//       .target_language = GLSLANG_TARGET_SPV,
//       .target_language_version = GLSLANG_TARGET_SPV_1_5,
//       .code = shaderSource.data(),
//       .default_version = 100,
//       .default_profile = GLSLANG_NO_PROFILE,
//       .force_default_version_and_profile = false,
//       .forward_compatible = false,
//       .messages = GLSLANG_MSG_DEFAULT_BIT,
//       .resource = reinterpret_cast<const glslang_resource_t *>(
//           &DefaultTBuiltInResource),
//   };
//
//   glslang_shader_t *shader = glslang_shader_create(&input);
//   if (!glslang_shader_preprocess(shader, &input)) {
//     fprintf(stderr, "GLSL preprocessing failed\n");
//     fprintf(stderr, "\n%s", glslang_shader_get_info_log(shader));
//     fprintf(stderr, "\n%s", glslang_shader_get_info_debug_log(shader));
//     fprintf(stderr, "code:\n%s", input.code);
//     exit(EXIT_FAILURE);
//   }
//   if (!glslang_shader_parse(shader, &input)) {
//     fprintf(stderr, "GLSL parsing failed\n");
//     fprintf(stderr, "\n%s", glslang_shader_get_info_log(shader));
//     fprintf(stderr, "\n%s", glslang_shader_get_info_debug_log(shader));
//     fprintf(stderr, "%s", glslang_shader_get_preprocessed_code(shader));
//     exit(EXIT_FAILURE);
//   }
//   auto *program = glslang_program_create();
//   glslang_program_add_shader(program, shader);
//   int messages = GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT;
//   if (!glslang_program_link(program, messages)) {
//     fprintf(stderr, "GLSL linking failed\n");
//     fprintf(stderr, "\n%s", glslang_program_get_info_log(program));
//     fprintf(stderr, "\n%s", glslang_program_get_info_debug_log(program));
//     exit(EXIT_FAILURE);
//   }
//   glslang_program_SPIRV_generate(program, stage);
//   shaderModule.spirv.resize(glslang_program_SPIRV_get_size(program));
//   glslang_program_SPIRV_get(program, shaderModule.spirv.data());
//
//   const char *spirv_messages = glslang_program_SPIRV_get_messages(program);
//   if (spirv_messages) {
//     fprintf(stderr, "%s", spirv_messages);
//   }
//   glslang_program_delete(program);
//   glslang_shader_delete(shader);
//   return shaderModule.spirv.size();
// }

// size_t Pipeline::compileShaderFile(const std::string &file,
//                                        ShaderModule &shaderModule) {
//   // if (; !shaderSource.empty()) {
//   // return compileShader(glslangShaderStageFromFileName(file),
//   shaderSource,
//   //                    shaderModule);
//   //}
//   auto shaderSource = readFile(file);
//   shaderc::Compiler compiler;
//   shaderc::CompileOptions options;
//   shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
//       file, shaderc_glsl_vertex_shader, shaderSource.data());
//
//   if (module.GetCompilationStatus() != shaderc_compilation_status_success)
//   {
//     std::cerr << module.GetErrorMessage();
//     exit(1);
//   }
//   shaderModule.spirv = {module.cbegin(), module.cend()};
//
//   return 0;
// }

} // namespace renderer
} // namespace liby
