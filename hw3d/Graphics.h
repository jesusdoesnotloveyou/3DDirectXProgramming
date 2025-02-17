#pragma once

#include "ScaldWin.h"
#include <d3d11.h>
#include <wrl.h>

namespace wrl = Microsoft::WRL;

class Graphics 
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;

private:
	wrl::ComPtr<ID3D11Device> pDevice;
	wrl::ComPtr<IDXGISwapChain> pSwapChain;
	wrl::ComPtr<ID3D11DeviceContext> pContext;
	wrl::ComPtr<ID3D11RenderTargetView> pRtv;
};