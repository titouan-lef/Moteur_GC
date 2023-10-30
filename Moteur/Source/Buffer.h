#pragma once
#include "framwork.h"

class Buffer
{
public:
	Buffer(UINT bufferSize, void* src);

protected:
	UINT m_bufferSize = 0;
	ID3D12Resource* m_buffer = nullptr;

private:
	// Update the constant buffer
	void UpdateConstBuffer(const void* src);
};

