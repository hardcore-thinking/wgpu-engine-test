struct MyUniforms {
	//projectionMatrix: mat4x4f,
	//viewMatrix: mat4x4f,
	//modelMatrix: mat4x4f,
	color: vec4f,
	time: f32
};

@group(0) @binding(0) var<uniform> myUniforms: MyUniforms;

struct VertexInput {
	@location(0) position: vec3f,
	@location(1) color: vec3f
};

struct VertexOutput {
	@builtin(position) position: vec4f,
	@location(0) color: vec3f
};

@vertex fn vert_main(in: VertexInput) -> VertexOutput {
	var pi : f32 = 3.1415926535897932384626433832795;
	var out: VertexOutput;
	let ratio = 800.0 / 600.0;

	var position = in.position;

	let S = transpose(mat4x4f(
		0.3,  0.0, 0.0, 0.0,
		0.0,  0.3, 0.0, 0.0,
		0.0,  0.0, 0.3, 0.0,
		0.0,  0.0, 0.0, 1.0,
	));

	let T = transpose(mat4x4f(
		1.0,  0.0, 0.0, 0.5,
		0.0,  1.0, 0.0, 0.0,
		0.0,  0.0, 1.0, 0.0,
		0.0,  0.0, 0.0, 1.0,
	));

	let angle1 = myUniforms.time;
	let c1 = cos(angle1);
	let s1 = sin(angle1);
	let R1 = transpose(mat4x4f(
		 c1,  s1, 0.0, 0.0,
		-s1,  c1, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
	));

	let angle2 = 3.0 * pi / 4.0;
	let c2 = cos(angle2);
	let s2 = sin(angle2);
	let R2 = transpose(mat4x4f(
		1.0, 0.0, 0.0, 0.0,
		0.0,  c2,  s2, 0.0,
		0.0, -s2,  c2, 0.0,
		0.0, 0.0, 0.0, 1.0,
	));

	let focalPoint = vec3f(0.0, 0.0, -2.0);
	let T2 = transpose(mat4x4f(
		1.0,  0.0, 0.0, -focalPoint.x,
		0.0,  1.0, 0.0, -focalPoint.y,
		0.0,  0.0, 1.0, -focalPoint.z,
		0.0,  0.0, 0.0,     1.0,
	));

	let homogeneous_position = vec4f(in.position, 1.0);
	let viewspace_position = T2 * R2 * R1 * T * S * homogeneous_position;

	let near = 0.01;
	let far = 100.0;
	let focal_length = 2.0;
	let divides = 1.0 / (far - near);
	let P = transpose(mat4x4f(
		focal_length,                  0.0,           0.0,                   0.0,
		         0.0, focal_length * ratio,           0.0,                   0.0,
		         0.0,                  0.0, far * divides, -far * near * divides,
		         0.0,                  0.0,           1.0,                   0.0,
	));

	out.position = P * viewspace_position;
	
	out.color = in.color;
	
	return out;
}

@fragment fn frag_main(in: VertexOutput) -> @location(0) vec4f {
	let color = in.color * myUniforms.color.rgb;
	let linear_color = pow(color, vec3f(2.2));
	return vec4f(linear_color, myUniforms.color.a);
}