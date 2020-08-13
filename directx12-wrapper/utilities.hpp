#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

#include <cassert>
#include <string>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#undef max
#undef min

#define __LOG_SPDLOG_IMPLEMENTATION__
#ifndef __LOG_SPDLOG_IMPLEMENTATION__
#else
#pragma warning(disable : 4275)
#include <spdlog/spdlog.h>
#endif

#ifdef __LOG_SPDLOG_IMPLEMENTATION__
#ifdef __LOG_USER_IMPLEMENTATION__
static_assert(false, "'__LOG_SPDLOG_IMPLEMENTATION__' and '__LOG_USER_IMPLEMENTATION__' can not be defined at the same time.");
#endif
#endif

namespace wrapper::directx12 {

	using Microsoft::WRL::ComPtr;

	using uint32 = unsigned;
	using int32 = int;
	using real = float;
	
	size_t size_of(const DXGI_FORMAT& format);

	size_t align_to(size_t value, size_t alignment);

	D3D12_CPU_DESCRIPTOR_HANDLE offset_handle(const D3D12_CPU_DESCRIPTOR_HANDLE& handle, size_t value);

	D3D12_GPU_DESCRIPTOR_HANDLE offset_handle(const D3D12_GPU_DESCRIPTOR_HANDLE& handle, size_t value);

	std::string copy_data_to_string(void* data, size_t size_in_bytes);

	std::string wide_string_to_multi_bytes_string(const std::wstring& wide_string);
	
	template <typename... Args>
	void debug(const std::string_view& format, const Args& ... args);

	template <typename... Args>
	void info(const std::string_view& format, const Args& ... args);

	template <typename... Args>
	void warn(const std::string_view& format, const Args& ... args);

	template <typename... Args>
	void error(const std::string_view& format, const Args& ... args);

#ifdef __LOG_SPDLOG_IMPLEMENTATION__
	template <typename... Args>
	void debug(const std::string_view& format, const Args& ... args) { spdlog::debug(format, args...); }

	template <typename... Args>
	void info(const std::string_view& format, const Args& ... args) { spdlog::info(format, args...); }

	template <typename... Args>
	void warn(const std::string_view& format, const Args& ... args) { spdlog::warn(format, args...); }

	template <typename... Args>
	void error(const std::string_view& format, const Args& ... args) { spdlog::error(format, args...); }
#else
	template <typename... Args>
	void debug(const std::string_view& format, const Args& ... args) {}

	template <typename... Args>
	void info(const std::string_view& format, const Args& ... args) {}

	template <typename... Args>
	void warn(const std::string_view& format, const Args& ... args) {}

	template <typename... Args>
	void error(const std::string_view& format, const Args& ... args) {}
#endif
	
}
