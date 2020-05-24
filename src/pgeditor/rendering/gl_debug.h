#pragma once

#include <cstdint>

namespace pgeditor::rendering
{
void check_gl_error(const char *file, uint32_t line);

#define CHECK_GL_ERROR(CODE)                                     \
	{                                                            \
		CODE;                                                    \
		pgeditor::rendering::check_gl_error(__FILE__, __LINE__); \
	}
}  // namespace pgeditor::rendering
