#pragma once
#include "../ComponentManager/LogicComponentManager.h"

class PlayerComponent : public LogicComponent
{
	float speed = 10;
public:
	PlayerComponent(GameObject* owner);

	void Update() override;

	void SetSpeed(float speed);

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};