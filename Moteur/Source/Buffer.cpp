#include "Buffer.h"
#include "MyException.h"

Buffer::Buffer(UINT bufferSize, void* src) : m_bufferSize(bufferSize)
{
	auto tmp1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto tmp2 = CD3DX12_RESOURCE_DESC::Buffer(m_bufferSize);
	GFX_THROW_INFO_ONLY(Engine::Device->CreateCommittedResource(
		&tmp1,
		D3D12_HEAP_FLAG_NONE,
		&tmp2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_buffer)));

	UpdateConstBuffer(src);
}

void Buffer::UpdateConstBuffer(const void* src)
{
	// Copie des donn�es dans le buffer
	BYTE* mappedData = nullptr;
	GFX_THROW_INFO_ONLY(m_buffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));
	memcpy(mappedData, src, m_bufferSize);
	m_buffer->Unmap(0, nullptr);
}