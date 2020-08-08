#pragma once

#include "noncopyable.hpp"

#include <Windows.h>
#include <string>

namespace wrapper::samples {

	class application : public noncopyable {
	public:
		explicit application(const std::string& name, int width, int height);

		~application() = default;

		void run_loop();
	protected:
		virtual void render(float delta) = 0;

		virtual void update(float delta) = 0;
		
		void process_message(const MSG& message);
	protected:
		std::string mName;

		int mWidth, mHeight;

		bool mExisted;

		HWND mHandle;
	};
	
}
