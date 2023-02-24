varying vec2 v_uv;

void main()
{
    gl_FragColor = vec4(mix(vec4(0,0,1,1), vec4(1, 0, 0, 1), v_uv.x));
}
