#pragma once

namespace wrapper::samples {

	class noncopyable {
	public:
		noncopyable() = default;
		virtual ~noncopyable() = default;

		noncopyable(const noncopyable&) = delete;
		noncopyable(noncopyable&&) noexcept = default;

		noncopyable& operator=(const noncopyable&) = delete;
		noncopyable& operator=(noncopyable&&) = default;
	};
	
}
