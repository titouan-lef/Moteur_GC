#pragma once
#include "Engine.h"
#include "DxgiInfoManager.h"

class Buffer
{
public:
	Buffer(UINT bufferSize);
	virtual ~Buffer();


	// TO DO : Protected ?
	UINT m_bufferSize = 0;
	ID3D12Resource* m_buffer = nullptr;

protected:
	void UpdateBuffer(const void* src);// Copie la source de données dans le buffer

private:

	// Gestion des erreurs
#ifndef  NDEBUG
	DxgiInfoManager infoManager = {};
#endif

};

