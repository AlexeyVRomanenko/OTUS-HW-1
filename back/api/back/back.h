#pragma once
#include <back/fwd.h>
#include <front/ifront.h>

namespace back
{
	class IBack
	{
	public:
		virtual ~IBack() = default;
	};

	std::shared_ptr<IBack> CreateBack(const std::weak_ptr<front::IFront>&, uint32_t N);
}