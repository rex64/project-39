﻿#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>

#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Scene/SplinePath.h>

#include "Urho3D/IO/Log.h"

#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/AudioEvents.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>

#include "Game.h"

using namespace Urho3D;

URHO3D_DEFINE_APPLICATION_MAIN(Game)

Game::Game(Context* context) : Application(context) {}

void Game::Setup() {

	engineParameters_["FullScreen"] = false;
	engineParameters_["WindowWidth"] = 1280;
	engineParameters_["WindowHeight"] = 720;

}

void Game::Start() {

	timer = 0.0f;

	Renderer* renderer = GetSubsystem<Renderer>();

	Stuff3d();
	Stuff2d();

	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Game, HandleUpdate));

	AddTarget(
		Vector3(4.f, 4.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		2.0f
	);

	AddTarget(
		Vector3(-4.f, 4.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		2.5f
	);

	AddTarget(
		Vector3(4.f, -4.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		3.0f
	);

	AddTarget(
		Vector3(4.f, -4.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		3.5f
	);

	Graphics* graphics = GetSubsystem<Graphics>();

	float halfWidth = graphics->GetWidth() * 0.5f * PIXEL_SIZE;
	float halfHeight = graphics->GetHeight() * 0.5f * PIXEL_SIZE;

	addStaticSprite(Vector3(halfWidth, halfHeight, 0.0f));
	addStaticSprite(Vector3(halfWidth, -halfHeight, 0.0f));
	addStaticSprite(Vector3(-halfWidth, -halfHeight, 0.0f));
	addStaticSprite(Vector3(-halfWidth, halfHeight, 0.0f));

	addStaticSprite(Vector3(halfWidth, 0.0f, 0.0f));
	addStaticSprite(Vector3(0.0f, -halfHeight, 0.0f));
	addStaticSprite(Vector3(-halfWidth, 0.0f, 0.0f));
	addStaticSprite(Vector3(0.0f, halfHeight, 0.0f));

	PlayMusic();

}

void Game::Stuff2d() {

	ResourceCache* cache = GetSubsystem<ResourceCache>();
	scene2d_ = new Scene(context_);
	scene2d_->CreateComponent<Octree>();


	Node *cameraNode_2 = scene2d_->CreateChild("Camera2D");
	Camera* camera2d = cameraNode_2->CreateComponent<Camera>();
	camera2d->SetOrthographic(true);
	cameraNode_2->SetPosition(Vector3(0.0f, 0.0f, -10.0f));


	Graphics* graphics = GetSubsystem<Graphics>();
	//camera2d->SetOrthoSize((float)graphics->GetHeight() * PIXEL_SIZE);

	float pixelSize = PIXEL_SIZE;
	int h = graphics->GetHeight();

	//float orthoSize =  * pixelSize;

	camera2d->SetOrthoSize((float)h * pixelSize);

	float halfWidth = graphics->GetWidth() * 0.5f * pixelSize;
	float halfHeight = graphics->GetHeight() * 0.5f * pixelSize;

	Renderer* renderer = GetSubsystem<Renderer>();

	SharedPtr<RenderPath> overlayRenderPath = SharedPtr<RenderPath>(new RenderPath());
	overlayRenderPath->Load(cache->GetResource<XMLFile>("RenderPaths/ForwardOverlay.xml"));


	SharedPtr<Viewport> viewport(new Viewport(context_, scene2d_, cameraNode_2->GetComponent<Camera>()));
	viewport->SetRenderPath(overlayRenderPath);
	renderer->SetViewport(1, viewport);

	// Construct new Text object
	SharedPtr<Text> watermarkText(new Text(context_));
	watermarkText->SetText("Project-39 alpha 0.1");
	watermarkText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
	watermarkText->SetColor(Color(0.0f, 1.0f, 0.0f));
	watermarkText->SetHorizontalAlignment(HA_RIGHT);
	watermarkText->SetVerticalAlignment(VA_BOTTOM);
	GetSubsystem<UI>()->GetRoot()->AddChild(watermarkText);


	// Construct new Text object
	timerText = SharedPtr<Text>(new Text(context_));
	timerText->SetText("Project-39 alpha 0.1");
	timerText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
	timerText->SetColor(Color(0.0f, 1.0f, 0.0f));
	timerText->SetHorizontalAlignment(HA_LEFT);
	timerText->SetVerticalAlignment(VA_TOP);
	GetSubsystem<UI>()->GetRoot()->AddChild(timerText);

	// Construct new Text object
	playTimeText = SharedPtr<Text>(new Text(context_));
	playTimeText->SetText("Project-39 alpha 0.1");
	playTimeText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
	playTimeText->SetColor(Color(0.0f, 1.0f, 0.0f));
	playTimeText->SetHorizontalAlignment(HA_LEFT);
	playTimeText->SetVerticalAlignment(VA_CENTER);
	GetSubsystem<UI>()->GetRoot()->AddChild(playTimeText);





}

void Game::addStaticSprite(Vector3 pos) {

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	Sprite2D* spriteCenter = cache->GetResource<Sprite2D>("Urho2D/target2.png");
	Node* spriteNode = scene2d_->CreateChild("StaticSprite2D");
	spriteNode->SetPosition(pos);
	StaticSprite2D* staticSprite = spriteNode->CreateComponent<StaticSprite2D>();
	staticSprite->SetBlendMode(BLEND_ALPHA);
	staticSprite->SetSprite(spriteCenter);

}

void Game::Stuff3d() {

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	//// Construct new Text object
	//SharedPtr<Text> helloText(new Text(context_));

	//// Set String to display
	//helloText->SetText("Project-39 alpha 0.1");

	//// Set font and text color
	//helloText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
	//helloText->SetColor(Color(0.0f, 1.0f, 0.0f));

	//// Align Text center-screen
	//helloText->SetHorizontalAlignment(HA_RIGHT);
	//helloText->SetVerticalAlignment(VA_BOTTOM);

	//// Add Text instance to the UI root element
	//GetSubsystem<UI>()->GetRoot()->AddChild(helloText);

	Scene *scene_ = new Scene(context_);

	// Create the Octree component to the scene. This is required before adding any drawable components, or else nothing will
	// show up. The default octree volume will be from (-1000, -1000, -1000) to (1000, 1000, 1000) in world coordinates; it
	// is also legal to place objects outside the volume but their visibility can then not be checked in a hierarchically
	// optimizing manner
	scene_->CreateComponent<Octree>();

	//// Create a child scene node (at world origin) and a StaticModel component into it. Set the StaticModel to show a simple
	//// plane mesh with a "stone" material. Note that naming the scene nodes is optional. Scale the scene node larger
	//// (100 x 100 world units)
	//Node* planeNode = scene_->CreateChild("Plane");
	//planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
	//StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
	//planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
	//planeObject->SetMaterial(cache->GetResource<Material>("Materials/Grid.xml"));

	// Create a directional light to the world so that we can see something. The light scene node's orientation controls the
	// light direction; we will use the SetDirection() function which calculates the orientation from a forward direction vector.
	// The light will use default settings (white light, no shadows)
	Node* lightNode = scene_->CreateChild("DirectionalLight");
	lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f)); // The direction vector does not need to be normalized
	Light* light = lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);

	// Create a scene node for the camera, which we will move around
	// The camera will use default settings (1000 far clip distance, 45 degrees FOV, set aspect ratio automatically)
	Node *cameraNode_ = scene_->CreateChild("Camera");
	cameraNode_->CreateComponent<Camera>();

	// Set an initial position for the camera scene node above the plane
	cameraNode_->SetPosition(Vector3(0.0f, 5.0f, 0.0f));

	Renderer* renderer = GetSubsystem<Renderer>();

	// Set up a viewport to the Renderer subsystem so that the 3D scene can be seen. We need to define the scene and the camera
	// at minimum. Additionally we could configure the viewport screen size and the rendering path (eg. forward / deferred) to
	// use, but now we just use full screen and default render path configured in the engine command line options
	SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
	renderer->SetViewport(0, viewport);

	//// Let's put some sky in there.
	//// Again, if the engine can't find these resources you need to check
	//// the "ResourcePrefixPath". These files come with Urho3D.
	//Node* skyNode = scene_->CreateChild("Sky");
	//skyNode->SetScale(500.0f); // The scale actually does not matter
	//Skybox* skybox = skyNode->CreateComponent<Skybox>();
	//skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	//skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));

	//// Let's put a box in there.
	//Node *boxNode_ = scene_->CreateChild("Box");
	//boxNode_->SetPosition(Vector3(0, 2, 15));
	//boxNode_->SetScale(Vector3(3, 3, 3));
	//StaticModel* boxObject = boxNode_->CreateComponent<StaticModel>();
	//boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	//boxObject->SetMaterial(cache->GetResource<Material>("Materials/Grid.xml"));
	//boxObject->SetCastShadows(true);

}

void Game::Stop() {

}

void Game::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	float timeStep = eventData["TimeStep"].GetFloat();
	float playTime = soundSource->GetTimePosition();

	timer = timer + timeStep;

	for (unsigned i = 0; i < paths.Size(); ++i) {

		SplinePath *path = paths[i]->GetComponent<SplinePath>();
		
		float omar = paths[i]->GetVar("offset").GetFloat();

		if (omar <= timer) {
			if (path->IsFinished()) {
				//
				URHO3D_LOGINFO(String("[gameTimer] reached: ") + String(timer));
				URHO3D_LOGINFO(String("[playTime] reached: ") + String(playTime));
				//path->Reset();
			}
			else {
				path->Move(timeStep);
			}
		}

		

	}

	timerText->SetText(String(timer));
	playTimeText->SetText(String(playTime));


	Input* input = GetSubsystem<Input>();


	if (input->GetKeyPress('W')) {
		URHO3D_LOGINFO("w pressed");
	}
}

void Game::PlayMusic() {

	// Get the sound resource
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Sound* sound = cache->GetResource<Sound>("Music/UntitledSong.wav");

	// Create a scene node with a SoundSource component for playing the sound. The SoundSource component plays
	// non-positional audio, so its 3D position in the scene does not matter. For positional sounds the
	// SoundSource3D component would be used instead
	Node* soundNode = scene2d_->CreateChild("Sound");
	soundSource = soundNode->CreateComponent<SoundSource>();
	soundSource->Play(sound);
	// In case we also play music, set the sound volume below maximum so that we don't clip the output
	soundSource->SetGain(0.75f);

	// Subscribe to the "sound finished" event generated by the SoundSource for removing the node once the sound has played
	// Note: the event is sent through the Node (similar to e.g. node physics collision and animation trigger events)
	// to not require subscribing to the particular component
	//SubscribeToEvent(soundNode, E_SOUNDFINISHED, URHO3D_HANDLER(SoundEffects, HandleSoundFinished));


}

void Game::AddTarget(Vector3 startPos, Vector3 endPos, float timeOffset) {

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	//ResourceCache* cache = GetSubsystem<ResourceCache>();
	// Get sprite
	Sprite2D* sprite = cache->GetResource<Sprite2D>("Urho2D/target.png");

	/*float halfWidth = graphics->GetWidth() * 0.5f * PIXEL_SIZE;
	float halfHeight = graphics->GetHeight() * 0.5f * PIXEL_SIZE;*/

	Node* spriteNode = scene2d_->CreateChild("StaticSprite2D");
	spriteNode->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	StaticSprite2D* staticSprite = spriteNode->CreateComponent<StaticSprite2D>();
	// Set blend mode
	staticSprite->SetBlendMode(BLEND_ALPHA);
	// Set sprite
	staticSprite->SetSprite(sprite);

	// Initial point
	Node* startNode = scene2d_->CreateChild("Start");
	startNode->SetPosition(startPos);

	// Target point
	Node* targetNode = scene2d_->CreateChild("Target");
	targetNode->SetPosition(endPos);

	// Node to move along the path ('controlled node')
	Node* movingNode = scene2d_->CreateChild("MovingNode");

	// Spline path
	Node* pathNode = scene2d_->CreateChild("PathNode");

	SharedPtr<SplinePath> path(pathNode->CreateComponent<SplinePath>());
	path->AddControlPoint(startNode, 0);
	path->AddControlPoint(targetNode, 1);
	path->SetInterpolationMode(LINEAR_CURVE);
	path->SetSpeed(4.5f);
	path->SetControlledNode(spriteNode);

	float timeToReach = path->GetLength() / path->GetSpeed();
	URHO3D_LOGINFO(String("time to reach: ") + String(timeToReach));
	pathNode->SetVar("offset", timeOffset - timeToReach);

	paths.Push(SharedPtr<Node>(pathNode));
}