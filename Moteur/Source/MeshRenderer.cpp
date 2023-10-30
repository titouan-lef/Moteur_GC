#include "Component.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(ConstantBufferData* constBufferData)
{
	m_constBuffer = new ConstantBuffer(constBufferData);
}
