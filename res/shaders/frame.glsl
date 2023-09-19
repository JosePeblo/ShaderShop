#shader vertex
#version 460 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

void main()
{
   gl_Position = vec4( position.x, position.y, 0.0, 1.0 );
   v_texCoord = texCoord;
};

#shader fragment
#version 460 core
layout(location = 0) out vec4 color;

uniform sampler2D u_texture;
uniform int u_kernelSize;

in vec2 v_texCoord;

void main()
{
    color = texture(u_texture, v_texCoord);
};

// void main()
// {
//     vec2 Resolution = vec2(640, 640);
// 
//     vec2 src_size = vec2(1.0 / Resolution.x, 1.0 / Resolution.y);
//     float n = float((u_kernelSize + 1) * (u_kernelSize + 1));
//     int i;
//     int j;
//     vec3 m0 = vec3(0.0); vec3 m1 = vec3(0.0); vec3 m2 = vec3(0.0); vec3 m3 = vec3(0.0); 
//     vec3 s0 = vec3(0.0); vec3 s1 = vec3(0.0); vec3 s2 = vec3(0.0); vec3 s3 = vec3(0.0); 
//     vec3 c;
// 
//     for(int j = -u_kernelSize; j <= 0; ++j)
//     {
//         for(int i = -u_kernelSize; i <= 0; ++i)
//         {
//             c = texture(u_texture, v_texCoord + vec2(i,j) * src_size).rgb;
//             m0 += c;
//             s0 += c * c;
//         }
//     }
// 
//     for(int j = -u_kernelSize; j <= 0; ++j)
//     {
//         for(int i = 0; i <= u_kernelSize; ++i)
//         {
//             c = texture(u_texture, v_texCoord + vec2(i,j) * src_size).rgb;
//             m1 += c;
//             s1 += c * c;
//         }
//     }
// 
//     for(int j = 0; j <= u_kernelSize; ++j)
//     {
//         for(int i = 0; i <= u_kernelSize; ++i)
//         {
//             c = texture(u_texture, v_texCoord + vec2(i,j) * src_size).rgb;
//             m2 += c;
//             s2 += c * c;
//         }   
//     }
// 
//     for(int j = 0; j <= u_kernelSize; ++j)
//     {
//         for(int i = -u_kernelSize; i <= 0; ++i)
//         {
//             c = texture(u_texture, v_texCoord + vec2(i,j) * src_size).rgb;
//             m3 += c;
//             s3 += c * c;
//         }
//     }
// 
//     float min_sigma2 = 1e+2;
// 
//     m0 /= n;
//     s0 = abs(s0 / n - m0 * m0);
// 
//     float sigma2 = s0.r + s0.g + s0.b;
//     if(sigma2 < min_sigma2)
//     {
//         min_sigma2 = sigma2;
//         color = vec4(m0, 1.0);
//     }
// 
//     m1 /= n;
//     s1 = abs(s1 / n - m1 * m1);
// 
//     sigma2 = s1.r + s1.g + s1.b;
//     if(sigma2 < min_sigma2)
//     {
//         min_sigma2 = sigma2;
//         color = vec4(m1, 1.0);
//     }
// 
//     m2 /= n;
//     s2 = abs(s2 / n - m2 * m2);
// 
//     sigma2 = s2.r + s2.g + s2.b;
//     if(sigma2 < min_sigma2)
//     {
//         min_sigma2 = sigma2;
//         color = vec4(m2, 1.0);
//     }
// 
//     m3 /= n;
//     s3 = abs(s3 / n - m3 * m3);
// 
//     sigma2 = s3.r + s3.g + s3.b;
//     if(sigma2 < min_sigma2)
//     {
//         min_sigma2 = sigma2;
//         color = vec4(m3, 1.0);
//     }
// 
// };

// uniform sampler2D u_texture;
// uniform float u_contrast;
// uniform float u_brightness;
// uniform float u_saturation;
// uniform float u_gamma;
// 
// 
// 
// 
// 
// void main()
// {
//     vec4 texColor = texture(u_texture, v_texCoord);
//     color = texColor;
//     // Contrast & Brightness
//     color = clamp((u_contrast * (color - 0.5) + 0.5 + u_brightness), 0.0, 1.0);
// 
//     // Saturation
//     float grayscale = dot(vec3(color), vec3(0.299, 0.587, 0.114));
//     float colorR = mix(grayscale, color.r, u_saturation);
//     float colorG = mix(grayscale, color.g, u_saturation);
//     float colorB = mix(grayscale, color.b, u_saturation);
//     color = vec4( colorR, colorG, colorB, 1);
//     color = clamp(color, 0.0, 1.0);
// 
//     // Gamma correction
//     colorR = pow(color.r, u_gamma);
//     colorG = pow(color.g, u_gamma);
//     colorB = pow(color.b, u_gamma);
//     color = vec4( colorR, colorG, colorB, 1);
//     color = clamp(color, 0.0, 1.0);
// 
// };
