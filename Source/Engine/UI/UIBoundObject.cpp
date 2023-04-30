#include "UIBoundObject.h"

#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/Events/ObjectEvents.h"
#include "Engine/Utils/GameUtils.h"

using namespace cocos2d;

unsigned long long UIBoundObject::TaskId = 0;

UIBoundObject* UIBoundObject::create(cocos2d::Node* referencedObject)
{
    UIBoundObject* instance = new UIBoundObject(referencedObject);

    instance->autorelease();

    return instance;
}

UIBoundObject::UIBoundObject(cocos2d::Node* referencedObject)
{
    this->referencedObject = referencedObject;
    this->originalParent = this->referencedObject == nullptr ? nullptr : dynamic_cast<SmartNode*>(this->referencedObject->getParent());
    this->eventKey = "";
}

UIBoundObject::~UIBoundObject()
{
}

void UIBoundObject::onEnter()
{
    super::onEnter();

    if (this->referencedObject != nullptr && this->referencedObject->getParent() != this)
    {
        Vec3 position = this->referencedObject->getPosition3D();
        
        GameUtils::changeParent(this->referencedObject, this, false);

        this->referencedObject->setPosition3D(position);
    }

    this->scheduleUpdateTask();
}

void UIBoundObject::initializeListeners()
{
    super::initializeListeners();

    this->addEventListenerIgnorePause(EventListenerCustom::create(ObjectEvents::EventReparentBindPrefix + std::to_string((unsigned long long)(this->referencedObject)), [=](EventCustom* eventCustom)
    {
        ReparentBindArgs* args = static_cast<ReparentBindArgs*>(eventCustom->getData());
        
        if (args != nullptr)
        {
            this->originalParent = dynamic_cast<SmartNode*>(args->newParent);
            this->scheduleUpdateTask();
        }
    }));
}

void UIBoundObject::scheduleUpdateTask()
{
    if (this->scheduleTarget != nullptr && !this->eventKey.empty())
    {
        this->scheduleTarget->unschedule(eventKey);
    }

    unsigned long long taskId = UIBoundObject::TaskId++;
    this->eventKey = "EVENT_UIBOUND_UPDATE_TASK_" + std::to_string(taskId);

   this->scheduleTarget = this->originalParent;

    if (this->scheduleTarget == nullptr)
    {
        this->scheduleTarget = this;
    }

    this->realCoords = UIBoundObject::getRealCoords(this);
    this->realScale = UIBoundObject::getRealScale(this);
    
    // Schedule the task on the original parent, that way if the original parent is disposed, update will not be called (avoiding a crash)
    this->scheduleTarget->schedule([=](float dt)
    {
        if (this->referencedObject == nullptr)
        {
            return;
        }

        this->realCoords = UIBoundObject::getRealCoords(this);
        this->realScale = UIBoundObject::getRealScale(this);
    }, this->eventKey);
}

Vec3 UIBoundObject::getRealCoords(UIBoundObject* uiBoundObject)
{
    if (uiBoundObject == nullptr || uiBoundObject->referencedObject == nullptr || uiBoundObject->originalParent == nullptr)
    {
        return Vec3::ZERO;
    }

    Vec3 thisCoords = GameUtils::getWorldCoords3D(uiBoundObject);
    Vec3 originalParentCoords = GameUtils::getWorldCoords3D(uiBoundObject->originalParent);
    Vec3 originalCoords = uiBoundObject->referencedObject->getPosition3D();
    float originalScale = GameUtils::getUniformScale(uiBoundObject->originalParent);
    Vec3 anchorOffset = Vec3(
        0.5f * uiBoundObject->originalParent->getContentSize().width * originalScale,
        0.5f * uiBoundObject->originalParent->getContentSize().height * originalScale,
        0.0f
    );
    Vec3 delta = originalParentCoords - thisCoords + anchorOffset;

    return originalCoords + delta;
}

float UIBoundObject::getRealScale(UIBoundObject* uiBoundObject)
{
    if (uiBoundObject == nullptr || uiBoundObject->referencedObject == nullptr || uiBoundObject->originalParent == nullptr)
    {
        return 1.0f;
    }

    float parentScale = GameUtils::getUniformScale(uiBoundObject->originalParent);

    return parentScale * uiBoundObject->referencedObject->getScale();
}

cocos2d::Node* UIBoundObject::getReferencedObject()
{
    return this->referencedObject;
}

cocos2d::Node* UIBoundObject::getOriginalParent()
{
    return this->originalParent;
}

void UIBoundObject::pushRealPosition()
{
    this->originalCoords = this->referencedObject->getPosition3D();
    this->originalScale = this->referencedObject->getScale();
    
    this->referencedObject->setPosition3D(this->realCoords);
    this->referencedObject->setScale(this->realScale);
}

void UIBoundObject::popRealPosition()
{
    this->referencedObject->setPosition3D(this->originalCoords);
    this->referencedObject->setScale(this->originalScale);
}

void UIBoundObject::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    if (this->referencedObject == nullptr)
    {
        return;
    }

    this->pushRealPosition();

	super::visit(renderer, parentTransform, parentFlags);

    this->popRealPosition();
}
