#pragma once

#include <Urho3D/Engine/Application.h>

using namespace Urho3D;

class Game : public Application
{
    // Enable type information.
    URHO3D_OBJECT(Game, Application);

public:
    /// Construct.
    Game(Context* context);

    /// Setup before engine initialization. Modifies the engine parameters.
    virtual void Setup();
    /// Setup after engine initialization. Creates the logo, console & debug HUD.
    virtual void Start();
    /// Cleanup after the main loop. Called by Application.
    virtual void Stop();
	virtual void HandleUpdate(StringHash eventType, VariantMap& eventData);

	void Stuff2d();
	void Stuff3d();

	void AddTarget(Vector3 startPos, Vector3 endPos, float timeOffset);
	void addStaticSprite(Vector3 pos);
	void PlayMusic();

	Scene *scene2d_;

	Vector<SharedPtr<Node> > paths;
	SharedPtr<Text> timerText;

	float timer;

	SoundSource* soundSource;
	SharedPtr<Text> playTimeText;

};
