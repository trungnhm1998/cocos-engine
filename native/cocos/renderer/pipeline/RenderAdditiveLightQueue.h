/****************************************************************************
 Copyright (c) 2020-2022 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/

#pragma once

#include "Define.h"
#include "base/std/container/array.h"

namespace cc {
namespace scene {
class Camera;
class Pass;
class Light;
class SpotLight;
class SphereLight;
} // namespace scene
namespace pipeline {
struct RenderObject;
class RenderPipeline;
class RenderInstancedQueue;
class RenderBatchedQueue;
class ForwardPipeline;

struct AdditiveLightPass {
    const scene::SubModel *subModel = nullptr;
    const scene::Pass *    pass     = nullptr;
    gfx::Shader *          shader   = nullptr;
    ccstd::vector<uint>    dynamicOffsets;
    ccstd::vector<uint>    lights;
};

class RenderAdditiveLightQueue final {
public:
    explicit RenderAdditiveLightQueue(RenderPipeline *pipeline);
    ~RenderAdditiveLightQueue();

    void recordCommandBuffer(gfx::Device *device, scene::Camera *camera, gfx::RenderPass *renderPass, gfx::CommandBuffer *cmdBuffer);
    void gatherLightPasses(const scene::Camera *camera, gfx::CommandBuffer *cmdBuffer);

private:
    static bool cullSphereLight(const scene::SphereLight *light, const scene::Model *model);
    static bool cullSpotLight(const scene::SpotLight *light, const scene::Model *model);

    void clear();
    void addRenderQueue(const scene::Pass *pass, const scene::SubModel *subModel, const scene::Model *model, uint lightPassIdx);
    void updateUBOs(const scene::Camera *camera, gfx::CommandBuffer *cmdBuffer);
    void updateLightDescriptorSet(const scene::Camera *camera, gfx::CommandBuffer *cmdBuffer);
    bool getLightPassIndex(const scene::Model *model, ccstd::vector<uint> *lightPassIndices) const;
    void lightCulling(const scene::Model *model);

    RenderPipeline *                                _pipeline = nullptr;
    ccstd::vector<ccstd::vector<scene::SubModel *>> _sortedSubModelsArray;
    ccstd::vector<ccstd::vector<uint>>              _sortedPSOCIArray;
    ccstd::vector<const scene::Light *>             _validPunctualLights;
    ccstd::vector<uint>                             _lightIndices;
    ccstd::vector<AdditiveLightPass>                _lightPasses;
    ccstd::vector<uint>                             _dynamicOffsets;
    ccstd::vector<float>                            _lightBufferData;
    RenderInstancedQueue *                          _instancedQueue       = nullptr;
    RenderBatchedQueue *                            _batchedQueue         = nullptr;
    gfx::Buffer *                                   _lightBuffer          = nullptr;
    gfx::Buffer *                                   _firstLightBufferView = nullptr;

    ccstd::array<float, UBOShadow::COUNT> _shadowUBO{};

    uint  _lightBufferStride       = 0;
    uint  _lightBufferElementCount = 0;
    uint  _lightBufferCount        = 16;
    float _lightMeterScale         = 10000.0F;
    uint  _phaseID                 = 0;
};

} // namespace pipeline
} // namespace cc
