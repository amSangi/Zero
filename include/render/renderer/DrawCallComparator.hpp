#pragma once

#include <memory>
#include "render/renderer/IDrawCall.hpp"

namespace zero::render
{

	class DrawCallComparator
	{
	public:
		DrawCallComparator() = delete;
		static bool CompareDrawCalls(const std::unique_ptr<IDrawCall>& lhs, const std::unique_ptr<IDrawCall>& rhs);
	};

} // namespace zero::render