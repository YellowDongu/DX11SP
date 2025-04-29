#include "Foundation.h"
#include "Base.h"


UINT Release(Base*& pointer)
{
	UINT referenceCount = pointer->Release();
	if (referenceCount == 0)
		pointer = nullptr;
	return referenceCount;
}

Base::Base(void) : referenceCount(1)
{

}

Base::Base(const Base& other) : referenceCount(other.referenceCount)
{
	referenceCount++;
}

UINT Base::Release(void)
{
	referenceCount--;

	if (referenceCount == 0)
	{
		Free();
		delete this;

		return 0;
	}
	return referenceCount;
}

void Base::Destroy(Base* instance)
{
	if (!instance)
		return;
	try
	{
		instance->Free();
		delete instance;
	}
	catch (...)
	{
		return;
	}
}
