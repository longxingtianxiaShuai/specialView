static const char* SimpleVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 DiffuseMaterial;
attribute vec2 TextureCoord;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform vec3 LightPosition;
uniform vec3 AmbientMaterial;
uniform vec3 SpecularMaterial;
uniform float Shininess;
uniform float DiffuseAlpha;

uniform lowp int  RenderMode;

varying vec4 DestinationColor;
varying vec2 TextureCoordOut;

void main(void)
{
    vec3 L = normalize(LightPosition);
    vec3 E = vec3(0, 0, 1);
    vec3 H = normalize(L + E);

    float df = max(0.0, dot(Normal, L) * 1.3);
    float sf = max(0.0, dot(Normal, H));
    sf = pow(sf, Shininess);

    if(RenderMode<2)
    {
        vec3 color = AmbientMaterial + df * DiffuseMaterial + sf * SpecularMaterial;
        //*1.225
        DestinationColor = vec4(color, DiffuseAlpha);
    }
    else
    {
        DestinationColor = vec4(DiffuseMaterial, DiffuseAlpha);
    }
    gl_Position = Projection * Modelview * Position;
    TextureCoordOut = TextureCoord;
}
);
