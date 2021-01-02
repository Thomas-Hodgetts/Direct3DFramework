#pragma once
#include "ImaginaryObject.h"
#include "PhysicalObject.h"

//Framework's camera class. Child of type imaginary object and so can be used in flotillas
class Camera : public ImaginaryObject 
{
	private:
		// Private attributes to store the camera position and view 
		// volume

		//Position
		XMFLOAT3 m_Eye;
		//Focus
		XMFLOAT3 m_At;
		//Shows which way is up
		XMFLOAT3 m_Up;
		//Shows Which way is right
		XMFLOAT3 m_Right;

		FLOAT m_WindowWidth;
		FLOAT m_WindowHeight;
		FLOAT m_NearDepth;
		FLOAT m_FarDepth;

		// attributes to hold the view and projection matrices which 
		// will be passed to the shader

		XMFLOAT4X4 m_View;
		XMFLOAT4X4 m_Projection;
		float Sensitivity = 110.5;

		ObjectController c_Controller;


	public:


		Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT
			windowWidth, FLOAT windowHeight, FLOAT
			nearDepth, FLOAT farDepth);

		//to stop error C2512
		Camera() = default;
		~Camera();

		//SET//

		void SetEye(XMFLOAT3 pos) { m_Eye = pos; };
		void SetAt(XMFLOAT3 At) { m_At = At; };
		void SetUp(XMFLOAT3 Up) { m_Up = Up ;};
		void SetRight(XMFLOAT3 Right) { m_Right = Right ;};
		void SetLookTo(XMVECTOR Forwardvector);


		//GET//

		XMVECTOR GetEye() { return XMLoadFloat3(&m_Eye); };
		XMVECTOR GetAt() { return XMLoadFloat3(&m_At); };
		XMVECTOR GetUp() { return XMLoadFloat3(&m_Up); };
		XMVECTOR GetRight() { return XMLoadFloat3(&m_Right); };

		//Movement//

		//Takes the mouse input and makes it useful to the class
		void ManageMouseMovement(MouseData MD);


		//Moves the Camera along the Z vector
		void Walk(float Direction);
		//Moves the Camera along the Z vector
		void Strafe(float Direction);
		//Rotates the Camera on the X axis
		void Pitch(float Angle);
		//Rotates the Camera on the Y axis
		void Yaw(float Angle);

		//Lens Effects//

		//To be fixed or removed
		void Zoom();
		void Unzoom();


		//Updates the Porjection and view of the object
		void Update();

		//Returns a struct Camera data.
		CameraData ReturnCamData();

		//Returns the View and Projection matrix and a struct
		CameraBufferData ReturnViewPlusProjection();

		//Here to reset Camera if the window is resized. must be called manually
		void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	};


