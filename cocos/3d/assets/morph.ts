/*
 Copyright (c) 2020 Xiamen Yaji Software Co., Ltd.

 https://www.cocos.com/

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
 */



import { AttributeName, Device, DescriptorSet } from '../../core/gfx';
import { Mesh } from './mesh';
import { StdMorphRendering } from './morph-rendering';
import { IMacroPatch } from '../../core/renderer';

export interface Morph {
    /**
     * Morph data of each sub-mesh.
     */
    subMeshMorphs: (SubMeshMorph | null)[];

    /**
     * Common initial weights of each sub-mesh.
     */
    weights?: number[];

    /**
     * Name of each target of each sub-mesh morph.
     * This field is only meaningful if every sub-mesh has the same number of targets.
     */
    targetNames?: string[];
}

export interface MorphTarget {
    /**
     * Displacement of each target attribute.
     */
    displacements: Mesh.IBufferView[];
}

export interface SubMeshMorph {
    /**
     * Attributes to morph.
     */
    attributes: AttributeName[];

    /**
     * Targets.
     */
    targets: MorphTarget[];

    /**
     * Initial weights of each target.
     */
    weights?: number[];
}

export function createMorphRendering (mesh: Mesh, gfxDevice: Device): MorphRendering | null {
    return new StdMorphRendering(mesh, gfxDevice);
}

/**
 * Class which control rendering of a morph resource.
 */
export interface MorphRendering {
    createInstance (): MorphRenderingInstance;
}

/**
 * This rendering instance of a morph resource.
 */
export interface MorphRenderingInstance {
    /**
     * Sets weights of targets of specified sub mesh.
     * @param subMeshIndex
     * @param weights
     */
    setWeights (subMeshIndex: number, weights: number[]): void;

    /**
     * Adapts pipeline state to do the rendering.
     * @param subMeshIndex
     * @param pipelineState
     */
    adaptPipelineState (subMeshIndex: number, descriptorSet: DescriptorSet): void;

    requiredPatches (subMeshIndex: number): IMacroPatch[] | null;

    /**
     * Destroy the rendering instance.
     */
    destroy (): void;
}
