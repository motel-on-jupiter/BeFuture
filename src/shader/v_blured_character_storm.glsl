#version 120

void main(void) {
  gl_Position = gl_Vertex;
  gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
  gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord0;
}