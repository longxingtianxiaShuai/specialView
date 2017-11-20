static const char* SimpleFragmentShader = STRINGIFY(

varying lowp vec4 DestinationColor;
varying mediump vec2 TextureCoordOut;

uniform sampler2D Sampler;
uniform lowp int  RenderMode;

void main(void)
{
    if(RenderMode==0)
        gl_FragColor = texture2D(Sampler, TextureCoordOut) * DestinationColor;
    else if(RenderMode==2)
    {
        gl_FragColor = texture2D(Sampler, TextureCoordOut);
    }
    else
        gl_FragColor = DestinationColor;
}
);
