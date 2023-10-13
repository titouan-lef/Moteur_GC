#include "Buffer.h"

Buffer::Buffer(Triangle t)
{
    /*Créez un tampon de vertex*/
    ID3D12Resource* vertexBuffer = nullptr;

    // Propriétés du tas de mémoire dans lequel le buffer sera alloué
    D3D12_HEAP_PROPERTIES heapProperties = {};
    heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

    // Propriétés du buffer
    D3D12_RESOURCE_DESC bufferDesc = {};
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Width = t.Size();// TO DO
    bufferDesc.Height = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.MipLevels = 1;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    
    // Créez le buffer
    ID3D12Device* g_pd3dDevice = nullptr;
    g_pd3dDevice->CreateCommittedResource(
        &heapProperties,  // Propriétés du tas de mémoire
        D3D12_HEAP_FLAG_NONE,  // Drapeaux du tas de mémoire
        &bufferDesc,  // Description du tampon
        D3D12_RESOURCE_STATE_GENERIC_READ,  // État initial du tampon
        nullptr,  // Description d'allocation personnalisée (peut être nul)
        __uuidof(ID3D12Resource),
        (void**)&vertexBuffer
    );

    /*Copiez les données des sommets dans le tampon*/
    void* pVertexDataBegin = nullptr;  // Déclarez le pointeur et initialisez-le à nullptr

    // Pas besoin de spécifier une plage de lecture, vous pouvez accéder à l'ensemble du tampon
    vertexBuffer->Map(0, nullptr, &pVertexDataBegin);

    // Copiez les données des sommets dans le tampon
    memcpy(pVertexDataBegin, t.GetTabVertices(), t.Size());

    // Désallouez le tampon
    vertexBuffer->Unmap(0, nullptr);

    /*Configurer la vue de tampon de vertex*/

    /*Utilisez le tampon de vertex lors du rendu*/
}
