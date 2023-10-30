#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"

class Buffer
{
public:
	Buffer(UINT bufferSize, void* src);

protected:
	UINT m_bufferSize = 0;
	ID3D12Resource* m_buffer = nullptr;

private:
	// Gestion des erreurs
#ifndef  NDEBUG
	DxgiInfoManager infoManager = {};
#endif

	// Update the constant buffer
	void UpdateConstBuffer(const void* src);
};

