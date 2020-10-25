#pragma once
class Component
{
public:
	Component(class Actor* owner);
	virtual ~Component();

	virtual void Update(float deltTime) {}

	class Actor* GetOwner() { return mOwner;  }

protected:

	class Actor* mOwner; 
};

