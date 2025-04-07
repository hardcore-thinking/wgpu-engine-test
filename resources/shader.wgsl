struct MyUniforms {
	projectionMatrix: mat4x4f,
	viewMatrix: mat4x4f,
	modelMatrix: mat4x4f,
	color: vec4f,
	cameraPosition: vec3f,
	time: f32,
};

@group(0) @binding(0) var<uniform> myUniforms: MyUniforms;
@group(0) @binding(1) var gradientTexture: texture_2d<f32>;
@group(0) @binding(2) var textureSampler: sampler;

struct VertexInput {
	@location(0) position: vec3f,
	@location(1) normal: vec3f,
	@location(2) color: vec3f,
	@location(3) uv: vec2f,
};

struct VertexOutput {
	@builtin(position) position: vec4f,
	@location(0) color: vec3f,
	@location(1) normal: vec3f,
	@location(2) uv: vec2f,
};

@vertex fn vert_main(in: VertexInput) -> VertexOutput {
	var out: VertexOutput;

	out.position = myUniforms.projectionMatrix * myUniforms.viewMatrix * myUniforms.modelMatrix * vec4f(in.position, 1.0);
	out.normal = (myUniforms.modelMatrix * vec4f(in.normal, 0.0)).xyz;
	out.color = in.color;
	out.uv = in.uv * 6.0;

	return out;
}

@fragment fn frag_main(in: VertexOutput) -> @location(0) vec4f {
	let normal = normalize(in.normal);

	//let lightColor1 = vec3f(1.0f, 0.9, 0.6);
	let lightColor1 = vec3f(1.0f, 1.0, 1.0);
	//let lightDirection1 = vec3f(0.5, -0.9, 0.1);
	//let lightDirection1 = vec3f(0.7, -0.8, 0.4);
	let lightDirection1 = normalize(myUniforms.cameraPosition);
	let shading1 = max(0.0, dot(lightDirection1, normal));
	
	//let lightColor2 = vec3f(0.6f, 0.9, 1.0);
	//let lightDirection2 = vec3f(-0.3, 0.4, 0.3);
	//let shading2 = max(0.0, dot(lightDirection2, normal));

	//let shading = shading1 * lightColor1 + shading2 * lightColor2;
	let shading = shading1 * lightColor1;
	
	let texelCoords = vec2i(in.uv * vec2f(textureDimensions(gradientTexture)));

	//let color = in.color * shading;
	//let color = textureLoad(gradientTexture, texelCoords, 0).rgb;
	//let color = textureSample(gradientTexture, textureSampler, in.uv).rgb;
	let color = textureSample(gradientTexture, textureSampler, in.uv).rgb * shading;
	
	let linear_color = pow(color, vec3f(2.2));
	return vec4f(linear_color, myUniforms.color.a);
}