#include "OpenGLFrameBuffer.h"
#include "Deer/Core/Log.h"
#include "Deer/Core/Application.h"

#include "glad/glad.h"

namespace Deer {
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& frameBufferSpecification)
		: m_specification(frameBufferSpecification) {
		invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		if (m_frameBuffer != 0) {
			glDeleteFramebuffers(1, &m_frameBuffer);
			glDeleteTextures(1, &m_colorAttachment);
			glDeleteRenderbuffers(1, &m_depthAttachment);
		}
	}

	void OpenGLFrameBuffer::bind() {
		glViewport(0, 0, m_specification.width, m_specification.height);
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	}

	void OpenGLFrameBuffer::unbind() {
		unsigned int width = Application::s_application->m_window->getWitdth();
		unsigned int height = Application::s_application->m_window->getHeight();
		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::clear() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLFrameBuffer::resize(unsigned int width, unsigned int height) {
		m_specification.width = width;
		m_specification.height = height;

		invalidate();
	}

	void OpenGLFrameBuffer::invalidate() {

		if (m_frameBuffer != 0) {
			glDeleteFramebuffers(1, &m_frameBuffer);
			glDeleteTextures(1, &m_colorAttachment);
			glDeleteRenderbuffers(1, &m_depthAttachment);
		}

		glGenFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

		// Color texture
		glGenTextures(1, &m_colorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_specification.width, m_specification.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Join
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

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
