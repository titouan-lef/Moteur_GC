#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"
#include "Engine.h"
#include "ConstantBuffer.h"

enum Type
{
	texture, color
};

class Shader
{
public:
	ID3D12RootSignature* m_rootSignature = nullptr; // M�canisme qui d�finit comment les shaders acc�dent aux ressources graphiques
	ID3D12PipelineState* m_pipelineState = nullptr;// Sp�cifie comment la pipeline de rendu doit fonctionner pour chaque rendu


	Shader(Type type, ID3D12RootSignature* rootSignature);
	virtual ~Shader();

	static ID3D12RootSignature* CreateRootSignature(UINT nbParam, CD3DX12_ROOT_PARAMETER rootParameters[]);
	void CreatePSO();

	// TO DO : en private ?
	ConstantBuffer* m_constBuffer;

protected:
	// Gestion des erreurs
	#ifndef  NDEBUG
		static DxgiInfoManager infoManager;
	#endif

private:
	Type m_type;
};
