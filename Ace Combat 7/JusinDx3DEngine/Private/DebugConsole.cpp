#include "Foundation.h"
#include "DebugConsole.h"


using namespace Engine;

void DebugConsole::Free(void)
{
    if (stream == nullptr)
        return;
    fclose(stream);
    stream = nullptr;
    FreeConsole();
}

DebugConsole* DebugConsole::Create(void)
{
    DebugConsole* newInstance = new DebugConsole();
    if (FAILED(newInstance->Start()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }
    return newInstance;
}

HRESULT DebugConsole::Start(void)
{
    AllocConsole();
    freopen_s(&stream, "CONOUT$", "w", stdout);

    return S_OK;
}

void DebugConsole::SetActive(BOOL value)
{
    active = value;
    if (active)
        Free();
    else
        Start();
}

