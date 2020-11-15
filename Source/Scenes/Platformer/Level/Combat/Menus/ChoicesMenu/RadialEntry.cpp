#include "RadialEntry.h"

#include "cocos/2d/CCSprite.h"

#include "Engine/Input/ClickableTextNode.h"

using namespace cocos2d;

RadialEntry* RadialEntry::create(
	LocalizedLabel* label,
	LocalizedLabel* labelSelected,
	std::string spriteResource,
	std::string spriteSelectedResource)
{
	RadialEntry* instance = new RadialEntry(ClickableTextNode::create(label, labelSelected, spriteResource, spriteSelectedResource));

	instance->autorelease();

	return instance;
}

RadialEntry* RadialEntry::create(
	LocalizedLabel* label,
	LocalizedLabel* labelSelected,
	Node* spriteResource,
	std::string spriteSelectedResource)
{
	RadialEntry* instance = new RadialEntry(ClickableTextNode::create(label, labelSelected, spriteResource, spriteSelectedResource));

	instance->autorelease();

	return instance;
}

RadialEntry* RadialEntry::create(
	LocalizedLabel* label,
	LocalizedLabel* labelSelected, 
	Node* content,
	Node* contentSelected)
{
	RadialEntry* instance = new RadialEntry(ClickableTextNode::create(label, labelSelected, content, contentSelected));

	instance->autorelease();

	return instance;
}

RadialEntry::RadialEntry(ClickableTextNode* entry) : super()
{
	this->entry = entry;
	this->iconResource = "";
	this->allowInteractionEdits = true;

	this->addChild(this->entry);
}

RadialEntry::~RadialEntry()
{
}

std::string RadialEntry::getIconResource()
{
	return this->iconResource;
}

void RadialEntry::addIcon(std::string iconResource)
{
	this->iconResource = iconResource;

	this->entry->addChild(Sprite::create(iconResource));
}

void RadialEntry::setTextVisible(bool isVisible)
{
	this->entry->setTextVisible(isVisible);
}

void RadialEntry::setTextOffset(Vec2 offset)
{
	this->entry->setTextOffset(offset);
}

bool RadialEntry::canInteract()
{
	return this->entry->canInteract();
}

void RadialEntry::interact()
{
	this->entry->interact();
}

void RadialEntry::setContentSize(const Size& size)
{
	this->entry->setContentSize(size);
}

void RadialEntry::setMouseClickCallback(std::function<void(MouseEventArgs* args)> onMouseClick)
{
	this->entry->setMouseClickCallback(onMouseClick);
}

void RadialEntry::setMouseInCallback(std::function<void(MouseEventArgs* args)> onMouseIn)
{
	this->entry->setMouseInCallback(onMouseIn);
}

void RadialEntry::setMouseOverCallback(std::function<void(MouseEventArgs* args)> onMouseOver)
{
	this->entry->setMouseOverCallback(onMouseOver);
}

void RadialEntry::setMouseOutCallback(std::function<void(MouseEventArgs* args)> onMouseOut)
{
	this->entry->setMouseOutCallback(onMouseOut);
}

void RadialEntry::setMouseDownCallback(std::function<void(MouseEventArgs* args)> onMouseDown)
{
	this->entry->setMouseDownCallback(onMouseDown);
}

void RadialEntry::setMousePressCallback(std::function<void(MouseEventArgs* args)> mousePressEvent)
{
	this->entry->setMousePressCallback(mousePressEvent);
}

void RadialEntry::setMouseReleaseNoHitTestCallback(std::function<void(MouseEventArgs* args)> mouseReleaseNoHitTestEvent)
{
	this->entry->setMouseReleaseNoHitTestCallback(mouseReleaseNoHitTestEvent);
}

void RadialEntry::setMouseReleaseCallback(std::function<void(MouseEventArgs* args)> mouseReleaseEvent)
{
	this->entry->setMouseReleaseCallback(mouseReleaseEvent);
}

void RadialEntry::setMouseDragCallback(std::function<void(MouseEventArgs* args)> onDrag)
{
	this->entry->setMouseDragCallback(onDrag);
}

void RadialEntry::setMouseScrollCallback(std::function<void(MouseEventArgs* args)> onScroll)
{
	this->entry->setMouseScrollCallback(onScroll);
}

void RadialEntry::setMouseOverSound(std::string soundResource)
{
	this->entry->setMouseOverSound(soundResource);
}

void RadialEntry::setClickSound(std::string soundResource)
{
	this->entry->setClickSound(soundResource);
}

void RadialEntry::setAllowCollisionWhenInvisible(bool allowCollisionWhenInvisible)
{
	this->entry->setAllowCollisionWhenInvisible(allowCollisionWhenInvisible);
}

void RadialEntry::disableInteraction(uint8_t newOpacity)
{
	if (this->allowInteractionEdits)
	{
		this->entry->disableInteraction(newOpacity);
	}
}

void RadialEntry::enableInteraction(uint8_t newOpacity)
{
	if (this->allowInteractionEdits)
	{
		this->entry->enableInteraction(newOpacity);
	}
}

void RadialEntry::toggleAllowInteractionEdits(bool allowInteractionEdits)
{
	this->allowInteractionEdits = allowInteractionEdits;
}

void RadialEntry::setClickModifier(EventKeyboard::KeyCode modifier)
{
	this->entry->setClickModifier(modifier);
}

Node* RadialEntry::getContent()
{
	return this->entry->getContent();
}

Node* RadialEntry::getContentSelected()
{
	return this->entry->getContentSelected();
}

void RadialEntry::setIntersectFunction(std::function<bool(Vec2 mousePos)> intersectFunction)
{
	this->entry->setIntersectFunction(intersectFunction);
}
