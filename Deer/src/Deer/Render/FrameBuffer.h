#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Log.h"

namespace Deer {
	struct FrameBufferSpecification {
		unsigned int width, height;
		unsigned int samples;

		bool swapChainTarget = false;
		FrameBufferSpecification(unsigned int _width, unsigned int _height, unsigned int _samples = 1, bool _swapChainTarget = false)
			: width(_width), height(_height), samples(_samples), swapChainTarget(_swapChainTarget) {
		}
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() { DEER_CORE_TRACE("Destroying frame buffer"); }
		virtual const FrameBufferSpecification& getSpecification() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void clear() = 0;
		virtual void resize(unsigned int width, unsigned int height) = 0;

		virtual unsigned int getColorBufferID() = 0;

		static Ref<FrameBuffer> create(const FrameBufferSpecification& spec);
	};
}

