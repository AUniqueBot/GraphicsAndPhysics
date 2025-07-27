layout(std140, binding = 0) uniform CameraBlock {
	// - matrices --------------------------
	mat4 u_modelMatrix;
	mat4 u_cameraMatrix;
	mat4 u_projectionMatrix;
};

