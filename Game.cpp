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

	Renderer* renderer = GetSubsystem<Renderer>();

	Stuff3d();
	Stuff2d();

	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Game, HandleUpdate));
}

void Game::Stuff2d() {

	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Scene *scene_ = new Scene(context_);
	scene_->CreateComponent<Octree>();


	Node *cameraNode_2 = scene_->CreateChild("Camera2D");
	Camera* camera2d = cameraNode_2->CreateComponent<Camera>();
	camera2d->SetOrthographic(true);
	cameraNode_2->SetPosition(Vector3(0.0f, 0.0f, -10.0f));


	Graphics* graphics = GetSubsystem<Graphics>();
	camera2d->SetOrthoSize((float)graphics->GetHeight() * PIXEL_SIZE);

	//ResourceCache* cache = GetSubsystem<ResourceCache>();
	// Get sprite
	Sprite2D* sprite = cache->GetResource<Sprite2D>("Urho2D/Aster.png");
	if (!sprite)
		return;

	float halfWidth = graphics->GetWidth() * 0.5f * PIXEL_SIZE;
	float halfHeight = graphics->GetHeight() * 0.5f * PIXEL_SIZE;

	SharedPtr<Node> spriteNode(scene_->CreateChild("StaticSprite2D"));
	spriteNode->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	spriteNode->SetScale(Vector3(2.0f, 1.0f, 1.0f));

	StaticSprite2D* staticSprite = spriteNode->CreateComponent<StaticSprite2D>();
	// Set random color
	staticSprite->SetColor(Color(Random(1.0f), Random(1.0f), Random(1.0f), 1.0f));
	// Set blend mode
	staticSprite->SetBlendMode(BLEND_ALPHA);
	// Set sprite
	staticSprite->SetSprite(sprite);

	{
	
		// Get animation set
		AnimationSet2D* animationSet = cache->GetResource<AnimationSet2D>("Urho2D/GoldIcon.scml");
		if (!animationSet)
			return;

		SharedPtr<Node> spriteNode(scene_->CreateChild("AnimatedSprite2D"));
		spriteNode->SetPosition(Vector3(0.0f, 0.0f, -1.0f));

		AnimatedSprite2D* animatedSprite = spriteNode->CreateComponent<AnimatedSprite2D>();
		// Set animation
		animatedSprite->SetAnimationSet(animationSet);
		animatedSprite->SetAnimation("idle");
	
	}

	Renderer* renderer = GetSubsystem<Renderer>();

	SharedPtr<RenderPath> overlayRenderPath = SharedPtr<RenderPath>(new RenderPath());
	overlayRenderPath->Load(cache->GetResource<XMLFile>("RenderPaths/ForwardOverlay.xml"));
	

	SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_2->GetComponent<Camera>()));
	viewport->SetRenderPath(overlayRenderPath);
	renderer->SetViewport(1, viewport);


}

void Game::Stuff3d() {

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	// Construct new Text object
	SharedPtr<Text> helloText(new Text(context_));

	// Set String to display
	helloText->SetText("Project-39 alpha 0.1");

	// Set font and text color
	helloText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
	helloText->SetColor(Color(0.0f, 1.0f, 0.0f));

	// Align Text center-screen
	helloText->SetHorizontalAlignment(HA_RIGHT);
	helloText->SetVerticalAlignment(VA_BOTTOM);

	// Add Text instance to the UI root element
	GetSubsystem<UI>()->GetRoot()->AddChild(helloText);

	Scene *scene_ = new Scene(context_);

	// Create the Octree component to the scene. This is required before adding any drawable components, or else nothing will
	// show up. The default octree volume will be from (-1000, -1000, -1000) to (1000, 1000, 1000) in world coordinates; it
	// is also legal to place objects outside the volume but their visibility can then not be checked in a hierarchically
	// optimizing manner
	scene_->CreateComponent<Octree>();

	// Create a child scene node (at world origin) and a StaticModel component into it. Set the StaticModel to show a simple
	// plane mesh with a "stone" material. Note that naming the scene nodes is optional. Scale the scene node larger
	// (100 x 100 world units)
	Node* planeNode = scene_->CreateChild("Plane");
	planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
	StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
	planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
	planeObject->SetMaterial(cache->GetResource<Material>("Materials/Grid.xml"));

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

	// Let's put some sky in there.
	// Again, if the engine can't find these resources you need to check
	// the "ResourcePrefixPath". These files come with Urho3D.
	Node* skyNode = scene_->CreateChild("Sky");
	skyNode->SetScale(500.0f); // The scale actually does not matter
	Skybox* skybox = skyNode->CreateComponent<Skybox>();
	skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));

	// Let's put a box in there.
	Node *boxNode_ = scene_->CreateChild("Box");
	boxNode_->SetPosition(Vector3(0, 2, 15));
	boxNode_->SetScale(Vector3(3, 3, 3));
	StaticModel* boxObject = boxNode_->CreateComponent<StaticModel>();
	boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	boxObject->SetMaterial(cache->GetResource<Material>("Materials/Grid.xml"));
	boxObject->SetCastShadows(true);

}

void Game::Stop() {

}

void Game::HandleUpdate(StringHash eventType, VariantMap& eventData)
{

}
