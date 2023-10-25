#pragma once
#include <Windows.h>
#include "d3d12.h"

class Shader {
public: 
	void Begin(ID3D12GraphicsCommandList* pList);
	void Draw(ID3D12GraphicsCommandList* pList, MeshGeometry* pMesh);
	void End(ID3D12GraphicsCommandList* pList);
protected:
	void AddObject();
public:
	static ID3DBlob* Compile(BYTE* buffer, int size, cstr entrypoint, cstr target);

protected:
	ID3D12Device* m_pDevice;
	ID3D12DescriptorHeap* m_pCvHeap;
	UINT m_descriptorSize;

	ID3DBlob* m_pSerializedRootSig;
	SUploadBufferBase* m_pPass;
	std::vector<SUploadBufferBase*> m_objects;
	ID3D12RootSignature* m_pRootSignature;
	ID3DBlob* m_pVS;
	ID3DBlob* m_pPS;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayout;
	ID3D12PipelineState* m_pPso;
	int m_index;

};

Shader::Shader() 
{
	m_pDevice = nullptr;
	m_pCvHeap = nullptr;
	m_descriptorSize = 0;
	m_pSerializedRootSig = nullptr;
	m_pPass = nullptr;
	m_pRootSignature = nullptr;
	m_pVS = nullptr;
	m_pPS = nullptr;
	m_pPso = nullptr;
}