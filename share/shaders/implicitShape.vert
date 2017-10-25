attribute vec2 g;
void main() 
{
   gl_Position = vec4(g.xy, 1.0, 1.0);
}
