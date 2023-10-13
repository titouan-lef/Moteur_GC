#include "Buffer.h"

Buffer::Buffer(Triangle t)
{
    /*Cr�ez un tampon de vertex*/
    ID3D12Resource* vertexBuffer = nullptr;

    // Propri�t�s du tas de m�moire dans lequel le buffer sera allou�
    D3D12_HEAP_PROPERTIES heapProperties = {};
    heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

    // Propri�t�s du buffer
    D3D12_RESOURCE_DESC bufferDesc = {};
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Width = t.Size();// TO DO
    bufferDesc.Height = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.MipLevels = 1;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    
    // Cr�ez le buffer
    ID3D12Device* g_pd3dDevice = nullptr;
    g_pd3dDevice->CreateCommittedResource(
        &heapProperties,  // Propri�t�s du tas de m�moire
        D3D12_HEAP_FLAG_NONE,  // Drapeaux du tas de m�moire
        &bufferDesc,  // Description du tampon
        D3D12_RESOURCE_STATE_GENERIC_READ,  // �tat initial du tampon
        nullptr,  // Description d'allocation personnalis�e (peut �tre nul)
        __uuidof(ID3D12Resource),
        (void**)&vertexBuffer
    );

    /*Copiez les donn�es des sommets dans le tampon*/
    void* pVertexDataBegin = nullptr;  // D�clarez le pointeur et initialisez-le � nullptr

    // Pas besoin de sp�cifier une plage de lecture, vous pouvez acc�der � l'ensemble du tampon
    vertexBuffer->Map(0, nullptr, &pVertexDataBegin);

    // Copiez les donn�es des sommets dans le tampon
    memcpy(pVertexDataBegin, t.GetTabVertices(), t.Size());

    // D�sallouez le tampon
    vertexBuffer->Unmap(0, nullptr);

    /*Configurer la vue de tampon de vertex*/

    /*Utilisez le tampon de vertex lors du rendu*/
}
