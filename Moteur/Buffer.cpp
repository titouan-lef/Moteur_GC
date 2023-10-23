#include "Buffer.h"

Buffer::Buffer(Triangle t)
{
    /*Créez un buffer
    ID3D12Resource* vertexBuffer = nullptr;
    D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(t.GetTabVertices()));
    ID3D12Device* g_pd3dDevice = nullptr;
    g_pd3dDevice->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __uuidof(ID3D12Resource), (void**)&vertexBuffer);

    /*Copiez les données des sommets dans le tampon
    void* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    vertexBuffer->Map(0, &readRange, &pVertexDataBegin);
    memcpy(pVertexDataBegin, t.GetTabVertices(), sizeof(t.GetTabVertices()));
    vertexBuffer->Unmap(0, nullptr);

    /*Configurer la vue de tampon de vertex
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
    vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
    vertexBufferView.StrideInBytes = sizeof(Triangle::Vertex);
    vertexBufferView.SizeInBytes = sizeof(t.GetTabVertices());

    /*Utilisez le tampon de vertex lors du rendu*/
}
