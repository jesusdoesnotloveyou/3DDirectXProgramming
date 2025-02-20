#pragma once

#include "ScaldWin.h"
#include "ScaldException.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>

namespace wrl = Microsoft::WRL;

class Graphics 
{
public:
	class Exception : public ScaldException
	{
		using ScaldException::ScaldException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
	};
	class DeviceRemovedException : HrException
	{
		using HrException::HrException;
	public:
		virtual const char* GetType() const noexcept override;
	};

public:
	Graphics(HWND hWnd);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawTestTriangle();
private:
	wrl::ComPtr<ID3D11Device> pDevice;
	wrl::ComPtr<IDXGISwapChain> pSwapChain;
	wrl::ComPtr<ID3D11DeviceContext> pContext;
	wrl::ComPtr<ID3D11RenderTargetView> pRtv;
};