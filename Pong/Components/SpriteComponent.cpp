#include "SpriteComponent.h"

#include "../ComponentManager/GameObject.h"
#include "../ComponentManager/EngineComponent.h"
#include "../ComponentManager/ResourceManager.h"
#include "../Resource/TextureResource.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(GameObject* go) : GraphicComponent(go)
{
	ID = "Sprite";
}

SpriteComponent::~SpriteComponent()
{
	if (mTex != nullptr && !texName.empty())
		ResourceManager::GetPtr()->Unload(texName);
}

void SpriteComponent::Update()
{
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxVertexList* mesh = AEGfxMeshEnd();

	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	//AEGfxSetColorToMultiply(mColor.r / 255.f, mColor.g / 255.f, mColor.b / 255.f, 255.f);

	AEGfxSetColorToMultiply(1, 1, 1, 1);
	AEGfxSetColorToAdd(mColor.r / 255.f, mColor.g / 255.f, mColor.b / 255.f, 0);
	
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);

	AEGfxTextureSet(mTex, 0, 0);

	TransformComponent* trs = (TransformComponent*)owner->FindComponent("Transform");
	AEMtx33 tranf = trs->getMatrix();
	AEGfxSetTransform(tranf.m);

	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxMeshFree(mesh);
}

void SpriteComponent::SetColor(const Color& color)
{
	mColor = color;
}

void SpriteComponent::SetTexture(std::string s)
{
	if (mTex != nullptr)
		AEGfxTextureUnload(mTex);

	mTex = AEGfxTextureLoad(s.c_str());
}

void SpriteComponent::LoadFromJson(const json& data)
{
	auto componentData = data.find("componentData");
	if (componentData != data.end())
	{
		auto it = componentData->find("color");
		mColor.r = it->begin().value();
		mColor.g = (it->begin() + 1).value();
		mColor.b = (it->begin() + 2).value();

		texName = data.at("texName").get<std::string>();
		SetTexture(texName);
	}
}

json SpriteComponent::SaveToJson()
{
	json data, componentData;
	data["type"] = "Sprite";

	componentData["color"] = { mColor.r, mColor.g, mColor.b };
	componentData["texName"] = texName;

	data["componentData"] = componentData;

	return data;
}

ComponentSerializer* SpriteComponent::CreateComponent(GameObject* owner)
{
	return owner->FindComponent("Sprite");
}

