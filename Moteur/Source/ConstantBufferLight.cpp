#include "ConstantBufferLight.h"
ConstantBufferLight::ConstantBufferLight(ConstantBufferData* constBufferData, Light light) : ConstantBuffer(constBufferData, 2), m_light(light) {
    
    //PAS BESOIN NORMALEMENT
    //// Créer un heap descripteur pour le buffer constant.
    //D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    //heapDesc.NumDescriptors = 1;
    //heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    //heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //(Engine::Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_cbvHeapDesc)));

    // Créer le buffer constant.
    const UINT bufferSize = (sizeof(Light) + 255) & ~255; // Aligné sur 256 octets
    auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
    auto var = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    (Engine::Device->CreateCommittedResource(
        &var,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_buffer)));

    // Décrire et créer une vue du buffer constant.
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = m_buffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = bufferSize; // Taille du buffer constant.
    Engine::Device->CreateConstantBufferView(&cbvDesc, m_cbvHeapDesc->GetCPUDescriptorHandleForHeapStart());
}



void ConstantBufferLight::Update(const Light& lightData) {
    // Mettre à jour les données du buffer constant.
    UINT8* destination;
    
    CD3DX12_RANGE readRange(0, 0); // Nous n'allons pas lire de ce buffer sur la CPU.
    (m_buffer->Map(0, &readRange, reinterpret_cast<void**>(&destination)));
    memcpy(destination, &lightData, sizeof(Light));
    m_buffer->Unmap(0, nullptr);
}

