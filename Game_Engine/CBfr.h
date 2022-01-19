#pragma once
#ifndef CBFR_H
#define CBFR_H
#include "BufferHandler.h"

class CBfr :
    public BufferHandler
{
private:
    CD3D11_BUFFER_DESC m_cBufferDesc = {};
    D3D11_SUBRESOURCE_DATA m_cBufferSub = {};
public:
    // Inherited via BufferHandler
    virtual void createBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften = true, UINT arraySize = 0,  bool useSRV = true, bool useUAV = false, bool stage = false) override;
    virtual void updateBfr(ID3D11DeviceContext* context, void* data) override;
    virtual void mapBfr(ID3D11DeviceContext* context, void* data, size_t size) override;
    virtual void reCreateBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften = true, UINT arraySize = 0, bool useSRV = true, bool useUAV = false, bool stage = false) override;
};

#endif