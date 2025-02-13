/****************************************************************************
 Copyright (c) 2021 Xiamen Yaji Software Co., Ltd.

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

// clang-format off
#include "core/builtin/Effects.h"
#include "base/StringUtil.h"
namespace cc {

// absolute essential effects
const GzipedString BUILTIN_EFFECTS { 
"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"
};
} // namespace cc
