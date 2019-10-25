#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

//uniform sampler2D ourTexture;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1,TexCoord),texture(texture2,TexCoord),0.2);
    //FragColor = texture(ourTexture,TexCoord);
    //FragColor = vec4(ourColor, 1.0f);
}

