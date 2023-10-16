#include "PipeLine.h"

PipeLine::PipeLine()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
	pipelineState.InputLayout = { nullptr, 0 };
	pipelineState.pRootSignature = nullptr;
	//pipelineState.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
	//pipelineState.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
	pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipelineState.DepthStencilState.DepthEnable = FALSE;
	pipelineState.DepthStencilState.StencilEnable = FALSE;
	pipelineState.SampleMask = UINT_MAX;
	pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineState.NumRenderTargets = 1;
	pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineState.SampleDesc.Count = 1;
}
