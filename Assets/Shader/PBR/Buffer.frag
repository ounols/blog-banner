precision highp float;
precision highp int;


//Uniforms
//[texture.albedo]//
uniform sampler2D u_sampler_albedo;

//Varying
in mediump vec2 v_textureCoordOut;
in mediump vec3 v_worldPosition;

out vec4 FragColor;

void main(void) {
	highp vec4 albedo = texture(u_sampler_albedo, v_textureCoordOut);
	FragColor = albedo;
}