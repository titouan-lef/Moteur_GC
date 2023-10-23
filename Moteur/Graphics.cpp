#include "Graphics.h"
#include <sstream>
#include <string>
#include <d3dcompiler.h>
#include "MyWindow.h"

namespace wrl = Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// Constructeur de la classe Graphics
Graphics::Graphics(HWND hWnd)
{
    // Configuration de la description du mode de swap (DXGI_SWAP_CHAIN_DESC)
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // Pour vérifier le résultat
    HRESULT hr;

    // Création de l'appareil (device) et de la chaîne de swap (swap chain)
    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwap,
        &pDevice,
        nullptr,
        &pContext
    );

    // Obtention du tampon (buffer) de la cible arrière (back buffer)
    wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
    pSwap->GetBuffer(0, __uuidof(ID3D11Resource), (&pBackBuffer));
    pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);
}

// Fonction pour afficher le contenu à l'écran
void Graphics::EndFrame()
{
    HRESULT hr;
    if (FAILED(hr = pSwap->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw pDevice->GetDeviceRemovedReason();
        }
        else
        {
            throw hr;
        }
    }
}

// Fonction pour effacer le tampon de rendu avec une couleur spécifiée
void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
    const float color[] = { red, green, blue, 1.0 };
    pContext->ClearRenderTargetView(pTarget.Get(), color);
}

// Fonction pour dessiner un triangle de test
void Graphics::DrawTestTriangle() {
    namespace wrl = Microsoft::WRL;
    HRESULT hr;
    struct Vertex {
        float x;
        float y;
    };
    const Vertex vertices[] = {
        { 0.0f, 0.5f },
        { 0.5f, -0.5f },
        { -0.5f, -0.5f },
    };

    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

    // Lier le tampon de sommets (vertex buffer) au pipeline de rendu
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

    // Créer un pixel shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
    pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

    // Lier le pixel shader au pipeline de rendu
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

    // Créer un vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
    pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

    // Lier le vertex shader au pipeline de rendu
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

    // Configuration du format d'entrée des sommets (input vertex layout) pour des données 2D
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] = {
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    pDevice.Get()->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    );

    // Lier le format d'entrée des sommets (input vertex layout)
    pContext->IASetInputLayout(pInputLayout.Get());

    // Lier la cible de rendu (render target)
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

    // Configuration du type de primitif (liste de triangles)
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Configuration de la vue (viewport)
    D3D11_VIEWPORT vp;
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0;
    vp.MaxDepth = 0;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1u, &vp);

    // Dessiner le triangle
    pContext.Get()->Draw((UINT)std::size(vertices), 0u);
}
