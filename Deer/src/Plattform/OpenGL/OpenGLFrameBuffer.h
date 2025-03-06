#pragma once
#include "DeerRender/Render/FrameBuffer.h"

#include <vector>

namespace Deer {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& frameBufferSpecification);
		~OpenGLFrameBuffer() override;

		void bind() override;
		void unbind() override;

		void clear() override;
		void resize(unsigned int width, unsigned int height) override;

		unsigned int getTextureBufferID(int id = 0) override { return m_colorsAttachments[id]; }
		void clearBuffer(unsigned int bufferId, void* data) override;

		int getTextureBufferPixel(int id, unsigned int x, unsigned int y) override;

		const FrameBufferSpecification& getSpecification() override { return m_specification; }
	private:
		void invalidate();
	private:
		FrameBufferSpecification m_specification;
		std::vector<unsigned int> m_colorsAttachments;

		unsigned int m_frameBuffer = 0;
		unsigned int m_depthAttachment = 0;
	};
}

