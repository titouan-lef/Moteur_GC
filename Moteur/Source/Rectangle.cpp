#include "Rectangle.h"
#include "MeshRenderer.h"
#include "Camera.h"//TO DO : A supprimer

//Mesh* MyRectangle::m_mesh = new Mesh(
//    {
//        // Carre
//        { { -0.5f, 0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin superieur gauche
//        { { 0.5f, 0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin superieur droit
//        { { -0.5f, -0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin inferieur gauche
//        { { 0.5f, -0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin inferieur droit
//    },
//    { 0, 1, 2, 2, 1, 3 }
//);

MyRectangle::MyRectangle()
{
	this->AddComponent<Transform>();
    
    // TO DO : A supprimer
    GetComponent<Transform>()->SetScale(0.5f, 1, 0.5f);
    this->GetComponent<Transform>()->MoveByVector({ -0.5f, 0, 0.5f });
    this->GetComponent<Transform>()->UpdateMatrix();
    ConstantBufferData* cbd = new ConstantBufferData();
    cbd->World = this->GetComponent<Transform>()->GetMatrixTranspose();
    Camera* camera = new Camera();
    cbd->View = camera->GetViewMatrix();
    cbd->Projection = camera->GetProjMatrix();

    this->AddComponent<MeshRenderer>();
    //this->GetComponent<MeshRenderer>()->Init(m_mesh, cbd);
}

MyRectangle::~MyRectangle()
{
}

void MyRectangle::Init()
{
}

void MyRectangle::Update()
{

}

void MyRectangle::PostUpdate()
{
}

void MyRectangle::Render()
{
}

void MyRectangle::OnDestroy()
{
}
