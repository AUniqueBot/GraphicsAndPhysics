#include <pch.h>
#include <util/util_graphics_debugging.h>



namespace GraphicsDebug {

	void GetError() {
		return;
		GLenum _errCode = glGetError();

		switch (_errCode) {
		case GL_NO_ERROR:
			LOG_INFO("OPERATION OK");
			return;
		case GL_INVALID_ENUM:
			LOG_ERROR("BAD ENUM SPECIFIED");
			break;
		case GL_INVALID_VALUE:
			LOG_ERROR("BAD VALUE");
			break;
		case GL_INVALID_OPERATION:
			LOG_ERROR("INVALD OPERATION");
			break;
		case GL_STACK_OVERFLOW:
			LOG_ERROR("OVERFLOW");
			break;
		case GL_STACK_UNDERFLOW:
			LOG_ERROR("UNDERFLOW");
			break;
		case GL_OUT_OF_MEMORY:
			LOG_ERROR("RAN OUT OF MEMORY");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			LOG_ERROR("BAD FRAMEBUFFER OPERATION");
			break;
		default:
			LOG_ERROR("ERR UNKNOWN - CODE:" << _errCode);

		}
	}
}