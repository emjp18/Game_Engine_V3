#include "stdafx.h"
#include "SBfr.h"

void SBfr::createBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften, UINT arraySize, bool useSRV, bool useUAV, bool stage)
{
	D3D11_BUFFER_DESC sBufferDesc = {};
	D3D11_SUBRESOURCE_DATA sBufferSub = {};

	sBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	sBufferDesc.ByteWidth = byteStride * arraySize;
	if (useSRV && useUAV)
	{
		sBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		sBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	}
	else if (useSRV && !useUAV)
	{
		sBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (useUAV && !useSRV)
	{
		sBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		sBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	}

	sBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (stage)
	{
		sBufferDesc.Usage = D3D11_USAGE_STAGING;
		sBufferDesc.BindFlags = 0;
		sBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}
	sBufferDesc.StructureByteStride = byteStride;
	sBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sBufferSub.pSysMem = data;

	HRESULT hr = device->CreateBuffer(&sBufferDesc, &sBufferSub, m_bfr.GetAddressOf());
	assert(SUCCEEDED(hr));
	if (useSRV && !stage)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = arraySize;
		hr = device->CreateShaderResourceView(m_bfr.Get(), &srvDesc, m_srv.GetAddressOf());

		assert(SUCCEEDED(hr));
	}
	if (useUAV && !stage)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.Flags = 0;
		uavDesc.Buffer.NumElements = arraySize;
		hr = device->CreateUnorderedAccessView(m_bfr.Get(), &uavDesc, m_uav.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
}

void SBfr::updateBfr(ID3D11DeviceContext* context, void* data)
{
	context->UpdateSubresource(m_bfr.Get(), 0, 0, data, 0, 0);
}

void SBfr::mapBfr(ID3D11DeviceContext* context, void* data, size_t size)
{
	context->Map(m_bfr.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mapped);
	memcpy(m_mapped.pData, data, size);
	context->Unmap(m_bfr.Get(), 0);
}

void SBfr::reCreateBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften, UINT arraySize, bool useSRV, bool useUAV, bool stage)
{
	if(m_bfr)
		m_bfr->Release();
	if(m_srv)
		m_srv->Release();
	if(m_uav)
		m_uav->Release();
	createBfr(device, byteStride, data, updateOften, arraySize, useSRV, useUAV, stage);
}
