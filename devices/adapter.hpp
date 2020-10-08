#pragma once

#include "../interfaces/wrapper.hpp"

namespace wrapper::directx12 {

	class adapter final : public wrapper_t<IDXGIAdapter1> {
	public:
		adapter() = default;

		explicit adapter(const ComPtr<IDXGIAdapter1>& source);

		~adapter() = default;

		static std::vector<adapter> all();
	private:
		DXGI_ADAPTER_DESC1 mDesc;
	};
	
}
