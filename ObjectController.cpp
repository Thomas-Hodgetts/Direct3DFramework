#include "ObjectController.h"

XMFLOAT4X4 ObjectController::MoveObject(XMFLOAT4X4 BasePosition, XMFLOAT3 TransformData, XMFLOAT3 ScaleData, float RotationData, Axis RotationAxis)
{
	switch (RotationAxis)
	{
	case Axis::X:
		XMStoreFloat4x4(&BasePosition, XMMatrixTranslation(TransformData.x, TransformData.y, TransformData.z) * XMMatrixScaling(ScaleData.x, ScaleData.y, ScaleData.z) * XMMatrixRotationX(RotationData));
		break;
	case Axis::Y:
		XMStoreFloat4x4(&BasePosition, XMMatrixTranslation(TransformData.x, TransformData.y, TransformData.z) * XMMatrixScaling(ScaleData.x, ScaleData.y, ScaleData.z) * XMMatrixRotationY(RotationData));
		break;
	case Axis::Z:
		XMStoreFloat4x4(&BasePosition, XMMatrixTranslation(TransformData.x, TransformData.y, TransformData.z) * XMMatrixScaling(ScaleData.x, ScaleData.y, ScaleData.z) * XMMatrixRotationZ(RotationData));
		break;
	}
	return BasePosition;
}

XMFLOAT4X4 ObjectController::OribitObject(XMFLOAT4X4 BasePosition, XMFLOAT4X4 OribitPosition, XMFLOAT3 TransformData, XMFLOAT3 ScaleData, float RotationData, Axis RotationAxis)
{
	switch (RotationAxis)
	{
	case Axis::X:
		XMStoreFloat4x4(&BasePosition, XMMatrixTranslation(TransformData.x, TransformData.y, TransformData.z) * XMMatrixScaling(ScaleData.x, ScaleData.y, ScaleData.z) * XMMatrixRotationX(RotationData));
		break;
	case Axis::Y:
		XMStoreFloat4x4(&BasePosition, XMMatrixTranslation(TransformData.x, TransformData.y, TransformData.z) * XMMatrixScaling(ScaleData.x, ScaleData.y, ScaleData.z) * XMMatrixRotationY(RotationData));
		break;
	case Axis::Z:
		XMStoreFloat4x4(&BasePosition, XMMatrixTranslation(TransformData.x, TransformData.y, TransformData.z) * XMMatrixScaling(ScaleData.x, ScaleData.y, ScaleData.z) * XMMatrixRotationZ(RotationData));
		break;
	}
	return BasePosition;
}

XMFLOAT4X4 ObjectController::Transform(XMFLOAT4X4 BasePosition, XMFLOAT3 NewData)
{
	XMStoreFloat4x4(&BasePosition, XMMatrixTranslation(NewData.x, NewData.y, NewData.z));
	return BasePosition;
}

XMFLOAT4X4 ObjectController::Scale(XMFLOAT4X4 BasePosition, XMFLOAT3 NewData)
{
	XMStoreFloat4x4(&BasePosition, XMMatrixScaling(NewData.x, NewData.y, NewData.z));
	return BasePosition;
}

XMFLOAT4X4 ObjectController::Rotation(XMFLOAT4X4 BasePosition, float NewData, Axis axis)
{
	switch (axis)
	{
		X:
			XMStoreFloat4x4(&BasePosition, XMMatrixRotationX(NewData));
			break;
		Y:
			XMStoreFloat4x4(&BasePosition, XMMatrixRotationY(NewData));
			break;
		Z:
			XMStoreFloat4x4(&BasePosition, XMMatrixRotationZ(NewData));
			break;
	}
	return BasePosition;
}