varying vec3 v,EP;

void main(){
  gl_Position=gl_Vertex;
  gl_TexCoord[0]=gl_MultiTexCoord0; 
  v = vec3( gl_ModelViewMatrix*gl_Vertex);
  EP= vec3( gl_ModelViewMatrix*vec4(0,0,1,1) );
}

