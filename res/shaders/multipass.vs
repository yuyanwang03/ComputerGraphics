// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;
uniform sampler2D u_colorTexture;
uniform sampler2D u_normalTexture;
uniform vec3 u_Ia, u_Id, u_Is, u_lightPosition, u_Ka, u_Ks, u_eye;
uniform float u_alfa, u_addAmbient;

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_world_position, v_world_normal;

//here create uniforms for all the data we need here

void main()
{
    vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;
    vec3 world_normal = (u_model * vec4( gl_Normal.xyz, 0.0)).xyz;
    
    // Pass parameters to fragment shader
	v_uv = gl_MultiTexCoord0.xy;
    v_world_position = world_position;
    v_world_normal = world_normal;
    
	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); // Output of the vertex shader
    
}
