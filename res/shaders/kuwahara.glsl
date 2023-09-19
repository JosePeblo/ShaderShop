#version 460 core
//layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in; // WARP optimization
layout(rgba32f, binding = 0) uniform image2D screen;

uniform sampler2D u_input;

void main()
{
    //ivec2 pixel_coords = ivec2(gl_LocalInvocationID.xy);
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy); // WARP optimization
    ivec2 dims = imageSize(screen);
    vec2 uvs = 

    vec2 Resolution = vec2(640, 640);

    vec2 src_size = vec2(1.0 / Resolution.x, 1.0 / Resolution.y);
    float n = float((u_kernelSize + 1) * (u_kernelSize + 1));
    int i;
    int j;
    vec3 m0 = vec3(0.0); vec3 m1 = vec3(0.0); vec3 m2 = vec3(0.0); vec3 m3 = vec3(0.0); 
    vec3 s0 = vec3(0.0); vec3 s1 = vec3(0.0); vec3 s2 = vec3(0.0); vec3 s3 = vec3(0.0); 
    vec3 c;

    for(int j = -u_kernelSize; j <= 0; ++j)
    {
        for(int i = -u_kernelSize; i <= 0; ++i)
        {
            c = texture(u_texture, v_texCoord + vec2(i,j) * src_size).rgb;
            m0 += c;
            s0 += c * c;
        }
    }

    for(int j = -u_kernelSize; j <= 0; ++j)
    {
        for(int i = 0; i <= u_kernelSize; ++i)
        {
            c = texture(u_texture, v_texCoord + vec2(i,j) * src_size).rgb;
            m1 += c;
            s1 += c * c;
        }
    }

    for(int j = 0; j <= u_kernelSize; ++j)
    {
        for(int i = 0; i <= u_kernelSize; ++i)
        {
            c = texture(u_texture, v_texCoord + vec2(i,j) * src_size).rgb;
            m2 += c;
            s2 += c * c;
        }   
    }

    for(int j = 0; j <= u_kernelSize; ++j)
    {
        for(int i = -u_kernelSize; i <= 0; ++i)
        {
            c = texture(u_texture, v_texCoord + vec2(i,j) * src_size).rgb;
            m3 += c;
            s3 += c * c;
        }
    }

    float min_sigma2 = 1e+2;

    m0 /= n;
    s0 = abs(s0 / n - m0 * m0);

    float sigma2 = s0.r + s0.g + s0.b;
    if(sigma2 < min_sigma2)
    {
        min_sigma2 = sigma2;
        color = vec4(m0, 1.0);
    }

    m1 /= n;
    s1 = abs(s1 / n - m1 * m1);

    sigma2 = s1.r + s1.g + s1.b;
    if(sigma2 < min_sigma2)
    {
        min_sigma2 = sigma2;
        color = vec4(m1, 1.0);
    }

    m2 /= n;
    s2 = abs(s2 / n - m2 * m2);

    sigma2 = s2.r + s2.g + s2.b;
    if(sigma2 < min_sigma2)
    {
        min_sigma2 = sigma2;
        color = vec4(m2, 1.0);
    }

    m3 /= n;
    s3 = abs(s3 / n - m3 * m3);

    sigma2 = s3.r + s3.g + s3.b;
    if(sigma2 < min_sigma2)
    {
        min_sigma2 = sigma2;
        color = vec4(m3, 1.0);
    }

};

