#include "WebDemoScene.h"
#include <sstream>
#include <random>

#include "Util/Render/SEnvironmentMgr.h"
#include "Component/LightComponent.h"
#include "Component/CameraComponent.h"
#include "Component/RenderComponent.h"
#include <Util/MoreComponentUtil.h>
#include <Component/PartsReplacerComponent.h>
#include <Component/RenderOnceComponent.h>
#include <Component/CustomComponent.h>
#include <Component/ObjectMovementComponent.h>

#include "Util/Loader/DAE/DAELoader.h"
#include "Util/Render/SFrameBuffer.h"
#include "Util/Render/SMaterial.h"
using namespace CSE;

void WebDemoScene::Init() {
    auto root = new SGameObject("root");

    /// Plane Prefab
    auto planePrefab = SResource::Create<SPrefab>("plane.prefab");
    const auto& planePrefabRenderComp = GetRenderComponent(*planePrefab->GetRoot());
    planePrefabRenderComp->SetMaterial(SResource::Create<SMaterial>("file:Material/Character.mat"));

    // Result Render
    SGameObject* plane = planePrefab->Clone(vec3{0, 0, 0 }, root);
    plane->GetTransform()->m_scale = vec3{ 0.5f, 0.5f, 0.5f };
    plane->GetTransform()->m_rotation = Quaternion::AngleAxis(vec3{ 0, 0, 1 }, Pi);
    const auto& currentPlaneRenderComponent = GetRenderComponent(*plane);
    currentPlaneRenderComponent->SetMaterial(SResource::Create<SMaterial>("file:Material/ForFramebufferTest.mat"));
//    const auto& script = plane->CreateComponent<CustomComponent>();
//    script->SetClassName("Stormtrooper.script");

    // Main Camera
    auto camera = new SGameObject("camera");
    const auto& cameraComp = camera->CreateComponent<CameraComponent>();
    cameraComp->SetOrtho(0.3f, -0.3f, -0.3f, 0.3f);
    cameraComp->SetCameraType(CSE::CameraComponent::ORTHO);
    cameraComp->SetProjectionMatrix();
    camera->GetTransform()->m_position = vec3{ 0, 0, 1.5f };

    // Directional Light
    SGameObject* direction = new SGameObject();
    direction->SetParent(root);
    direction->SetName("directional");
    direction->GetTransform()->m_position = vec3{ 0.f, 0.5f, 1.f };
    direction->CreateComponent<LightComponent>();
//    direction->GetComponent<LightComponent>()->SetSunrising(true);
    direction->GetComponent<LightComponent>()->SetColor(vec3{ 5.f, 0.5f, 0.5f });
    direction->GetComponent<LightComponent>()->SetLightType(LightComponent::DIRECTIONAL);
    direction->GetComponent<LightComponent>()->SetDirection(vec4{ -0.5f, 0, 0.5f, 0 });
//    direction->GetComponent<LightComponent>()->SetShadow(true);

    // Character Render Root
    m_characterRoot = new SGameObject("character root");
    // Camera for Character Framebuffer
    auto characterCameraObj = new SGameObject("character camera");
    characterCameraObj->SetParent(m_characterRoot);
    const auto& charCamera = characterCameraObj->CreateComponent<CameraComponent>();
    characterCameraObj->GetTransform()->m_position = vec3{ 0, 0, 1.f };
    charCamera->SetBackgroundSkybox();
    charCamera->SetBackgroundType(CSE::CameraBase::SKYBOX);
    charCamera->SetFrameBuffer(SResource::Create<SFrameBuffer>("File:Texture/CharacterCamera.framebuffer"));

    // Create Walking Model
    auto* model = planePrefab->Clone(vec3{ -0.4f, 0, 0 }, m_characterRoot);
    float scaleValue = 0.5f;
    model->GetTransform()->m_scale = vec3{ scaleValue, scaleValue * 1.12f, scaleValue };
//    model->GetTransform()->m_rotation = Quaternion::AngleAxis(vec3{ 0, 0, 1 }, Pi);
    m_characterRoot->GetTransform()->m_position = vec3{ -100.f, 100.f, 0 };
//    m_characterRoot->GetTransform()->m_rotation = Quaternion::AngleAxis(vec3{ 0, 0, 1 }, Pi);
    m_characterRoot->CreateComponent<ObjectMovementComponent>();
    charCamera->SetTarget(m_characterRoot);
    {
        const auto& renderComp = GetRenderComponent(*model);
        renderComp->SetMaterial(SResource::Create<SMaterial>("file:Material/ddol.mat"));
    }

//    // GL Config
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    m_upperQuaternion = Quaternion::AngleAxis(vec3{ -1, 0, 0 }, 0.2f);
    m_characterRoot->GetTransform()->m_rotation = m_upperQuaternion;
}

void WebDemoScene::Tick(float elapsedTime) {
    // ?????? ????????? ????????? ????????? ????????? ?????? ?????????????????? ?????? ???????????? ???????????? ?????????.
    // ????????? ??? ?????? ????????? ????????? ????????????! ?????? ????????? ??? ???????????? ????????? ???????????????.
    m_characterRoot->GetTransform()->m_rotation = m_characterRoot->GetTransform()->m_rotation
            .Multiplied(Quaternion::AngleAxis(vec3{0, 1, 0}, 0.002f));
}

void WebDemoScene::Destroy() {
    // ?????? ????????? ????????? ????????? ????????? ?????? ?????????????????? ????????? ?????? ??? ????????? ???????????????.
    // ????????? ??? ?????? ????????? ????????? ????????????! ?????? ????????? ??? ???????????? ????????? ???????????????.
}