#include "Buffer.h"
#include "MyException.h"

Buffer::Buffer(UINT bufferSize) : m_bufferSize(bufferSize)
{
	CD3DX12_HEAP_PROPERTIES tmp1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC tmp2 = CD3DX12_RESOURCE_DESC::Buffer(m_bufferSize);
	GFX_THROW_INFO_ONLY(Engine::GetInstance()->Device->CreateCommittedResource(
		&tmp1,
		D3D12_HEAP_FLAG_NONE,
		&tmp2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_buffer)));

	mappedData = nullptr;
	GFX_THROW_INFO_ONLY(m_buffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));
}

Buffer::~Buffer()
{
	m_buffer->Unmap(0, nullptr);
}

void Buffer::UpdateBuffer(const void* src)
{
	memcpy(mappedData, src, m_bufferSize);
}