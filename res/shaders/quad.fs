varying vec2 v_uv;
uniform float shaderType;
uniform sampler2D u_texture;
uniform float u_time;

// 3.1.c
// Compute the smoothed effect of the colors
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
        float fstep = 0.002;
        gl_FragColor = (texture2D(u_texture, vec2(v_uv.x-fstep, v_uv.y-fstep)) + texture2D(u_texture, vec2(v_uv.x+fstep, v_uv.y-fstep)) + texture2D(u_texture, vec2(v_uv.x-fstep, v_uv.y+fstep)) + texture2D(u_texture, vec2(v_uv.x+fstep, v_uv.y+fstep)))/vec4(4.0);
    } else if (shaderType==6.5){
        // 3.2.f
        gl_FragColor = texture_color*(vec4(1.0)-distance(vec2(0.5, 0.5), v_uv.xy)*vec4(1,1,1,1));
    } else if (shaderType==7.5){
        float temp =  4.0 - mod(floor(u_time), 5.0);
        float tempX = v_uv.x - (1.0/pow(2.0,temp))*(floor(v_uv.x/(1.0/pow(2.0,temp))));
        float tempY = v_uv.y - (1.0/pow(2.0,temp))*(floor(v_uv.y/(1.0/pow(2.0,temp))));
        vec2 tempUV = pow(2.0,temp)*vec2(tempX, tempY);
        gl_FragColor = texture2D(u_texture, tempUV);
    } else if (shaderType==8.5){
        // moveR and moveB are the displacement of the colors with respect the original one
        float duration = 0.6, moveR = 0.01, moveB = -0.025;
        float time = mod(u_time, duration); // Get the stage of the current time
        float amplitude = max(sin(time * (3.1415 / duration)), 0.0);

        texture_color = texture2D(u_texture, v_uv);
        vec4 maskR = texture2D(u_texture, v_uv + vec2(moveR * amplitude, 0.0));
        vec4 maskB = texture2D(u_texture, v_uv + vec2(moveB * amplitude, 0.0));
        gl_FragColor = vec4(maskR.r, texture_color.g, maskB.b, 1.0);
    }
    
    /* Zoom in Zomm out
    float duration = 1.2;
    float maxAmplitude = 0.3;
    float time = mod(u_time, duration);
    float amplitude = 1.0 + maxAmplitude * abs(sin(time * (3.1415 / duration)));
    gl_FragColor = texture2D(u_texture, vec2(v_uv.x*amplitude, v_uv.y*amplitude));
    */
    
}
