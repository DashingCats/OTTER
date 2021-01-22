/*
Week 2 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "NOU/GLTFLoader.h"

#include "Logging.h"

#include <memory>

using namespace nou;

//TODO: Write templated LERP function.
template<typename T>
T LERP(const T& p0, const T& p1, float t)
{
	return (1.0f - t)* p0 + t * p1;
}

int main() 
{
	App::Init("Week 1 Tutorial - LERP", 800, 800);
	App::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	auto prog_texLit = ShaderProgram({ v_texLit.get(), f_texLit.get() });

	//TODO: Load in duck model and texture.
	Mesh duckMesh;
	GLTF::LoadMesh("duck/Duck.gltf", duckMesh);
	Texture2D duckTex = Texture2D("duck/DuckCM.png");

	Mesh axeMesh;
	GLTF::LoadMesh("axe/axe_model.gltf", axeMesh);
	Texture2D axeTex = Texture2D("axe/axe_diffuse.png");


	Material duckMat(prog_texLit);
	duckMat.AddTexture("albedo", duckTex);

	Material axeMat(prog_texLit);
	axeMat.AddTexture("Albedo", axeTex);


	//TODO: Set up camera entity and transform.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::uvec3(0.0f, 0.0f, 4.0f);


	//TODO: Create our duck entity.
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, duckMesh, duckMat);
	duckEntity.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	Entity axeEntity = Entity::Create();
	axeEntity.Add<CMeshRenderer>(axeEntity, axeMesh, axeMat);
	axeEntity.transform.m_scale = glm::vec3(0.2f, 0.2f, 0.2f);
	axeEntity.transform.m_pos = glm::vec3(-1.0f, -1.0f, 0.0f);

	

	//Variables for lerping position
	glm::vec3 origPos = glm::vec3(0.0f, -1.0f, 0.0f), newPos = glm::vec3(0.0f);
	float posTimer = 0.0f, posTimeLimit = 2.0f;
	bool duckyGoPlaces = true;

	glm::vec3 origPosaxe = glm::vec3(-1.0f, 0.0f, 0.0f), newPosaxe = glm::vec3(0.0f);
	float axeposTimer = 0.0f, axeposTimeLimit = 1.0f;
	bool axeswing = true;

	//variable for lerping
	glm::vec3 origCol = glm::vec3(1.0f, 1.0f, 1.0f), newCol1 = glm::vec3(1.0f, 0.0f, 0.0f);
	float colTimer = 0.0f, colTimeLimit = 5.0f;
	bool duckyGoColor = true;



	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//TODO: LERP stuff!
		posTimer += deltaTime;
		colTimer += deltaTime;

		axeposTimer += deltaTime;

		if (posTimer >= posTimeLimit)
		{
			posTimer = 0.0f;

			duckyGoPlaces = !duckyGoPlaces;
		}

		if (colTimer >= colTimeLimit)
		{
			colTimer = 0.0f;
			duckyGoColor = !duckyGoColor;
		}


		if (axeposTimer >= axeposTimeLimit)
		{
			axeposTimer = 0.0f;
			axeswing = !axeswing;
		}

		
		//Calculate t
		float tPos = posTimer / posTimeLimit;
		float tCol = colTimer / colTimeLimit;
		float axetPos = axeposTimer / axeposTimeLimit;

		if (duckyGoPlaces == true)
		{
			duckEntity.transform.m_pos = LERP(origPos, newPos, tPos);
		}
		else if (duckyGoPlaces == false)
		{
			duckEntity.transform.m_pos = LERP(newPos, origPos, tPos);
		}
		
		if (duckyGoColor == true)
		{
			duckMat.m_color = LERP(origCol, newCol1, tCol);
		}
		else if (duckyGoColor == false)
		{
			duckMat.m_color = LERP(newCol1, origCol, tCol);
		}

		if (axeswing == true)
		{
			axeEntity.transform.m_pos = LERP(origPosaxe, newPosaxe, axetPos);
		}
		else if (axeswing == false)
		{
			axeEntity.transform.m_pos = LERP(newPosaxe, origPosaxe, axetPos);
		}
		

		//TODO: Update camera, update duck transform, draw duck...
		camEntity.Get<CCamera>().Update();

		duckEntity.transform.RecomputeGlobal();
		duckEntity.Get<CMeshRenderer>().Draw();

		axeEntity.transform.RecomputeGlobal();
		axeEntity.Get<CMeshRenderer>().Draw();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
