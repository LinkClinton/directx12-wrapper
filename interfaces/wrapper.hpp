#pragma once

#include "../utilities.hpp"

namespace wrapper::directx12 {

	template <typename T>
	class wrapper_t {
	public:
		wrapper_t() = default;

		explicit wrapper_t(const ComPtr<T>& source);

		virtual ~wrapper_t() = default;

		bool valid() const noexcept { return mWrapperInstance.Get() != nullptr; }
		
		T* const* get_address_off() const;
		T* operator->() const;
		T* get() const;
	protected:
		ComPtr<T> mWrapperInstance;
	};

	template <typename T>
	wrapper_t<T>::wrapper_t(const ComPtr<T>& source) : mWrapperInstance(source)
	{
	}

	template <typename T>
	T* const* wrapper_t<T>::get_address_off() const
	{
		return mWrapperInstance.GetAddressOf();
	}

	template <typename T>
	T* wrapper_t<T>::operator->() const
	{
		return mWrapperInstance.Get();
	}

	template <typename T>
	T* wrapper_t<T>::get() const
	{
		return mWrapperInstance.Get();
	}

}
