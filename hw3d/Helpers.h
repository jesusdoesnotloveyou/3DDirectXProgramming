#pragma once

#include "ScaldWin.h" // for HRESULT in Windows.h
#include <exception>

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::exception{};
	}
}