uniform vec2 resolution;
uniform vec2 mouse;
uniform float wheelDelta;
uniform vec3 position;
uniform vec3 lookAt;
uniform vec4 rotation;
uniform float fov;

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.00001;

uniform vec3 evalColorSphere1;
uniform vec3 evalPositionSphere1;
uniform vec3 evalColorSphere2;
uniform vec3 evalPositionSphere2;
uniform vec3 evalColorSphere3;
uniform vec3 evalPositionSphere3;

float sdPlane( vec3 p )
{
   return p.y;
}
float sdSphere( vec3 p, float s )
{
   return length(p)-s;
}

vec4 minVec4( vec4 d1, vec4 d2 )
{
   return (d1.x<d2.x) ? d1 : d2;
}

vec4 map( in vec3 pos )
{
   float x = pos.x;
   float y = pos.y;
   float z = pos.z;
   vec4 res = vec4(sdPlane(pos), vec3(0.45, 0.45, 0.45));
   x = pos.x - evalPositionSphere1.x;
   y = pos.y - evalPositionSphere1.y;
   z = pos.z - evalPositionSphere1.z;
   res = minVec4(
       res,
		vec4(min(sqrt(x*x+y*y+z*z) -1.0, sqrt((x+.5)*(x+0.5)+y*y+z*z) -1.0), evalColorSphere1)
   );    
   x = pos.x - evalPositionSphere2.x;
   y = pos.y - evalPositionSphere2.y;
   z = pos.z - evalPositionSphere2.z;
   res = minVec4(
       res,
		vec4(sqrt(x*x+y*y+z*z) -1.0, evalColorSphere2)
   );    
   x = pos.x - evalPositionSphere3.x;
   y = pos.y - evalPositionSphere3.y;
   z = pos.z - evalPositionSphere3.z;
   res = minVec4(
       res,
		vec4(sqrt(x*x+y*y+z*z) -1.0, evalColorSphere3)
   );    
   return res;
}

vec4 castRay( in vec3 ro, in vec3 rd )
{
   float depth = MIN_DIST;
   vec3 m = vec3(-1.0, -1.0, -1.0);
   for( int i = 0; i < MAX_MARCHING_STEPS; i++ )
   {
       vec4 res = map( ro+rd*depth );
       if( res.x < EPSILON || depth > MAX_DIST ) break;
       depth += res.x*0.2;
           m = res.yzw;
   }
   return vec4( depth, m );
}

float softshadow( in vec3 ro, in vec3 rd, in float mint, in float tmax )
{
   float res = 1.0;
   float t = mint;
   for( int i=0; i<16; i++ )
   {
      float h = map( ro + rd*t ).x;
      res = min( res, 8.0*h/t );
      t += clamp( h, 0.02, 0.10 );
      if( h<0.001 || t>tmax ) break;
   }
   return clamp( res, 0.0, 1.0 );
}

vec3 estimateNormal(vec3 p) {
   return normalize(vec3(
       map(vec3(p.x + EPSILON, p.y, p.z)).x - map(vec3(p.x - EPSILON, p.y, p.z)).x,
       map(vec3(p.x, p.y + EPSILON, p.z)).x - map(vec3(p.x, p.y - EPSILON, p.z)).x,
       map(vec3(p.x, p.y, p.z  + EPSILON)).x - map(vec3(p.x, p.y, p.z - EPSILON)).x
   ));
}

vec3 render( in vec3 ro, in vec3 rd )
{
   vec4 res = castRay(ro,rd);
   if (res.x > MAX_DIST) return vec3(0.0, 0.0, 0.0)
;
   float dist = res.x;
   vec3 col = res.yzw;
   vec3 pos = ro + dist * rd;
   vec3 nor = estimateNormal( pos );
   vec3 ref = reflect( rd, nor );

   vec3  lig = normalize(ro);
   float amb = clamp( 0.5+0.5*nor.y, 0.0, 1.0 );
   float dif = clamp( dot( nor, lig ), 0.0, 1.0 );
   float dom = smoothstep( -0.1, 0.1, ref.y );
   float spe = pow(clamp( dot( ref, lig ), 0.0, 1.0 ),16.0);
   dif *= softshadow( pos, lig, 0.02, 2.5 );
   dom *= softshadow( pos, ref, 0.02, 2.5 );

   vec3 lin = vec3(0.0);
   lin += 1.30*dif*vec3(1.00,0.80,0.55);
   lin += 2.00*spe*vec3(1.00,0.90,0.70)*dif;
   lin += 0.40*amb*vec3(0.40,0.60,1.00);
   lin += 0.50*dom*vec3(0.40,0.60,1.00);
   col = col*lin;

   return vec3( clamp(col,0.0,1.0) );
}

vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
   vec2 xy = fragCoord - size / 2.0;
   float z = size.y / tan(radians(fieldOfView));
   return normalize(vec3(xy, -z));
}

mat3 viewMatrix(vec3 eye, vec3 center, vec3 up) {
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);
    return mat3(s, u, -f);
}

void main()
{
   vec3 viewDir = rayDirection(fov, resolution.xy, gl_FragCoord.xy);
   mat3 viewToWorld = viewMatrix(position, lookAt, rotation.xyz);
   vec3 worldDir = viewToWorld * viewDir;
   gl_FragColor = vec4( render( position, worldDir ), 1.0 );
}

