#include "camera.h"

void CBaseCamera::LookAt(CBaseObject* obj)
{
	CVector3f WorldUp(0.0f,1.0f,0.0f);

	CVector3f L = obj->m_MyBody.centerPosition - m_MyBody.centerPosition;
	L.Normalize();

	CVector3f R;
	R.CrossProduct(WorldUp,L);

	CVector3f U;
	U.CrossProduct(L,R);

	//Set the Camera Data.
	m_MyBody.centerPosition		= m_MyBody.centerPosition;
	m_Right			= R;
	m_Up			= U;
	m_Look			= L;

}

void CBaseCamera::LookAt(CVector3f pos)
{
	CVector3f WorldUp(0.0f,1.0f,0.0f);

	CVector3f L = pos - m_MyBody.centerPosition;
	L.Normalize();

	CVector3f R;
	R.CrossProduct(WorldUp,L);

	CVector3f U;
	U.CrossProduct(L,R);

	//Set the Camera Data.
	m_MyBody.centerPosition		= m_MyBody.centerPosition;
	m_Right			= R;
	m_Up			= U;
	m_Look			= L;

}


void CBaseCamera::FollowLookAt(CBaseObject* obj)
{
	CVector3f WorldUp(0.0f,1.0f,0.0f);

	CVector3f L = obj->m_MyBody.centerPosition - m_MyBody.centerPosition;
	L.Normalize();

	CVector3f R;
	R.CrossProduct(WorldUp,L);

	CVector3f U;
	U.CrossProduct(L,R);

	//Set the Camera Data.
	m_MyBody.centerPosition		= m_MyBody.centerPosition;
	m_Right			= R;
	m_Up			= U;
	m_Look			= L;


	//m_MyBody.centerPosition.x = obj->m_MyBody.centerPosition.x;
	m_MyBody.centerPosition.z = obj->m_MyBody.centerPosition.z;

}

void CBaseCamera::SetLens(float fov, float aspect, float fnear, float ffar)
{
	m_Fov = fov;
	m_Aspect = aspect;
	m_Near = fnear;
	m_Far = ffar;
}

void CBaseCamera::SetViewFromMouse(float deltax, float deltay)
{

	//currentRotX -= angleZ;  

	//// If the current rotation (in radians) is greater than 1.0, we want to cap it.
	//if(currentRotX > 1.0f)
	//	currentRotX = 1.0f;
	//// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
	//else if(currentRotX < -1.0f)
	//	currentRotX = -1.0f;
	//// Otherwise, we can rotate the view around our position
	//else
	//{
	//	// To find the axis we need to rotate around for up and down
	//	// movements, we need to get a perpendicular vector from the
	//	// camera's view vector and up vector.  This will be the axis.
	//	CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
	//	vAxis = Normalize(vAxis);

	//	// Rotate around our perpendicular axis and along the y-axis
	//	RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	//	RotateView(angleY, 0, 1, 0);
	//}

	static float currentRotX = 0.0f;

	currentRotX += deltay;

	// If the current rotation (in radians) is greater than 1.0, we want to cap it.
	if(currentRotX > 1.0f)
		currentRotX = 1.0f;
	// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
	else if(currentRotX < -1.0f)
		currentRotX = -1.0f;
	// Otherwise, we can rotate the view around our position
	else
	{
		// To find the axis we need to rotate around for up and down
		// movements, we need to get a perpendicular vector from the
		// camera's view vector and up vector.  This will be the axis.
		//CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
		//vAxis = Normalize(vAxis);

		CVector3f view;
		view.CrossProduct(m_Look - m_MyBody.centerPosition,m_Up);
		view.Normalize();

		RotateView(deltay,view);
		RotateView(deltax,CVector3f(0.0f,1.0f,0.0f));


		// Rotate around our perpendicular axis and along the y-axis
	//	RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	//	RotateView(angleY, 0, 1, 0);
	}


}

void CBaseCamera::RotateView(float angle, CVector3f look)
{
	CVector3f vNewView;

	// Get the view vector (The direction we are facing)
	CVector3f vView = m_Look - m_MyBody.centerPosition;		

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * look.x * look.x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * look.x * look.y - look.z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * look.x * look.z + look.y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * look.x * look.y + look.z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * look.y * look.y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * look.y * look.z - look.x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * look.x * look.z - look.y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * look.y * look.z + look.x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * look.z * look.z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_Look = m_MyBody.centerPosition + vNewView;
}


void CBaseCamera::LookLeft()
{
	//Rotate along the y.
	m_Look.RotateY(-m_RotationXSpeed);
}

void CBaseCamera::LookUp()
{
	//Rotate along the x
	m_Look.RotateX(-m_RotationYSpeed);
}

void CBaseCamera::LookRight()
{
	//Rotate along the y.
	m_Look.RotateY(m_RotationXSpeed);
}

void CBaseCamera::LookDown()
{
	//Rotate along the y.
	m_Look.RotateX(m_RotationYSpeed);
}