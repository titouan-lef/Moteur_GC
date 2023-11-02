#pragma once
#include "Buffer.h"

class UploadBuffer : public Buffer
{
public:
	UploadBuffer(ID3D12Resource* m_texture);
	virtual ~UploadBuffer();

public://TO DO : en private ?
	ID3D12DescriptorHeap* m_srvHeap = nullptr;// Tas de descripteurs dont le shader a besoin pour accéder aux différentes ressources
};