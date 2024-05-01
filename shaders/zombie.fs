uniform sampler2D texture0;

void main()
{
    vec2 fragTexCoord = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(texture0, fragTexCoord);
    float gray = (texColor.r + texColor.g + texColor.b);
    gl_FragColor = vec4(gray, gray, gray, texColor.a);
}
