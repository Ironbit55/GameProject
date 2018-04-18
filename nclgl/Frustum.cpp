#include "Frustum.h"

bool Frustum::InsideFrustum(SceneNode &n) {
	for (int p = 0; p < 6; ++p) {
		if (!planes[p].SphereInPlane(n.GetWorldTransform().GetPositionVector(), n.GetBoundingRadius())) {
			return false; // scenenode is outside this plane !
		}
	}
	return true; // Scenenode is inside every plane ...
}

bool Frustum::InsideFrustum(Vector3 position, float boundingRadius){
	for (int p = 0; p < 4; ++p) {
		if (!planes[p].SphereInPlane(position, boundingRadius)) {
			return false; // scenenode is outside this plane !
		}
	}
	return true; // Scenenode is inside every plane ...
}

bool Frustum::InsideFrustum2d(Vector2 position, float boundingRadius){
	//if (position.x + boundingRadius < (-planes[0].GetDistance() * planes[0].GetNormal().x)){
	//	//outside rectange frustrum on right side
	//	return false;
	//}

	//if (position.x - boundingRadius > (planes[1].GetDistance() * planes[1].GetNormal().x)) {
	//	//outside left
	//	return false;
	//}

	//if (position.y + boundingRadius > (planes[2].GetDistance() * planes[2].GetNormal().y)) {
	//	//outside bottom
	//	return false;
	//}

	//if (position.y - boundingRadius < (planes[3].GetDistance() * planes[3].GetNormal().y)) {
	//	//outside top
	//	return false;
	//}

	if (position.x - boundingRadius > (+planes[0].GetDistance())) {
		//outside rectange frustrum on right side
		return false;
	}

	if (position.x + boundingRadius < (-planes[1].GetDistance())) {
		//outside left
		return false;
	}

	if (position.y + boundingRadius < (-planes[2].GetDistance())) {
		//outside bottom
		return false;
	}

	if (position.y - boundingRadius > (planes[3].GetDistance())) {
		//outside top
		return false;
	}

	return true;
}

void Frustum::FromMatrix(const Matrix4 & mat) {
	Vector3 xaxis = Vector3(mat.values[0], mat.values[4], mat.values[8]);
	Vector3 yaxis = Vector3(mat.values[1], mat.values[5], mat.values[9]);
	Vector3 zaxis = Vector3(mat.values[2], mat.values[6], mat.values[10]);
	Vector3 waxis = Vector3(mat.values[3], mat.values[7], mat.values[11]);
	// RIGHT
	planes[0] = Plane(waxis - xaxis, (mat.values[15] - mat.values[12]), true);
	// LEFT
	planes[1] = Plane(waxis + xaxis, (mat.values[15] + mat.values[12]), true);
	// BOTTOM
	planes[2] = Plane(waxis + yaxis, (mat.values[15] + mat.values[13]), true);
	// TOP
	planes[3] = Plane(waxis - yaxis, (mat.values[15] - mat.values[13]), true);
	// FAR
	planes[4] = Plane(waxis - zaxis, (mat.values[15] - mat.values[14]), true);
	// NEAR
	planes[5] = Plane(waxis + zaxis, (mat.values[15] + mat.values[14]), true);
}

