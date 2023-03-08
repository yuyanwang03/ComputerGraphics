varying vec2 v_uv;
uniform float shaderType;
uniform sampler2D u_texture;

// 3.1.c
// 色值大小计算 Compute the smoothed effect of the colors
float v(in vec2 uv, float d, float o){
     return 1.0-smoothstep(0.0,1.,abs(sin((uv.x-0.1) * 15.)))-0.1;
}
float w(in vec2 uv, float d, float o){
     return 1.0-smoothstep(0.0,1.,abs(sin((uv.y-0.1) * 15.)));
}

// 3.1.f
vec3 board(vec2 uv, float rows, float cols){
    float t = (floor(v_uv.x*cols)+floor(v_uv.y*rows));
    return vec3(mod(t,2.0)<1.0);
}

void main()
{
    vec4 texture_color;
    
    if (shaderType==1.0){
        // 3.1.a
        gl_FragColor = vec4(mix(vec4(0,0,1,1), vec4(1, 0, 0, 1), v_uv.x));
    } else if (shaderType==2.0){
        // 3.1.b
        gl_FragColor = distance(vec2(0.5, 0.5), v_uv.xy)*vec4(1,1,1,1);
    } else if (shaderType==3.0){
        // 3.1.c
        float d1 =  v_uv.x;
        float d2 = v_uv.y;
        gl_FragColor = vec4(v(v_uv, d1, 1.0), 0.0, 0.0, 0.1) + vec4(0.0, 0.0, w(v_uv, d2, 1.0), 0.1); // red + blue
    } else if (shaderType==4.0){
        // 3.1.d
        gl_FragColor = vec4(floor(v_uv.x*10.0)/10.0, floor(v_uv.y*10.0)/10.0, 0.0, 1.0);
    } else if (shaderType==5.0){
        // 3.1.e
        gl_FragColor = vec4((v_uv.y < 0.5+0.25*sin(v_uv.x*2.0*3.1415)))*vec4(mix(vec4(0,0,0,1), vec4(0, 1, 0, 1), v_uv.y)) + vec4((v_uv.y > 0.5+0.25*sin(v_uv.x*2.0*3.1415)))*vec4(mix(vec4(0,1,0,1), vec4(0, 0, 0, 1), v_uv.y));
    } else if (shaderType==6.0){
        // 3.1.f
        gl_FragColor = vec4(board(v_uv, 20.0, 20.0),1);
    } else{
        // Load texture (image)
        texture_color = texture2D(u_texture, v_uv);
    }
    
    if (shaderType==1.5){
        // 3.2.a
        gl_FragColor = vec4(vec3((texture_color.r+texture_color.g+texture_color.b)/3.0), 1.0);
    } else if (shaderType==2.5){
        // 3.2.b
        gl_FragColor  = vec4(vec3(1.0)-texture_color.rgb, 1.0);
    } else if (shaderType==3.5){
        // 3.2.c
        gl_FragColor = vec4(1.0-texture_color.b, texture_color.r, 0.5*texture_color.g, 1.0);
    } else if (shaderType==4.5){
        // 3.2.d
        gl_FragColor = vec4(vec3(step(0.5, (texture_color.r+texture_color.g+texture_color.b)/3.0)), 1.0);
    } else if (shaderType==5.5){
        // 3.2.e
        
    } else if (shaderType==6.5){
        // 3.2.f
    }
    
    // 3.2.e
    // texture_color = vec4(1.0);
    // texture_color = vec4(texture_color.rbb, 0.0);
    
    //gl_FragColor = texture_color*vec4(0.2);
    
}
