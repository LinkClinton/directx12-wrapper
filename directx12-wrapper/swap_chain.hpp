#pragma once

#include "commands/command_queue.hpp"
#include "resources/texture2d.hpp"

#include <vector>

namespace wrapper::directx12 {

	class swap_chain final : public wrapper_t<IDXGISwapChain4> {
	public:
		swap_chain() = default;

		explicit swap_chain(const ComPtr<IDXGISwapChain4>& source);

		~swap_chain() = default;
		
		void present(bool sync = true) const;

		DXGI_FORMAT format() const;
		
		const std::vector<texture2d>& buffers() const;
		
		static swap_chain create(const command_queue& queue, int width, int height, HWND handle);
	private:
		std::vector<texture2d> mTextures;
		
		DXGI_SWAP_CHAIN_DESC mDesc = {};
	};
	
}
