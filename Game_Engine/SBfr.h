#pragma once
#ifndef SBFR_H
#define SBFR_H
#include "BufferHandler.h"
class SBfr :
    public BufferHandler
{
private:
    WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
    WRL::ComPtr<ID3D11UnorderedAccessView> m_uav;
public:
   
    WRL::ComPtr<ID3D11ShaderResourceView> getSRV() { return m_srv; }
    WRL::ComPtr<ID3D11UnorderedAccessView> getUAV() { return m_uav; }

    // Inherited via BufferHandler
    virtual void createBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften = true, UINT arraySize = 0, bool useSRV = true, bool useUAV = false, bool stage = false) override;
    virtual void updateBfr(ID3D11DeviceContext* context, void* data) override;
    virtual void mapBfr(ID3D11DeviceContext* context, void* data, size_t size) override;
    virtual void reCreateBfr(ID3D11Device* device, UINT byteStride, void* data, bool updateOften = true, UINT arraySize = 0, bool useSRV = true, bool useUAV = false, bool stage = false) override;
};

#endif