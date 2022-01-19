#pragma once
#ifndef BUFFERHANDLER_H
#define BUFFERHANDLER_H
#include "common.h"

class BufferHandler
{
protected:
	WRL::ComPtr<ID3D11Buffer> m_bfr;
	D3D11_MAPPED_SUBRESOURCE m_mapped = {};
	HRESULT m_hr;
public:
	virtual void createBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften = true, UINT arraySize = 0, bool useSRV = true, bool useUAV = false, bool stage = false) = 0;
	virtual void updateBfr(ID3D11DeviceContext* context, void* data) = 0;
	virtual void mapBfr(ID3D11DeviceContext* context, void* data, size_t size) = 0;
	virtual ID3D11Buffer** getBfr() { return m_bfr.GetAddressOf(); };
	virtual void reCreateBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften = true, UINT arraySize = 0, bool useSRV = true, bool useUAV = false, bool stage = false) = 0;
};

#endif