#include "Rectangle.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "ShaderColor.h"
//#include "ShaderColor.h"


Mesh* MyRectangle::m_mesh = new Mesh({
    // Carre
    { { -0.5f, 0.5f, 0 }, { 0.0f, 0.0f } },// Coin superieur gauche
    { { 0.5f, 0.5f, 0 }, { 1.0f, 0.0f } },// Coin superieur droit
    { { -0.5f, -0.5f, 0 }, { 0.0f, 1.0f } },// Coin inferieur gauche
    { { 0.5f, -0.5f, 0 }, { 1.0f, 1.0f } },// Coin inferieur droit
}, { 0, 1, 2, 2, 1, 3 });

MyRectangle::MyRectangle()
{
	this->AddComponent<Transform>();
    
    // TO DO : A supprimer
    GetComponent<Transform>()->SetScale(0.5f, 1, 0.5f);
   //this->GetComponent<Transform>()->MoveByVector({ -0.5f, 0, 0.5f });
    this->GetComponent<Transform>()->UpdateMatrix();

    XMMATRIX world = this->GetComponent<Transform>()->GetMatrixTranspose();
    this->AddComponent<MeshRenderer>()->Init(m_mesh, ShaderColor::GetInstance());
    //this->AddComponent<MeshRenderer>()->Init(m_mesh, new ShaderTexture(cbd, pierre));
}

MyRectangle::~MyRectangle()
{
}

//void MyRectangle::Init()
//{
//}

void MyRectangle::Update()
{
    this->GetComponent<Transform>()->UpdateMatrix();

    XMMATRIX world = this->GetComponent<Transform>()->GetMatrixTranspose();
    this->GetComponent<MeshRenderer>()->m_constBuffer->UpdateBuffer(world);
}

//void MyRectangle::PostUpdate()
//{
//}
//
//void MyRectangle::Render()
//{
//}
//
//void MyRectangle::OnDestroy()
//{
//}
