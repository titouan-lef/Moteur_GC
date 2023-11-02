#include "Engine.h"
#include "MyException.h"

Engine::Engine()
{
	this->m_Instance = this;
}

Engine::~Engine()
{
}

void Engine::Draw(ID3D12RootSignature* rootSignature)
{
    // R�initialisaion pour enregistrer de nouvelles commandes pour la frame actuelle
    GFX_THROW_INFO_ONLY(CommandAllocator->Reset());
    GFX_THROW_INFO_ONLY(CommandList->Reset(CommandAllocator, PipelineStateObject));

    /* GESTION DES BUFFER */

    // Gestion Vertex Buffer
    CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);// Param�tre l'affichage pour fonctionner avec une liste de triangle


    /* AJOUT DES COMMANDES */

    // Ajout de la Root Signature
    CommandList->SetGraphicsRootSignature(rootSignature);// Ajout de la root signature

    // Ajout de la pipeline de rendu
    CommandList->SetPipelineState(PipelineStateObject);

    // Ajout des diff�rentes fen�tres et de leur zone de rendu
    CommandList->RSSetViewports((UINT)Viewport.size(), Viewport.data());// Ajout des fen�tres
    CommandList->RSSetScissorRects((UINT)Scissor.size(), Scissor.data());// Ajout des zones de rendu

    // Ajout des "surfaces de dessin" � utiliser
    CD3DX12_RESOURCE_BARRIER transition[] = {
        CD3DX12_RESOURCE_BARRIER::Transition(&RenderTarget[BackBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET)// Indique que RenderTarget[BackBufferIndex] est pr�te � �tre utilis�e comme "surfaces de dessin"
    };
    CommandList->ResourceBarrier(_countof(transition), transition);// Ajout des "surfaces de dessin" pr�tes � �tre utilis�es

    // Ajout des "surfaces de dessin" au back buffer
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTarget[] = {
        CD3DX12_CPU_DESCRIPTOR_HANDLE(RTVHeap->GetCPUDescriptorHandleForHeapStart(), BackBufferIndex, RTVDescriptorSize)
    };
    CommandList->OMSetRenderTargets(_countof(renderTarget), renderTarget, FALSE, nullptr);

    // Ajout de clearColor au premier plan pour effacer l'arri�re plan par r��criture
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    CommandList->ClearRenderTargetView(renderTarget[0], clearColor, (UINT)Scissor.size(), Scissor.data());

    // Ajout de l'affichage
    const UINT nbForme = 1;// Nombre d'instance (= forme du vertex buffer) � dessiner
    /*
    * TO DO :
    * cr�er un vertex buffer par forme
    * avoir une liste static dans la classe d'un objet pour avoir la matrice World de tous les objets dans une liste et appliquer la bonne matrice � la bonne instance via SV_InstanceID ?
    * mettre en place un systeme d'update des matrice World pour chaque objet
    */
    for (int i = 0; i < DescriptorsHeap.size(); ++i)
    {
        CommandList->SetDescriptorHeaps(1, &DescriptorsHeap[i]);// D�fini les descripteurs que la liste de commandes peut potentiellement utiliser
        CommandList->SetGraphicsRootDescriptorTable(0, DescriptorsHeap[i]->GetGPUDescriptorHandleForHeapStart());// Ajout des descripteurs dont le shader a besoin pour acc�der � diff�rentes ressources (associ� au constant buffer)
        CommandList->IASetVertexBuffers(0, (UINT)VertexBufferView.size(), VertexBufferView.data());// Ajout des vertex buffer
        CommandList->IASetIndexBuffer(&IndexBufferView[0]);// Ajout des index buffer
        CommandList->DrawIndexedInstanced((UINT)Indices.size(), nbForme, 0, 0, 0);// Affichage
    }


    // Indique au back buffer les render target � ne plus utiliser
    transition[0] = CD3DX12_RESOURCE_BARRIER::Transition(&RenderTarget[BackBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);// Indique que RenderTarget[BackBufferIndex] ne doit plus �tre utilis�e comme render target
    CommandList->ResourceBarrier(_countof(transition), transition);

    // Indique que l'enregistrement des commandes est termin� et que le GPU peut les utiliser pour le rendu
    GFX_THROW_INFO_ONLY(CommandList->Close());
}
