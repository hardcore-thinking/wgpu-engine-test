struct Uniforms {
    viewDirectionProjectionInverse: mat4x4f,
};
 
struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) pos: vec4f,
};
 
@group(0) @binding(0) var<uniform> uniforms: Uniforms;
@group(0) @binding(1) var skyboxTexture: texture_cube<f32>;
@group(0) @binding(2) var skyboxSampler: sampler;
 
@vertex fn vs(@builtin(vertex_index) vNdx: u32) -> VertexOutput {
    let pos = array(
        vec2f(-1, 3),
        vec2f(-1,-1),
        vec2f( 3,-1),
    );
    
    var out: VertexOutput;
    
    out.position = vec4f(pos[vNdx], 1, 1);
    out.pos = out.position;
    
    return out;
}

@fragment fn fs(out: VertexOutput) -> @location(0) vec4f {
    let t = uniforms.viewDirectionProjectionInverse * out.pos;
    return vec4f(pow((textureSample(skyboxTexture, skyboxSampler, normalize(t.xyz / t.w) * vec3f(1, 1, 1))).rgb, vec3f(2.2)), 1.0);
}