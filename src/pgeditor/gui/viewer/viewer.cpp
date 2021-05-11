#include "viewer.h"

#include <pgeditor/renderer/rendering_system.h>

#include <pagoda/common/debug/logger.h>

#include <pagoda/math/math_utils.h>

#include <boost/qvm/gen/swizzle3.hpp>
#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_mat_operations.hpp>

#include <QGuiApplication>
#include <QWheelEvent>

using namespace pagoda::math;

namespace pgeditor::gui::viewer
{
Viewer::Viewer(QWidget* parent)
  : QOpenGLWidget(parent), m_cameraFocus{0, 0, 0}, m_horizontalRotation{MathUtils<float>::degrees_to_radians(45)},
    m_verticalRotation{MathUtils<float>::degrees_to_radians(45)}, m_zoom{4}
{
}

Viewer::~Viewer()
{
	//
}

void Viewer::SetRenderingSystem(const renderer::RenderingSystemPtr& renderingSystem)
{
	m_renderingSystem = renderingSystem;
}

void Viewer::initializeGL()
{
	using namespace pgeditor::renderer::gl;
	using namespace pagoda::math;

	makeCurrent();

	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		LOG_FATAL("Unable to initialize GLEW");
	}

	GLint glVersion[2];
	GLint numExtensions;
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	const unsigned char* vendor = glGetString(GL_VENDOR);
	const unsigned char* renderer = glGetString(GL_RENDERER);
	const unsigned char* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	LOG_INFO("----------------------------------------");
	LOG_INFO("OpenGL");
	LOG_INFO("  Version: " << glVersion[0] << "." << glVersion[1]);
	LOG_INFO("  Version String: " << glGetString(GL_VERSION));
	LOG_INFO("  Vendor: " << vendor);
	LOG_INFO("  Renderer: " << renderer);
	LOG_INFO("  GLSL Version: " << glslVersion);
	LOG_INFO("  Extensions:");
	for (auto i = 0; i < numExtensions; ++i) {
		LOG_INFO("    " << glGetStringi(GL_EXTENSIONS, i));
	}
	LOG_INFO("----------------------------------------");

	glClearColor(0.16f, 0.16f, 0.16f, 1.0f);

	doneCurrent();
}

void Viewer::resizeGL(int w, int h)
{
	using namespace pagoda::math;

	if (m_renderingSystem == nullptr) {
		return;
	}

	pagoda::scene::Lens lens;
	lens.SetPerspective(MathUtils<float>::degrees_to_radians(30),
	                    static_cast<float>(this->width()) / static_cast<float>(this->height()), 0.1, 1000);
	m_renderingSystem->GetCamera()->SetLens(lens);
	updateCamera();
}

void Viewer::paintGL()
{
	if (m_renderingSystem == nullptr) {
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT);

	m_renderingSystem->Render();
}

void Viewer::mouseMoveEvent(QMouseEvent* event)
{
	if (m_renderingSystem == nullptr) {
		return;
	}

	auto pos = event->position();
	auto dx = pos.x() - m_lastMousePos.x();
	auto dy = pos.y() - m_lastMousePos.y();

	auto& cam = m_renderingSystem->GetCamera();
	auto modifiers = QGuiApplication::keyboardModifiers();
	if (modifiers & Qt::KeyboardModifier::AltModifier) {
		auto vec = cam->GetRightVector() * dx + cam->GetUpVector() * dy;
		if (mag(vec) != 0.0f) {
			m_cameraFocus += boost::qvm::normalized(vec) * 0.1f;
		}
	} else {
		m_horizontalRotation += dx * 0.1;
		m_verticalRotation =
		  std::clamp<float>(m_verticalRotation + dy * 0.1f, 0.0f, 0.9f * pagoda::math::MathUtils<float>::half_pi);
	}

	m_lastMousePos = event->position();
	updateCamera();
	repaint();
}

void Viewer::mousePressEvent(QMouseEvent* event)
{
	m_lastMousePos = event->position();
	QOpenGLWidget::mousePressEvent(event);
}

void Viewer::mouseReleaseEvent(QMouseEvent* event)
{
	m_lastMousePos = event->position();
	QOpenGLWidget::mousePressEvent(event);
}

void Viewer::wheelEvent(QWheelEvent* event)
{
	m_zoom = std::max<float>(m_zoom + event->angleDelta().y() * 0.1, 0.5f);
	updateCamera();
	repaint();
	QOpenGLWidget::wheelEvent(event);
}

void Viewer::updateCamera()
{
	if (m_renderingSystem == nullptr) {
		return;
	}

	using namespace boost::qvm;
	using namespace pagoda::math;

	// clang-format off
	auto rotation = translation_mat(m_cameraFocus) *
	                roty_mat<4, float>(m_horizontalRotation) *
                  rotx_mat<4, float>(m_verticalRotation) *
                  translation_mat(Vec3F{0, 0, -m_zoom});
	// clang-format on

	Vec4F newPos = Vec4F(col<3>(rotation));
	auto direction = m_cameraFocus - XYZ(newPos);

	auto& cam = m_renderingSystem->GetCamera();
	cam->SetPosition(XYZ(newPos));
	cam->SetViewDirection(direction);
}
}  // namespace pgeditor::gui::viewer
