#pragma once

#include "commands/command_queue.hpp"
#include "resources/resource.hpp"

#include <vector>

namespace wrapper::directx12 {

	class swap_chain final {
	public:
		swap_chain() = default;

		explicit swap_chain(const ComPtr<IDXGISwapChain4>& source);

		~swap_chain() = default;

		IDXGISwapChain4* const* get_address_off() const;
		IDXGISwapChain4* operator->() const;
		IDXGISwapChain4* get() const;

		static swap_chain create(const command_queue& queue, int width, int height, HWND handle);
	private:
		std::vector<resource> mTextures;
		
		ComPtr<IDXGISwapChain4> mSwapChain;

		DXGI_SWAP_CHAIN_DESC mDesc;
	};
	
}
