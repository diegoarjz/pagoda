#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>
#include <memory>

#include <pgeditor/rendering/gl_debug.h>
#include <pgeditor/rendering/renderable_object.h>
#include <pgeditor/rendering/renderer.h>
#include <pgeditor/rendering/shader.h>
#include <pgeditor/rendering/shader_program.h>
#include <pgeditor/rendering/vertex.h>
#include <pgeditor/window/window.h>
#include <pgeditor/window/window_creation_params.h>

#include <pgeditor/scene/camera.h>
#include <pgeditor/scene/camera_manager.h>
#include <pgeditor/scene/camera_rig.h>
#include <pgeditor/scene/geometry.h>
#include <pgeditor/scene/mesh.h>
#include <pgeditor/scene/node.h>

#include <pgeditor/input/input_manager.h>
#include <pgeditor/input/keys.h>

#include <pagoda/math/degrees.h>
#include <pagoda/math/math_utils.h>

#include <pagoda/geometry/algorithms/triangulate.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>
#include <pagoda/graph/graph.h>

#include <pagoda/pagoda.h>

#include <boost/program_options.hpp>

#include <GL/glew.h>

#include <iostream>

using namespace boost;
namespace po = boost::program_options;

using namespace pagoda;
using namespace pagoda::common;
using namespace pagoda::common::debug;
using namespace pagoda::common::instrument;
using namespace pagoda::objects;
using namespace pagoda::geometry;
using namespace pagoda::geometry::algorithms;
using namespace pagoda::graph;
using namespace pagoda::math;

using namespace pgeditor;
using namespace pgeditor::input;
using namespace pgeditor::scene;
using namespace pgeditor::rendering;
using namespace pgeditor::window;

bool ParseCommandLine(int argc, char* argv[], po::variables_map* out_vm);
void PrintProfile();

const char* vertexShader =
    "#version 410\n"
    "layout(location = 0) in vec3 vertexPosition_modelSpace;\n"
    "layout(location = 1) in vec3 vertexNormal_modelSpace;\n"
    "layout(location = 2) in vec2 texCoord;\n"
    "uniform mat4 modelMatrix;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 projection;\n"
    "out vec3 vertexNormal;\n"
    "out vec2 outTexCoord;\n"
    "void main(){\n"
    "   gl_Position = projection * viewMatrix * modelMatrix * vec4(vertexPosition_modelSpace.xyz, 1.0);\n"
    "   vertexNormal = vertexNormal_modelSpace;\n"
    "   outTexCoord = texCoord;\n"
    "}";

const char* fragmentShader = "#version 410\n"
                             "in vec3 vertexNormal;\n"
                             "in vec2 outTexCoord;\n"
                             "out vec3 color;\n"
                             "void main() {\n"
                             "  color = vec3(outTexCoord.xy, 0);\n"
                             "}";

int main(int argc, char* argv[])
{
	po::variables_map vm;

	if (!ParseCommandLine(argc, argv, &vm))
	{
		return 0;
	}

	std::string graphFile;
	try
	{
		if (vm.count("file"))
		{
			graphFile = vm["file"].as<std::string>();
		}
	}
	catch (po::error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	if (graphFile.size() == 0)
	{
		std::cerr << "Empty file path" << std::endl;
		return 1;
	}

	Pagoda pagoda;
	ProceduralObjectSystemPtr objectSystem = pagoda.GetProceduralObjectSystem();
	GeometrySystemPtr geometrySystem = objectSystem->GetComponentSystem<GeometrySystem>();
	GraphPtr graph = pagoda.CreateGraphFromFile(graphFile);
	graph->Execute();

	auto rootNode = std::make_shared<pgeditor::scene::Node>();

	InputManager inputManager;
	CameraManager cameraManager;

	auto cameraNode = std::make_shared<pgeditor::scene::Node>();
	auto camera = cameraManager.CreateCamera();
	cameraNode->Attach(camera);
	cameraNode->SetParent(rootNode);
	cameraNode->Translate(0, 0, 30);
	cameraNode->Yaw(pagoda::math::Degrees<float>(180.0));

	auto shaderProgram = std::make_shared<ShaderProgram>();
	auto vShader = std::make_shared<Shader>(Shader::ShaderType::Vertex);
	vShader->SetShaderSource(vertexShader);
	auto fShader = std::make_shared<Shader>(Shader::ShaderType::Fragment);
	fShader->SetShaderSource(fragmentShader);
	shaderProgram->SetVertexShader(vShader);
	shaderProgram->SetFragmentShader(fShader);
	auto viewMatrix =
	    shaderProgram->CreateUniform("viewMatrix", Uniform::Type::Matrix4, Uniform::Semantics::ViewMatrix);
	auto proj =
	    shaderProgram->CreateUniform("projection", Uniform::Type::Matrix4, Uniform::Semantics::ProjectionMatrix);
	auto modelMatrix =
	    shaderProgram->CreateUniform("modelMatrix", Uniform::Type::Matrix4, Uniform::Semantics::ModelMatrix);

	Triangulate<pagoda::geometry::Geometry> t;
	std::shared_ptr<pgeditor::scene::Node> n;

	auto geomNode = std::make_shared<scene::Node>();
	geomNode->SetParent(rootNode);

	for (const auto& p : objectSystem->GetProceduralObjects())
	{
		auto geometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(p);
		auto geometry = geometryComponent->GetGeometry();
		auto triangulatedGeometry = std::make_shared<pagoda::geometry::Geometry>();
		t.Execute(geometry, triangulatedGeometry);

		std::vector<Vertex> verts;
		std::vector<uint32_t> indices;

		for (auto fIter = triangulatedGeometry->FacesBegin(); fIter != triangulatedGeometry->FacesEnd(); ++fIter)
		{
			pagoda::geometry::Geometry::FaceAttributes fAttr = triangulatedGeometry->GetFaceAttributes(*fIter);
			qvm::vec<float, 3> normal = fAttr.m_normal;
			for (auto pCirc = triangulatedGeometry->FacePointCirculatorBegin(*fIter); pCirc.IsValid(); ++pCirc)
			{
				pagoda::geometry::Geometry::PositionType pos = triangulatedGeometry->GetPosition(*pCirc);
				qvm::vec<float, 2> texCoord = triangulatedGeometry->GetVertexAttributes(*pCirc).m_texCoords;
				// clang-format off
				Vertex v{
                    {X(pos), Y(pos), Z(pos)},
                    {X(normal), Y(normal), Z(normal)},
                    {X(texCoord), Y(texCoord)}
                };
				// clang-format on
				verts.push_back(v);
				indices.push_back(indices.size());
			}
		}

		auto renderableGeometry = std::make_shared<pgeditor::scene::Geometry>(verts, indices);
		auto objectNode = std::make_shared<pgeditor::scene::Node>();
		objectNode->SetParent(geomNode);
		auto mesh = std::make_shared<Mesh>(renderableGeometry, shaderProgram);
		objectNode->Attach(mesh);

		n = objectNode;
	}

	WindowCreationParams wcp;
	auto w = std::make_shared<Window>();
	w->Create(wcp);

	// CameraRig rig(geomNode);
	CameraRig rig(cameraNode);

	// clang-format off
    float rot = 30.5f;
	inputManager.AddKeyMapping({0, Key::W, InputManager::Action::WhilePressed, std::bind(&CameraRig::MoveForward, &rig)});
	inputManager.AddKeyMapping({0, Key::S, InputManager::Action::WhilePressed, std::bind(&CameraRig::MoveBackward, &rig)});
	inputManager.AddKeyMapping({0, Key::A, InputManager::Action::WhilePressed, std::bind(&CameraRig::MoveLeft, &rig)});
	inputManager.AddKeyMapping({0, Key::D, InputManager::Action::WhilePressed, std::bind(&CameraRig::MoveRight, &rig)});
	inputManager.AddKeyMapping({0, Key::Q, InputManager::Action::WhilePressed, std::bind(&CameraRig::MoveUp, &rig)});
	inputManager.AddKeyMapping({0, Key::E, InputManager::Action::WhilePressed, std::bind(&CameraRig::MoveDown, &rig)});
	inputManager.AddKeyMapping({0, Key::J, InputManager::Action::WhilePressed, [&rig, rot](){ rig.Rotate(-rot,  0.0); }});
	inputManager.AddKeyMapping({0, Key::K, InputManager::Action::WhilePressed, [&rig, rot](){ rig.Rotate( rot,  0.0); }});
	inputManager.AddKeyMapping({0, Key::H, InputManager::Action::WhilePressed, [&rig, rot](){ rig.Rotate( 0.0, -rot); }});
	inputManager.AddKeyMapping({0, Key::L, InputManager::Action::WhilePressed, [&rig, rot](){ rig.Rotate( 0.0,  rot); }});


	inputManager.AddKeyMapping({0, Key::C, InputManager::Action::OnPress, [&cameraNode, &camera](){
            std::cout << "  Cam Node Pos: " << cameraNode->GetPosition() << std::endl;
            std::cout << "Cam Node Front: " << cameraNode->GetWorldTransformation().GetFrontDirection() << std::endl;
            std::cout << "Cam Node Right: " << cameraNode->GetWorldTransformation().GetRightDirection() << std::endl;
            std::cout << "   Cam Node Up: " << cameraNode->GetWorldTransformation().GetUpDirection() << std::endl;
            std::cout << "       Cam Pos: " << camera->GetPosition() << std::endl;
            std::cout << "      Cam View: " << camera->GetViewDirection() << std::endl;
            std::cout << "     Cam Right: " << camera->GetRightVector() << std::endl;
            std::cout << "        Cam Up: " << camera->GetUpVector() << std::endl;
    }});
	// clang-format on

	w->RegisterOnKeyPressed(
	    std::bind(&InputManager::OnKeyPressed, &inputManager, std::placeholders::_1, std::placeholders::_2));
	w->RegisterOnKeyReleased(
	    std::bind(&InputManager::OnKeyReleased, &inputManager, std::placeholders::_1, std::placeholders::_2));
	w->RegisterOnKeyRepeat(
	    std::bind(&InputManager::OnKeyRepeat, &inputManager, std::placeholders::_1, std::placeholders::_2));
	w->RegisterOnMouseMoved(std::bind(&InputManager::OnMouseMoved, &inputManager, std::placeholders::_1,
	                                  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	w->RegisterOnDrag(std::bind(&InputManager::OnMouseDrag, &inputManager, std::placeholders::_1, std::placeholders::_2,
	                            std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

	inputManager.AddMouseDragHandler(
	    {0, MouseButton::Left, [&rig](const MouseButton& b, int x, int y, int dx, int dy) { rig.Rotate(dy, -dx); }});

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	auto r = std::make_shared<Renderer>();
	r->SetCamera(camera);

	glEnable(GL_DEPTH_TEST);

	double previousTime = w->GetEllapsedTime();
	double currentTime;
	double ellapsedTime;
	while (!w->GetWindowShouldClose())
	{
		currentTime = w->GetEllapsedTime();
		ellapsedTime = currentTime - previousTime;
		previousTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		inputManager.Update(ellapsedTime);
		rig.Update(ellapsedTime);
		cameraManager.Update(ellapsedTime);

		r->RenderScene(rootNode);

		w->PollEvents();
		w->SwapBuffers();
	}

	w->Destroy();

	if (vm.count("show-profile"))
	{
		PrintProfile();
	}

	return 0;
}

void PrintProfile()
{
	ConsoleProfilerLogger consoleLogger(ProfilerManager::Instance());
	consoleLogger.Log(20);
}

bool ParseCommandLine(int argc, char* argv[], po::variables_map* out_vm)
{
	try
	{
		po::positional_options_description positionalOptions;
		positionalOptions.add("file", 1);

		po::options_description desc("Options");
		// clang-format off
        desc.add_options()
            ("help", "Print help message.")
            ("file", po::value<std::string>(), "Input Graph specification file.")
            ("show-profile", "Prints profiling information");
		// clang-format on

		po::store(po::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run(), *out_vm);
		po::notify(*out_vm);

		if (out_vm->count("help"))
		{
			std::cout << "Pagoda Editor" << std::endl << desc << std::endl;
			return false;
		}
	}
	catch (po::error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}

	return true;
}
