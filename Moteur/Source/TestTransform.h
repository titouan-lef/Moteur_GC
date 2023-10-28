#pragma once

#include "framwork.h"
#include "Entity.h"

int RunTests();

void TestSetPos(Entity*);          // Teste la fonction SetPosition
void TestMove(Entity*);            // Teste la fonction MoveByVector
void TestRotate(Entity*);          // Teste la fonction Rotate
void TestScale(Entity*);           // Teste la fonction Scale
void TestMatrix(Entity*);          // Teste la fonction UpdateMatrix
void TestQuaternion(Entity*);      // Teste la fonction UpdateRotationFromQuaternion

