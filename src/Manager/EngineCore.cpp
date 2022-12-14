//
// Created by ounol on 2021-03-22.
//

#include "EngineCore.h"
#include "Manager/ResMgr.h"
#include "Manager/GameObjectMgr.h"
#include "Manager/OGLMgr.h"
#include "Manager/ScriptMgr.h"
#include "Manager/CameraMgr.h"
#include "Manager/LightMgr.h"
#include "Manager/RenderMgr.h"
#include "Manager/SceneMgr.h"
#include "Manager/MemoryMgr.h"
#include "Manager/InputMgr.h"
#include "Util/Loader/SCENE/SSceneLoader.h"
#include "Util/AssetsDef.h"

#include "../Scene/WebDemoScene.h"

using namespace CSE;
IMPLEMENT_SINGLETON(EngineCore);

constexpr short ENGINE_COUNT = 9;

EngineCore::EngineCore() {
}

EngineCore::~EngineCore() {

}

void EngineCore::Init(unsigned int width, unsigned int height) {
    GenerateCores();

    for (const auto& core : m_cores) {
        core->Init();
    }

    m_oglMgr->ResizeWindow(width, height);
    m_renderMgr->SetViewport();


    m_sceneMgr->SetScene(new WebDemoScene());
}

void EngineCore::Update(float elapsedTime) {
    InputMgr::getInstance()->Update();
    for (const auto& core : m_updateCores) {
        core->Update(elapsedTime);
    }
}

void EngineCore::LateUpdate(float elapsedTime) {
    m_gameObjectMgr->DestroyQueuedObject();
    InputMgr::getInstance()->LateUpdate();
}

void EngineCore::Render() const {
    for (const auto& core : m_renderCores) {
        core->Render();
    }
}

void EngineCore::Exterminate() {
    m_memoryMgr->ExterminateObjects(true);
    for (auto core : m_cores) {
        SAFE_DELETE(core);
    }
    m_cores.clear();
}

void EngineCore::GenerateCores() {
    if(m_isGenerated) return;
    m_isGenerated = true;
    m_cores = std::vector<CoreBase*>();
    m_cores.reserve(ENGINE_COUNT);

    m_resMgr = new ResMgr();
    m_gameObjectMgr = new GameObjectMgr();
    m_oglMgr = new OGLMgr();
    m_renderMgr = new RenderMgr();
    m_cameraMgr = new CameraMgr();
    m_lightMgr = new LightMgr();
    m_sceneMgr = new SceneMgr();
    m_memoryMgr = new MemoryMgr();
    m_scriptMgr = new ScriptMgr();

    m_cores.push_back(m_resMgr);
    m_cores.push_back(m_gameObjectMgr);
    m_updateCores.push_back(m_gameObjectMgr);

    m_cores.push_back(m_oglMgr);
    m_renderCores.push_back(m_oglMgr);
    m_cores.push_back(m_renderMgr);
    m_renderCores.push_back(m_renderMgr);
    m_cores.push_back(m_cameraMgr);
    m_cores.push_back(m_lightMgr);

    m_cores.push_back(m_sceneMgr);
    m_updateCores.push_back(m_sceneMgr);
    m_cores.push_back(m_memoryMgr);

    m_cores.push_back(m_scriptMgr);
}

void EngineCore::ResizeWindow(unsigned int width, unsigned int height) {
    m_oglMgr->ResizeWindow(width, height);
    m_renderMgr->SetViewport();
}

