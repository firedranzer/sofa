uniform vec2 resolution;
uniform vec2 mouse;
uniform float wheelDelta;
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
		vec4(sqrt(x*x+y*y+z*z) -.5, evalColorSphere1)
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
        depth += res.x;
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
    vec3 col = vec3(1.0, 1.0, 1.0) + rd.y;
    vec4 res = castRay(ro,rd);
    float t = res.x;
    vec3 m = res.yzw;
    vec3 pos = ro + t * rd;
    vec3 nor = estimateNormal( pos );
    vec3 ref = reflect( rd, nor );
    col = m;
    vec3  lig = normalize( vec3(-0.4, 0.7, -0.6) );
    float amb = clamp( 0.5+0.5*nor.y, 0.0, 1.0 );
    float dif = clamp( dot( nor, lig ), 0.0, 1.0 );
    float bac = clamp( dot( nor, normalize(vec3(-lig.x,0.0,-lig.z))), 0.0, 1.0 )*clamp( 1.0-pos.y,0.0,1.0);
    float dom = smoothstep( -0.1, 0.1, ref.y );
    float fre = pow( clamp(1.0+dot(nor,rd),0.0,1.0), 2.0 );
    float spe = pow(clamp( dot( ref, lig ), 0.0, 1.0 ),16.0);
    dif *= softshadow( pos, lig, 0.02, 2.5 );
    dom *= softshadow( pos, ref, 0.02, 2.5 );
    vec3 lin = vec3(0.0);
    lin += 1.30*dif*vec3(1.00,0.80,0.55);
    lin += 2.00*spe*vec3(1.00,0.90,0.70)*dif;
    lin += 0.40*amb*vec3(0.40,0.60,1.00);
    lin += 0.50*dom*vec3(0.40,0.60,1.00);
    col = col*lin;
    col = mix( col, vec3(0.9,0.9,1.0), 1.0-exp( -0.0002*t*t*t ) );
    return vec3( clamp(col,0.0,1.0) );
}

mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

void main()
{
    vec2 mouseR = mouse.xy/resolution.xy;
    vec2 p = (-resolution.xy + 2.0*gl_FragCoord.xy)/resolution.y;
    vec3 ro = vec3( 4.0 * cos(7.0*mouseR.x) * (wheelDelta), 4.0*mouseR.y, 4.0*sin(7.0*mouseR.x) * (wheelDelta) );
    vec3 ta = vec3( 0.0, 0.0, 0.0 );
    mat3 ca = setCamera( ro, ta, 0.0 );
    vec3 rayDir = ca * normalize( vec3(p.xy, 2.0) );
    gl_FragColor = vec4( render( ro, rayDir ), 1.0 );
}

