#pragma once
#include "Deer/Render/FrameBuffer.h"

namespace Deer {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& frameBufferSpecification);
		~OpenGLFrameBuffer() override;

		void bind() override;
		void unbind() override;

		void clear() override;
		void resize(unsigned int width, unsigned int height);

		unsigned int getColorBufferID() override { return m_colorAttachment; }

		const FrameBufferSpecification& getSpecification() { return m_specification; }
	private:
		void invalidate();
	private:
		FrameBufferSpecification m_specification;

		unsigned int m_frameBuffer = 0;
		unsigned int m_colorAttachment = 0;
		unsigned int m_depthAttachment = 0;
	};
}

