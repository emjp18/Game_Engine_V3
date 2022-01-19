#include "stdafx.h"
#include "CBfr.h"

void CBfr::createBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften, UINT arraySize, bool useSRV, bool useUAV, bool stage)
{
	m_cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	if (updateOften)
	{
		m_cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	m_cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_cBufferDesc.MiscFlags = 0;
	m_cBufferDesc.ByteWidth = byteStride;
	m_cBufferDesc.StructureByteStride = 0;


	m_cBufferSub.pSysMem = data;

	m_hr = device->CreateBuffer(&m_cBufferDesc, &m_cBufferSub, m_bfr.GetAddressOf());
	assert(SUCCEEDED(m_hr));
}


void CBfr::updateBfr(ID3D11DeviceContext* context, void* data)
{
	context->UpdateSubresource(m_bfr.Get(), 0, 0, data, 0, 0);
}

void CBfr::mapBfr(ID3D11DeviceContext* context, void* data, size_t size)
{
	context->Map(m_bfr.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapped);
	memcpy(m_mapped.pData, data, size);
	context->Unmap(m_bfr.Get(), 0);

	
}

void CBfr::reCreateBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften, UINT arraySize, bool useSRV, bool useUAV, bool stage)
{
	if (m_bfr)
		m_bfr->Release();
	createBfr(device, byteStride, data, updateOften, arraySize, useSRV, useUAV, stage);
}
