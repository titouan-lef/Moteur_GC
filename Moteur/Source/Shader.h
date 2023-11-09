#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"

class Shader
{
public:
	Shader();
	virtual ~Shader();
	void Init(std::vector<CD3DX12_ROOT_PARAMETER> rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC> samplers, std::wstring fileName, std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs);

private:
	void CreateRootSignature(std::vector<CD3DX12_ROOT_PARAMETER> rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC> samplers);
	void CreatePSO(std::wstring &fileName, std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs);


	// Gestion des erreurs
	#ifndef  NDEBUG
		DxgiInfoManager infoManager;
	#endif

		//TO DO en private
public:
	ID3D12RootSignature* m_rootSignature = nullptr;
	ID3D12PipelineState* m_pso = nullptr;







// fileName = L"Source/shaders" + fileName + L".hlsl";



//
//public:
//	ID3D12RootSignature* m_rootSignature = nullptr; // Mécanisme qui définit comment les shaders accèdent aux ressources graphiques
//	ID3D12PipelineState* m_pipelineState = nullptr;// Spécifie comment la pipeline de rendu doit fonctionner pour chaque rendu
//
//
//	Shader(Type type);
//	virtual ~Shader();
//
//	ID3D12RootSignature* CreateRootSignature(UINT nbParam, CD3DX12_ROOT_PARAMETER rootParameters[]);
//
//	virtual void SetGraphicsRoot() = 0;
//
//	// TO DO : en private ?
//	ConstantBuffer* m_constBuffer;
//	std::vector<ID3D12DescriptorHeap*> m_descriptorHeaps;
//
//protected:
//
//private:
//	Type m_type;
};
