struct MyUniforms {
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

struct FragmentOutput {
	@builtin(frag_depth) depth: f32
};

@vertex fn vert_main(in: VertexInput) -> VertexOutput {
	var out: VertexOutput;
	let ratio = 800.0 / 600.0;

	let angle = myUniforms.time;
	let alpha: f32 = cos(angle);
	let beta: f32 = sin(angle);

	var position = vec3f(
		in.position.x,
		alpha * in.position.y + beta * in.position.z,
		alpha * in.position.z - beta * in.position.y
	);

	out.position = vec4f(position.x, (position.y) * ratio, position.z * 0.5 + 0.5, 1.0);
	out.color = in.color;
	return out;
}

//@fragment fn frag_main(in: VertexOutput) -> @location(0) vec4f {
@fragment fn frag_main(in: VertexOutput) -> FragmentOutput {
	//let color = in.color * myUniforms.color.rgb;
	//let linear_color = pow(color, vec3f(2.2));
	//return vec4f(linear_color, myUniforms.color.a);
	var out: FragmentOutput;
	//return vec4f(in.color.rgb, 1.0);
	return out;
}