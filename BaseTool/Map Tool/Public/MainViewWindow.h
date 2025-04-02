#pragma once
#include "BasicWindow.h"

class MainViewWindow : public BasicWindow
{
private:
	MainViewWindow(void) = delete;
	MainViewWindow(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual ~MainViewWindow(void) = default;
	virtual void Free(void) override;
public:
    static MainViewWindow* Create(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11ShaderResourceView* shaderResourceView, ID3D11RenderTargetView* mainRenderTargetView);
	
	HRESULT Start(void);
	void Update(void);
	void Render(void);
	virtual HRESULT CreateFrame(void) override;



	//vec2 CalculatePercentageDifference(vec2 vec1, vec2 vec2)
	//{
	//	float xDiff = std::abs(vec1.x - vec2.x);
	//	float yDiff = std::abs(vec1.y - vec2.y);
	//	float xPercent = (vec1.x != 0) ? (xDiff / std::abs(vec1.x)) : 0;
	//	float yPercent = (vec1.y != 0) ? (yDiff / std::abs(vec1.y)) : 0;
	//	return { 1.0f - xPercent, 1.0f - yPercent };
	//}

private:
	bool focused = false, mouseHovering = false;
	ImVec2 windowSize;
	ImVec4 clear_color = {};
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11ShaderResourceView* mainView = nullptr;
	ID3D11RenderTargetView* mainRenderTargetView = nullptr;

};