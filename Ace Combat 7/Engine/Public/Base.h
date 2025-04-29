#pragma once

class ENGINEDLL Base abstract
{
protected:
	Base(void);
	Base(const Base& other);
	virtual ~Base(void) = default;
	virtual void Free(void) {}

public:
	UINT AddReference(void) { return ++referenceCount; }
	virtual UINT Release(void);

	static void Destroy(Base* instance);
	template <typename T, typename std::enable_if<std::is_base_of<Base, T>::value, int>::type = 0>
	static void DestroyInstance(T*& pointer)
	{
		if (pointer == nullptr)
			return;

		Destroy(pointer);
		pointer = nullptr;
	}

private:

	UINT referenceCount;
};
#define REF .AddReference()
