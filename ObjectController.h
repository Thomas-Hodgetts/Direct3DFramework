#pragma once
#include "Structures.h"

using namespace DirectX;

class ObjectController
{

public:

	//The Function that will manipulate a 4x4 matrix in world space using data that the user passes in. 
	XMFLOAT4X4 MoveObject(XMFLOAT4X4 BasePosition, XMFLOAT3 TransformData, XMFLOAT3 ScaleData, float RotationData, Axis RotationAxis);

	//TBD
	XMFLOAT4X4 OribitObject(XMFLOAT4X4 BasePosition, XMFLOAT4X4 OribitPosition, XMFLOAT3 TransformData, XMFLOAT3 ScaleData, float RotationData, Axis RotationAxis);

	//The function will make one object trail another
	//XMFLOAT4X4 TrailObject(XMFLOAT4X4 ChildPosition, XMFLOAT4X4 ParentPosition, XMFLOAT3 TransformData, XMFLOAT3 ScaleData, float RotationData, Axis RotationAxis);

	XMFLOAT3 Return0Float3() { return { 0,0,0 }; };

protected:

private:


	//Uses DirectX math to translate the object and returns the resulting matrix.
	XMFLOAT4X4 Transform(XMFLOAT4X4 BasePosition, XMFLOAT3 NewData);
	//Uses DirectX math to scale the object and returns the resulting matrix.
	XMFLOAT4X4 Scale(XMFLOAT4X4 BasePosition, XMFLOAT3 NewData);
	//Uses DirectX math to roatate the object and returns the resulting matrix.
	XMFLOAT4X4 Rotation(XMFLOAT4X4 BasePosition,float NewData, Axis axis);




};

