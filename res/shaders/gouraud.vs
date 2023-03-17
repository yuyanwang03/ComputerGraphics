// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;
uniform sampler2D u_colorTexture;
uniform sampler2D u_normalTexture;
uniform vec3 u_Ia;//, u_Id, u_Is, u_lightPosition, u_Ka, u_Kd, u_Ks;
uniform vec3 u_Ka;

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec3 u_Ip;

//here create uniforms for all the data we need here

void main()
{	
	v_uv = gl_MultiTexCoord0.xy;

	// Convert local position to world space
	vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;
    
    vec3 temp = u_Ka*u_Ia + vec3(0.56, 0.5, 0.6);
    u_Ip = temp;
    
	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
    
}
