#pragma once
#include "Engine.h"
#include "DxgiInfoManager.h"

class Buffer
{
public:
	Buffer(UINT bufferSize, void* src);
	virtual ~Buffer();

	void UpdateBuffer(const void* src);// Copie la source de données dans le buffer
	

protected:
	UINT m_bufferSize = 0;
	ID3D12Resource* m_buffer = nullptr;

private:
	// Gestion des erreurs
#ifndef  NDEBUG
	DxgiInfoManager infoManager = {};
#endif

};

