#pragma once
#include "CBasicWindow.h"

class CResourceWindow : public BasicWindow
{
public:
	CResourceWindow(void);
	virtual ~CResourceWindow(void);
	void Start(void);
	virtual HRESULT CreateFrame(void) override;
	static CResourceWindow* Create(void);
private:
	CProtoMgr* prototypeManager;

};