#pragma once
#include "framwork.h"
#include "Buffer.h"


class ConstantBuffer : public Buffer
{
public:
	ConstantBuffer(ConstantBufferData* constBufferData);

private:
	std::vector<ID3D12DescriptorHeap*> m_descriptorHeaps = {}; // Tableau de tas de descripteurs dont le shader a besoin pour accéder aux différentes ressources (1 tas par constant buffer)
};

