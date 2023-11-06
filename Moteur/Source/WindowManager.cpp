
#include "MyException.h"
#include "Engine.h"
#include "WindowManager.h"
//#include "Camera.h"// TO DO : A supprimer
//#include "Shader.h"
//#include "MeshRenderer.h"
//#include "Collider.h"


WindowManager::WindowManager(UINT width, UINT height)
{
    m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
    m_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
    //m_entityTimer = new Timer();
}

WindowManager::~WindowManager()
{
}

void WindowManager::OnInit(UINT width, UINT height, HWND hWnd)
{
    LoadPipeline(width, height, hWnd);
    LoadAssets();

    ////TO DO : A supprimer
    //Camera::m_Instance = new Camera();
    //
    //r1 = new Cube();
    //r2 = new Cube();
    //
    //r1->GetComponent<Transform>()->SetScale(0.2f, 0.2f, 0.2f);
    //r1->GetComponent<Transform>()->SetPosition(-0.45f, 0, 1);
    //r1->GetComponent<Transform>()->UpdateMatrix();
    //r1->GetComponent<Transform>()->SetDirection(0.01f, 0, 0);
    //r1->GetComponent<Transform>()->SetRotationSpeed(45, 35, 90);
    //
    //r1->AddComponent<Collider>();
    //Transform colR1;
    //colR1.SetPosition(-0.45f, 0, 1);
    //colR1.SetScale(0.2f, 0.2f, 0.2f);
    //colR1.SetDirection(0.01f, 0, 0);
    //colR1.SetRotationSpeed(45, 35, 90);
    //colR1.UpdateMatrix();
    //r1->GetComponent<Collider>()->SetCollider(colR1);

    //r2->GetComponent<Transform>()->SetScale(0.2f, 0.2f, 0.2f);
    //r2->GetComponent<Transform>()->SetPosition(0.45f, 0, 1);
    //r2->GetComponent<Transform>()->RotatePitch(45);
    //r2->GetComponent<Transform>()->UpdateMatrix();
    //r2->GetComponent<Transform>()->SetDirection(-0.001f, 0, 0);
    //r2->GetComponent<Transform>()->SetRotationSpeed(-45, 35, -90);
    //
    //r2->AddComponent<Collider>();
    //Transform colR2;
    //colR2.SetPosition(0.45f, 0, 1);
    //colR2.SetScale(0.2f, 0.2f, 0.2f);
    //colR2.SetDirection(-0.001f, 0, 0);
    //colR2.SetRotationSpeed(-45, 35, -90);
    //colR2.UpdateMatrix();
    //r2->GetComponent<Collider>()->SetCollider(colR2);

    //r1->Update();
    //r2->Update();

    //m_entities.push_back(r1);
    //m_entities.push_back(r2);
    ////cb.push_back(r1->GetComponent<MeshRenderer>()->m_constBuffer);
    ////cb.push_back(r2->GetComponent<MeshRenderer>()->m_constBuffer);
}



void WindowManager::LoadPipeline(UINT width, UINT height, HWND hWnd)
{
    #if defined(_DEBUG)
    SetupDebugLayer();
    #endif

    CreateCommandQueue();
    CreateSwapChain(hWnd, width, height);
    CreateDescriptorHeaps();
    CreateFrameResources();
}

#pragma region LoadPipelineFunction
void WindowManager::SetupDebugLayer()
{
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }
}

void WindowManager::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    GFX_THROW_INFO_ONLY(Engine::Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
}

void WindowManager::CreateSwapChain(HWND hWnd, UINT width, UINT height)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    IDXGISwapChain1* swapChain;
    GFX_THROW_INFO_ONLY(Engine::Factory->CreateSwapChainForHwnd(m_commandQueue, hWnd, &swapChainDesc, nullptr, nullptr, &swapChain));

    GFX_THROW_INFO_ONLY(Engine::Factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

    m_swapChain = (IDXGISwapChain3*)swapChain;
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();// Indique sur quel back buffer on va pouvoir travailler (ici on en a un seul donc m_backBufferIndex sera toujours égal à 0)
}

void WindowManager::CreateDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    GFX_THROW_INFO_ONLY(Engine::Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
    m_rtvDescriptorSize = Engine::Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);// Récupération de la taille d'un descripteur
}


void WindowManager::CreateFrameResources()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());// Récupération de l'emplacement prévu pour la "surface de dessin" (= render target) 0

    for (UINT n = 0; n < FrameCount; n++)
    {
        GFX_THROW_INFO_ONLY(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));// Récupération de la "surface de dessin" n
        Engine::Device->CreateRenderTargetView(m_renderTargets[n], nullptr, rtvHandle);// Création de la "surface de dessin" n dans l'emplacement prévu
        rtvHandle.Offset(1, m_rtvDescriptorSize);// Récupération de l'emplacement prévu pour la "surface de dessin" n+1
    }
}
#pragma endregion

void WindowManager::LoadAssets()
{
    CreateSyncObj();
}

#pragma region LoadAssetsFunction

void WindowManager::CreateSyncObj()
{
    GFX_THROW_INFO_ONLY(Engine::Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));// Initialisation de m_fence
}
#pragma endregion

void WindowManager::OnUpdate()
{
    //float elapsedTime = m_entityTimer->Peek();
    ////if (elapsedTime > 5.0f)  // Plus de 5 secondes se sont écoulées
    ////{
    ////    CreateEntity();
    ////
    ////    m_entityTimer->Mark();  // Réinitialiser le timer
    ////}

    //for (auto entityIt = m_entities.begin(); entityIt != m_entities.end(); ++entityIt) {
    //    auto& entity = *entityIt;
    //    auto transform = entity->GetComponent<Transform>();
    //    auto collider = entity->GetComponent<Collider>();

    //    // Mettre à jour la position
    //    collider->GetCollider()->MoveByVector(XMFLOAT3(collider->GetCollider()->GetDirection()), elapsedTime);
    //    //
    //   //// //// Mettre à jour la rotation
    //    XMFLOAT3 rotationSpeed = collider->GetCollider()->GetRotationSpeed();
    //    collider->GetCollider()->RotateRoll(rotationSpeed.x * elapsedTime * 0.1f);
    //    collider->GetCollider()->RotatePitch(rotationSpeed.y * elapsedTime * 0.1f);
    //    collider->GetCollider()->RotateYaw(rotationSpeed.z * elapsedTime * 0.1f);

    //    // Mettre à jour la position
    //    transform->MoveByVector(XMFLOAT3(transform->GetDirection()), elapsedTime);

    //    // // Mettre à jour la rotation
    //    XMFLOAT3 rotationSpeed2 = transform->GetRotationSpeed();
    //    transform->RotateRoll(rotationSpeed2.x * elapsedTime * 0.1f);
    //    transform->RotatePitch(rotationSpeed2.y * elapsedTime * 0.1f);
    //    transform->RotateYaw(rotationSpeed2.z * elapsedTime * 0.1f);

    //    entity->Update();


    //}
    //for (int i = 0; i < m_entities.size(); ++i) {
    //    auto& entity1 = m_entities[i];
    //    auto collider1 = entity1->GetComponent<Collider>();
    //    auto transform1 = entity1->GetComponent<Transform>();
    //    for (int j = i + 1; j < m_entities.size(); ++j) {
    //        auto& entity2 = m_entities[j];
    //        auto collider2 = entity2->GetComponent<Collider>();
    //        auto transform2 = entity2->GetComponent<Transform>();
    //        if (collider1->CheckCollision(*collider1, *collider2)) {
    //            Transform::ChangeDirection(collider1->GetCollider());
    //            Transform::ChangeDirection(collider2->GetCollider());
    //            Transform::ChangeDirection(transform1);
    //            Transform::ChangeDirection(transform2);
    //        }
    //    }

    //}
}

void WindowManager::OnRender()
{
    PopulateCommandList();

    // Exécution des commandes (ici nous n'utilisons qu'une seul liste de comande)
    ID3D12CommandList* ppCommandLists[1] = { Engine::CmdList };
    m_commandQueue->ExecuteCommandLists(1, ppCommandLists);

    // Affichage de la frame.
    GFX_THROW_INFO_ONLY(m_swapChain->Present(1, 0));

    WaitForPreviousFrame();
}

void WindowManager::PopulateCommandList()
{
    // Réinitialisaion pour enregistrer de nouvelles commandes pour la frame actuelle
    GFX_THROW_INFO_ONLY(Engine::CmdAllocator->Reset());
    GFX_THROW_INFO_ONLY(Engine::CmdList->Reset(Engine::CmdAllocator, nullptr));

    // Paramètre l'affichage pour fonctionner avec une liste de triangle
    Engine::CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /* AJOUT DES COMMANDES */

    // Ajout des différentes fenêtres et de leur zone de rendu
    Engine::CmdList->RSSetViewports(1, &m_viewport);          // Ajout des fenêtres (ici 1 seule)
    Engine::CmdList->RSSetScissorRects(1, &m_scissorRect); // Ajout des zones de rendu (ici 1 seule)

    // Indique que m_renderTargets[m_backBufferIndex] est prête à être utilisée comme "surfaces de dessin"
    CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

    // Ajout des "surfaces de dessin" prêtes à être utilisées (ici 1 seule)
    Engine::CmdList->ResourceBarrier(1, &transition);

    // Ajout des "surfaces de dessin" au back buffer (ici 1 seule)
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTarget = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_backBufferIndex, m_rtvDescriptorSize);
    Engine::CmdList->OMSetRenderTargets(1, &renderTarget, FALSE, nullptr);

    // Ajout de clearColor au premier plan pour effacer l'arrière plan par réécriture
    Engine::CmdList->ClearRenderTargetView(renderTarget, m_clearColor, 1, &m_scissorRect);// Ajout de clearColor aux "surfaces de dessin" (ici 1 seule)

    // Ajout de l'affichage    
    // POUR LES TEXTURES
    //constBuffer->CreateTexture(Engine::CmdList);
    // POUR LES COULEURS
    //MeshRenderer* meshRenderer[] = {
    //    r1->GetComponent<MeshRenderer>(),
    //    r2->GetComponent<MeshRenderer>()
    //};
    //Shader* shader[] = {
    //    meshRenderer[0]->m_shader,
    //   meshRenderer[1]->m_shader
    //};
    //ConstantBuffer* constBuffer[] = {
    //    shader[0]->m_constBuffer,
    //    shader[1]->m_constBuffer
    //};

    //for (int i = 0; i < _countof(shader); ++i)
    //{
    //    Engine::CmdList->SetGraphicsRootSignature(shader[i]->m_rootSignature);// Ajout de la Root Signature
    //    Engine::CmdList->SetPipelineState(shader[i]->m_pipelineState);// Ajout de la pipeline de rendu

    //    Engine::CmdList->SetDescriptorHeaps(shader[i]->m_descriptorHeaps.size(), shader[i]->m_descriptorHeaps.data());

    //    constBuffer[i]->SetGraphicsRoot();

    //    Engine::CmdList->IASetVertexBuffers(0, 1, &meshRenderer[i]->m_mesh->m_vertexBuffer->m_vertexBufferView);// Ajout des vertex buffer (ici 1 seul)
    //    Engine::CmdList->IASetIndexBuffer(&meshRenderer[i]->m_mesh->m_indexBuffer->m_indexBufferView);// Ajout des index buffer (ici 1 seul)
    //    Engine::CmdList->DrawIndexedInstanced(meshRenderer[i]->m_mesh->m_indexBuffer->m_nbVertex, 1, 0, 0, 0);// Affichage (avec toujours une seule instance)
    //}

    // Indique au back buffer les "surfaces de dessin" à ne plus utiliser
    transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    Engine::CmdList->ResourceBarrier(1, &transition);

    // Indique que l'enregistrement des commandes est terminé et que le GPU peut les utiliser pour le rendu
    GFX_THROW_INFO_ONLY(Engine::CmdList->Close());
}

void WindowManager::WaitForPreviousFrame()
{
    // Indique que les commandes dans le file doivent être terminées avant de continuer
    const UINT64 fence = m_fenceId;
    GFX_THROW_INFO_ONLY(m_commandQueue->Signal(m_fence, fence));

    if (m_fence->GetCompletedValue() < fence)// Si la frame précédente n'a pas fini d'être traité
    {
        HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);// Signale la fin du rendu d'une frame

        if (fenceEvent != nullptr)// Vérifie que la création de l'événement s'est déroulée avec succès
        {
            GFX_THROW_INFO_ONLY(m_fence->SetEventOnCompletion(fence, fenceEvent));// Créer un évènement qui indique si la frame précédente est traité
            WaitForSingleObject(fenceEvent, INFINITE);// Tant que la frame précédente n'est pas traité, le programme est suspendu
            CloseHandle(fenceEvent);// Supprime l'évènement de fin de rendu d'une frame
        }
    }

    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();// Indique quel est le back buffer actuel (l'indice varie ici de 0 à 1 car on utilise 2 buffers : le back et front buffer)
    m_fenceId++;// On passe à la prochaine frame
}