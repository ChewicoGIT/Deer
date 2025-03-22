#include "OpenGLFrameBuffer.h"
#include "Deer/Core/Log.h"
#include "Deer/Application.h"

#include "glad/glad.h"

namespace Deer {
	Ref<FrameBuffer> FrameBuffer::create(const FrameBufferSpecification& spec) {
		return Ref<FrameBuffer>(new OpenGLFrameBuffer(spec));
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& frameBufferSpecification)
		: m_specification(frameBufferSpecification) {
		invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		if (m_frameBuffer != 0) {
			glDeleteFramebuffers(1, &m_frameBuffer);
			glDeleteRenderbuffers(1, &m_depthAttachment);
			for (auto id : m_colorsAttachments)
				glDeleteTextures(1, &id);
		}
	}

	void OpenGLFrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glViewport(0, 0, m_specification.width, m_specification.height);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLFrameBuffer::unbind() {
		unsigned int width = Application::s_application->m_window->getWitdth();
		unsigned int height = Application::s_application->m_window->getHeight();
		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::clear() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLFrameBuffer::resize(unsigned int width, unsigned int height) {
		m_specification.width = width;
		m_specification.height = height;

		invalidate();
	}

	void OpenGLFrameBuffer::clearBuffer(unsigned int bufferId, void* data) {
		switch (m_specification.frameBufferTextures[bufferId]) {
		case TextureBufferType::RGBA8:
			glClearTexImage(m_colorsAttachments[bufferId], 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;

		case TextureBufferType::RED_INTEGER:
			glClearTexImage(m_colorsAttachments[bufferId], 0, GL_RED_INTEGER, GL_INT, data);
			break;
		}
	}

	int OpenGLFrameBuffer::getTextureBufferPixel(int id, unsigned int x, unsigned int y) {
		int pixel = -2;
		glReadBuffer(GL_COLOR_ATTACHMENT0 + id);
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);

		return pixel;
	}

	void OpenGLFrameBuffer::invalidate() {
		if (m_frameBuffer != 0) {
			glDeleteFramebuffers(1, &m_frameBuffer);
			glDeleteRenderbuffers(1, &m_depthAttachment);
			for (auto id : m_colorsAttachments)
				glDeleteTextures(1, &id);

			m_colorsAttachments.clear();
		}

		glGenFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

		std::vector<GLenum> attachments;

		int id = 0;
		for (auto colorType : m_specification.frameBufferTextures) {
			switch (colorType)
			{
			case TextureBufferType::RGBA8:
			{
				unsigned int colorAttachment;
				// Color texture
				glGenTextures(1, &colorAttachment);
				glBindTexture(GL_TEXTURE_2D, colorAttachment);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				// Join
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, colorAttachment, 0);

				m_colorsAttachments.push_back(colorAttachment);
			}
			break;
			case TextureBufferType::RED_INTEGER:
			{
				unsigned int colorAttachment;
				// Color texture
				glGenTextures(1, &colorAttachment);
				glBindTexture(GL_TEXTURE_2D, colorAttachment);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_specification.width, m_specification.height, 0, GL_RED_INTEGER, GL_INT, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				// Join
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, colorAttachment, 0);

				m_colorsAttachments.push_back(colorAttachment);
			}
			break;
			}

			attachments.push_back(GL_COLOR_ATTACHMENT0 + id);
			id++;
		}

		glDrawBuffers(attachments.size(), &attachments[0]);

		// Depth buffer
		glGenRenderbuffers(1, &m_depthAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthAttachment);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height);

		// Join
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthAttachment);

		// Check
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			DEER_CORE_ERROR("frame buffer is incomplete!");
		}
	}
}
