// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;
uniform sampler2D u_colorTexture;
uniform sampler2D u_normalTexture;
uniform vec3 u_Ia, u_Id, u_Is, u_lightPosition, u_Ka, u_Kd, u_Ks, u_eye;
uniform float u_alfa;

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec3 v_Ip;

//here create uniforms for all the data we need here

void main()
{	
	v_uv = gl_MultiTexCoord0.xy;
    
	// Convert local position to world space
	vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;
    
    vec3 N = normalize((u_model * vec4( gl_Normal.xyz, 0.0)).xyz);
    // v_world_normal = world_normal; // N
    vec3 L = normalize(u_lightPosition-world_position);
    vec3 R = reflect(-L, N); // R is already normalized
    vec3 V = normalize(u_eye-world_position);
    
    //
    
    vec3 temp = u_Ka*u_Ia + (clamp(dot(L, N), 0.0, 1.0))*u_Kd*u_Id + u_Ks*(clamp(pow(dot(R,V), u_alfa),0.0, 1.0))*u_Is;
    v_Ip = temp;
    
	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
    
}
