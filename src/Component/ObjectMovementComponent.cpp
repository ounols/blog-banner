#include "../Manager/InputMgr.h"
#include "ObjectMovementComponent.h"
#include "Util/SafeLog.h"

using namespace CSE;

COMPONENT_CONSTRUCTOR(ObjectMovementComponent) {
}

void ObjectMovementComponent::Init() {
    m_input = InputMgr::getInstance();
}

void ObjectMovementComponent::Tick(float elapsedTime) {
//    if(!m_input->IsMouseFocus()) return;

    auto moveX = (m_input->GetX() - m_currentX) * m_speed;
    auto moveY = (m_input->GetY() - m_currentY) * m_speed;

    m_currentX += moveX;
    m_currentY += moveY;

    gameObject->GetTransform()->m_rotation = gameObject->GetTransform()->m_rotation.Multiplied(
            Quaternion::AngleAxis(vec3{0, 1, 0}, moveX)
    );
}

void ObjectMovementComponent::Exterminate() {

}
