#include "renderer.h"

#include "bound_state.h"
#include "gl_debug.h"
#include "renderable_object.h"
#include "shader.h"
#include "shader_program.h"

#include <pgeditor/scene/attachable.h>
#include <pgeditor/scene/attachable_visitor.h>
#include <pgeditor/scene/camera.h>
#include <pgeditor/scene/geometry.h>
#include <pgeditor/scene/mesh.h>
#include <pgeditor/scene/node.h>

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <GL/glew.h>

#include <boost/qvm/map_mat_mat.hpp>
#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_mat_operations.hpp>

#include <queue>
#include <unordered_map>

using namespace pagoda::math;
using namespace pgeditor::scene;
using namespace boost;

namespace pgeditor::rendering
{
class Renderer::Impl
{
public:
	class RendererAttachableVisitor final : public pgeditor::scene::IAttachableVisitor
	{
	public:
		RendererAttachableVisitor() {}

		void Visit(scene::Camera* camera) override
		{
			START_PROFILE;
			camera->SetTransformation(m_node->GetWorldTransformation());
		}

		void Visit(scene::Mesh* mesh) override
		{
			START_PROFILE;

			RenderableObject o(mesh->GetGeometry(), mesh->GetShaderProgram());
			o.SetModelMatrix(m_node->GetWorldTransformation().GetTransformationMatrix());

			m_renderableObjects.push_back(o);
		}

		void SetNode(std::shared_ptr<Node> n) { m_node = n; }

		std::vector<RenderableObject>&& GetRenderableObjects() { return std::move(m_renderableObjects); }

	private:
		std::shared_ptr<scene::Node> m_node;

		std::vector<RenderableObject> m_renderableObjects;
	};

	void SetCamera(std::shared_ptr<Camera> camera) { m_camera = camera; }

	void RenderScene(std::shared_ptr<Node> sceneRootNode)
	{
		START_PROFILE;

		std::queue<std::shared_ptr<Node>> nodes;
		nodes.push(sceneRootNode);

		RendererAttachableVisitor v;

		while (!nodes.empty())
		{
			auto n = nodes.front();
			nodes.pop();

			v.SetNode(n);
			for (auto a : n->GetAttachables())
			{
				a->AcceptVisitor(&v);
			}

			for (auto c : n->GetChildren())
			{
				nodes.push(c);
			}
		}

		renderObjects(std::move(v.GetRenderableObjects()));
	}

private:
	void renderObjects(std::vector<RenderableObject>&& objects)
	{
		for (auto& o : objects)
		{
			renderObject(o);
		}
	}

	void renderObject(RenderableObject& object)
	{
		START_PROFILE;

		object.SetProjectionMatrix(m_camera->GetProjectionMatrix());
		object.SetViewMatrix(m_camera->GetViewMatrix());

		SetShaderProgram(object);

		std::shared_ptr<Geometry> geometry = object.GetGeometry();
		switch (geometry->GetBoundState())
		{
			case BoundState::Unbound:
				LoadGeometry(geometry);
			case BoundState::Bound:
				RenderGeometry(geometry);
				break;
		}
	}

	struct GeometryBindData
	{
		GLuint m_vertexBufferObject;
		GLuint m_vertexArrayObject;
		GLuint m_elementBufferId;
	};

	struct ShaderProgramBindData
	{
		GLuint m_programId;
		GLuint m_vertexShader;
		GLuint m_fragmentShader;
	};

	GLenum convert_shader_type(Shader::ShaderType type)
	{
		switch (type)
		{
			case Shader::ShaderType::Vertex:
				return GL_VERTEX_SHADER;
			case Shader::ShaderType::Fragment:
				return GL_FRAGMENT_SHADER;
		}
	}

	void SetShaderProgram(RenderableObject& object)
	{
		START_PROFILE;

		std::shared_ptr<ShaderProgram> shaderProgram = object.GetShaderProgram();
		switch (shaderProgram->GetBoundState())
		{
			case BoundState::Unbound:
				LoadShaderProgram(shaderProgram);
			case BoundState::Bound:
				setShaderProgram(object, shaderProgram);
		}
	}

	bool CompileShader(std::shared_ptr<Shader> shader, GLuint& shaderId)
	{
		START_PROFILE;

		if (shader == nullptr)
		{
			return false;
		}

		const char* source = shader->GetShaderSource().c_str();

		CHECK_GL_ERROR(shaderId = glCreateShader(convert_shader_type(shader->GetType())));

		LOG_DEBUG("Compiling Shader " << shaderId);
		CHECK_GL_ERROR(glShaderSource(shaderId, 1, &source, NULL));
		CHECK_GL_ERROR(glCompileShader(shaderId));

		GLint compileStatus;
		CHECK_GL_ERROR(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus));
		if (compileStatus == GL_FALSE)
		{
			GLint logLength;
			CHECK_GL_ERROR(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength));
			std::vector<char> compilationLog(logLength + 1);
			CHECK_GL_ERROR(glGetShaderInfoLog(shaderId, logLength, NULL, &compilationLog[0]));
			LOG_ERROR("Shader compilation failed:");
			LOG_ERROR(&compilationLog[0]);
		}
		LOG_DEBUG("Done compiling shader");

		return true;
	}

	void DisposeShader(GLuint programId, GLuint shaderId)
	{
		START_PROFILE;

		LOG_DEBUG("Disposing Shader " << shaderId << " of program " << programId);
		CHECK_GL_ERROR(glDetachShader(programId, shaderId));
		CHECK_GL_ERROR(glDeleteShader(shaderId));
	}

	void LoadShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram)
	{
		START_PROFILE;

		ShaderProgramBindData data;
		CHECK_GL_ERROR(data.m_programId = glCreateProgram());
		LOG_DEBUG("Loading Shader Program " << data.m_programId);

		CompileShader(shaderProgram->GetVertexShader(), data.m_vertexShader);
		CompileShader(shaderProgram->GetFragmentShader(), data.m_fragmentShader);

		CHECK_GL_ERROR(glAttachShader(data.m_programId, data.m_vertexShader));
		CHECK_GL_ERROR(glAttachShader(data.m_programId, data.m_fragmentShader));

		LOG_DEBUG("Linking shader program " << data.m_programId);
		CHECK_GL_ERROR(glLinkProgram(data.m_programId));

		GLint linkStatus;
		CHECK_GL_ERROR(glGetProgramiv(data.m_programId, GL_LINK_STATUS, &linkStatus));
		if (linkStatus == GL_FALSE)
		{
			GLint logLength;
			CHECK_GL_ERROR(glGetProgramiv(data.m_programId, GL_INFO_LOG_LENGTH, &logLength));
			if (logLength > 0)
			{
				std::vector<char> errorMessage(logLength + 1);
				CHECK_GL_ERROR(glGetProgramInfoLog(data.m_programId, logLength, NULL, &errorMessage[0]));
				LOG_ERROR("Unable to link program");
				LOG_ERROR(&errorMessage[0]);
			}
		}

		for (auto uniform : shaderProgram->GetUniforms())
		{
			const std::string& uniformName = uniform->GetName();
			GLuint uniformId;
			CHECK_GL_ERROR(uniformId = glGetUniformLocation(data.m_programId, uniformName.c_str()));
			LOG_DEBUG("Binding Uniform '" << uniformName << "' to location " << uniformId);
			uniform->Bind(uniformId);
		}

		DisposeShader(data.m_programId, data.m_vertexShader);
		DisposeShader(data.m_programId, data.m_fragmentShader);

		shaderProgram->Bind(static_cast<std::size_t>(data.m_programId));
		m_boundShaderPrograms.emplace(data.m_programId, data);
	}

	void setShaderProgram(RenderableObject& o, std::shared_ptr<ShaderProgram> shaderProgram)
	{
		START_PROFILE;

		ShaderProgramBindData bindData = m_boundShaderPrograms[shaderProgram->GetBindableId()];
		CHECK_GL_ERROR(glUseProgram(bindData.m_programId));

		for (auto uniform : shaderProgram->GetUniforms())
		{
			GLuint uniformId = uniform->GetBindableId();

			switch (uniform->GetSemantics())
			{
				case Uniform::Semantics::ModelMatrix:
					uniform->SetMatrix4(o.GetModelMatrix());
					break;
				case Uniform::Semantics::ViewMatrix:
					uniform->SetMatrix4(o.GetViewMatrix());
					break;
				case Uniform::Semantics::ProjectionMatrix:
					uniform->SetMatrix4(o.GetProjectionMatrix());
					break;
				default:
					break;
			};

			if (!uniform->IsDirty())
			{
				continue;
			}

			switch (uniform->GetType())
			{
				case Uniform::Type::Float:
					break;
				case Uniform::Type::Vec2:
					break;
				case Uniform::Type::Vec3:
				{
					CHECK_GL_ERROR(glUniform3fv(uniformId, 1, uniform->GetVec3().a));
					break;
				}
				case Uniform::Type::Vec4:
					break;
				case Uniform::Type::Integer:
					break;
				case Uniform::Type::Unsigned:
					break;
				case Uniform::Type::Matrix2:
					break;
				case Uniform::Type::Matrix3:
					break;
				case Uniform::Type::Matrix4:
				{
					auto m = uniform->GetMatrix4();
					// clang-format off
					GLfloat mat[16] = {
                        A00(m), A10(m), A20(m), A30(m),
                        A01(m), A11(m), A21(m), A31(m),
                        A02(m), A12(m), A22(m), A32(m),
                        A03(m), A13(m), A23(m), A33(m),
                    };
					// clang-format on
					CHECK_GL_ERROR(glUniformMatrix4fv(uniformId, 1, GL_FALSE, mat));
					break;
				}
			};

			uniform->SetNotDirty();
		}
	}

	void RenderGeometry(std::shared_ptr<Geometry>& o)
	{
		START_PROFILE;

		GeometryBindData bindData = m_boundObjects[o->GetBindableId()];

		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, bindData.m_vertexBufferObject));
		CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bindData.m_elementBufferId));
		CHECK_GL_ERROR(glBindVertexArray(bindData.m_vertexArrayObject));

		CHECK_GL_ERROR(glEnableVertexAttribArray(0));
		CHECK_GL_ERROR(glEnableVertexAttribArray(1));

		CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, o->IndexCount(), GL_UNSIGNED_INT, (void*)0));

		CHECK_GL_ERROR(glDisableVertexAttribArray(0));
		CHECK_GL_ERROR(glDisableVertexAttribArray(1));
	}

	void LoadGeometry(std::shared_ptr<Geometry>& o)
	{
		START_PROFILE;

		LOG_DEBUG("Loading RenderableObject");
		GeometryBindData bindData;

		// Create vertex array object
		CHECK_GL_ERROR(glGenVertexArrays(1, &bindData.m_vertexArrayObject));
		CHECK_GL_ERROR(glBindVertexArray(bindData.m_vertexArrayObject));

		// Create vertex buffer
		CHECK_GL_ERROR(glGenBuffers(1, &bindData.m_vertexBufferObject));
		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, bindData.m_vertexBufferObject));
		CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, o->VertexBufferSizeInBytes(), o->GetVertexData(), GL_STATIC_DRAW));

		// Position Attrib Array
		CHECK_GL_ERROR(glEnableVertexAttribArray(0));
		CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0));

		// Normal Attrib Array
		CHECK_GL_ERROR(glEnableVertexAttribArray(1));
		CHECK_GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		                                     reinterpret_cast<void*>(3 * sizeof(float))));

		// Create Element Buffer
		CHECK_GL_ERROR(glGenBuffers(1, &bindData.m_elementBufferId));
		CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bindData.m_elementBufferId));
		CHECK_GL_ERROR(
		    glBufferData(GL_ELEMENT_ARRAY_BUFFER, o->IndexBufferSizeInBytes(), o->GetIndexData(), GL_STATIC_DRAW));

		std::size_t objectId = GetNextRenderableId();
		LOG_DEBUG("Loaded RenderableObject with id " << objectId);
		o->Bind(objectId);
		m_boundObjects.emplace(objectId, bindData);
	}

	std::size_t GetNextRenderableId() const
	{
		static std::size_t id = 0;
		return id++;
	}

	std::unordered_map<std::size_t, GeometryBindData> m_boundObjects;
	std::unordered_map<std::size_t, ShaderProgramBindData> m_boundShaderPrograms;

	std::shared_ptr<Camera> m_camera;
};  // namespace selector

Renderer::Renderer() : m_implementation(std::make_unique<Impl>()) {}
Renderer::~Renderer() {}

void Renderer::SetCamera(std::shared_ptr<Camera> camera) { m_implementation->SetCamera(camera); }

void Renderer::RenderScene(std::shared_ptr<Node> sceneRootNode) { m_implementation->RenderScene(sceneRootNode); }

}  // namespace pgeditor::rendering
