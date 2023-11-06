#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"
#include "Engine.h"
#include "ConstantBuffer.h"
#include "ConstantBufferLight.h"

enum Type
{
	texture, color
};
struct Light {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 intensity;
	DirectX::XMFLOAT3 direction;
};
class Shader
{
public:
	ID3D12RootSignature* m_rootSignature = nullptr; // Mécanisme qui définit comment les shaders accèdent aux ressources graphiques
	ID3D12PipelineState* m_pipelineState = nullptr;// Spécifie comment la pipeline de rendu doit fonctionner pour chaque rendu


	Shader(Type type, ID3D12RootSignature* rootSignature);
	virtual ~Shader();

	static ID3D12RootSignature* CreateRootSignature(UINT nbParam, CD3DX12_ROOT_PARAMETER rootParameters[]);
	void CreatePSO();

	virtual void SetGraphicsRoot() = 0;

	// TO DO : en private ?
	ConstantBufferLight* m_constBufferLight;
	ConstantBuffer* m_constBuffer;
	std::vector<ID3D12DescriptorHeap*> m_descriptorHeaps;

	Light lightData;

protected:
	// Gestion des erreurs
	#ifndef  NDEBUG
		static DxgiInfoManager infoManager;
	#endif

private:
	Type m_type;
};
