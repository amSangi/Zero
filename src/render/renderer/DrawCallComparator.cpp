#include "render/renderer/DrawCallComparator.hpp"

namespace zero::render
{

bool DrawCallComparator::CompareDrawCalls(const std::unique_ptr<IDrawCall>& lhs, const std::unique_ptr<IDrawCall>& rhs)
{
    const DrawKey& lhs_draw_key = lhs->GetDrawKey();
    const DrawKey& rhs_draw_key = rhs->GetDrawKey();

    // Sort on Mesh ID
    if (lhs_draw_key.mesh_id_ < rhs_draw_key.mesh_id_)
    {
        return true;
    }
    else if (lhs_draw_key.mesh_id_ > rhs_draw_key.mesh_id_)
    {
        return false;
    }

    // Sort on Material ID
    if (lhs_draw_key.material_id_ < rhs_draw_key.material_id_)
    {
        return true;
    }
    else if (lhs_draw_key.material_id_ > rhs_draw_key.material_id_)
    {
        return false;
    }

    // Sort on Texture ID
    return lhs_draw_key.texture_id_ < rhs_draw_key.texture_id_;
}

} // namespace zero::render