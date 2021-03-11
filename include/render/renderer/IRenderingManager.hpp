#pragma once

#include "core/NonCopyable.hpp"
#include "render/renderer/IModelManager.hpp"
#include "render/renderer/IRenderingContext.hpp"
#include "render/renderer/IShaderManager.hpp"
#include "render/renderer/ITextureManager.hpp"
#include "render/renderer/IUniformManager.hpp"

namespace zero::render
{

    class IRenderingManager : public NonCopyable
    {
    public:
        virtual ~IRenderingManager() = default;
        virtual IModelManager* GetModelManager() = 0;
        virtual IRenderingContext* GetRenderingContext() = 0;
        virtual IShaderManager* GetShaderManager() = 0;
        virtual ITextureManager* GetTextureManager() = 0;
        virtual IUniformManager* GetUniformManager() = 0;
    };

} // namespace zero::render