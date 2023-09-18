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

in vec2 v_texCoord;

uniform sampler2D u_texture;
uniform float u_contrast;
uniform float u_brightness;
uniform float u_saturation;
uniform float u_gamma;

//GamaCorrection(gamma)
// color^gamma

void main()
{
    vec4 texColor = texture(u_texture, v_texCoord);
    color = texColor;
    // Contrast & Brightness
    color = clamp((u_contrast * (color - 0.5) + 0.5 + u_brightness), 0.0, 1.0);

    // Saturation
    float grayscale = dot(vec3(color), vec3(0.299, 0.587, 0.114));
    float colorR = mix(grayscale, color.r, u_saturation);
    float colorG = mix(grayscale, color.g, u_saturation);
    float colorB = mix(grayscale, color.b, u_saturation);
    color = vec4( colorR, colorG, colorB, 1);
    color = clamp(color, 0.0, 1.0);

    // Gamma correction
    colorR = pow(color.r, u_gamma);
    colorG = pow(color.g, u_gamma);
    colorB = pow(color.b, u_gamma);
    color = vec4( colorR, colorG, colorB, 1);
    color = clamp(color, 0.0, 1.0);

};
