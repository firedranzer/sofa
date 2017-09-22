uniform vec3 color;
uniform vec2 resolution;
uniform vec2 mouse;

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.00001;

mat3 rotateX(float theta) {
	float c = cos(theta);
	float s = sin(theta);
	return mat3(
		vec3(1, 0, 0),
		vec3(0, c, -s),
		vec3(0, s, c)
	);
}

mat3 rotateY(float theta) {
	float c = cos(theta);
	float s = sin(theta);
	return mat3(
		vec3(c, 0, s),
		vec3(0, 1, 0),
		vec3(-s, 0, c)
	);
}

float sceneSDF(vec3 p) {

	p = rotateY(10.*mouse.x/resolution.x) * p;
	p = -rotateX(10.*mouse.y/resolution.y) * p;
	float x = p.x;
	float y = p.y;
	float z = p.z;

	vec3 h = vec3(1.0, 2.0, 1.0);



	vec3 q = abs(p);
	return sqrt(min(((1.0*x-0.0*(z-0.0))/1.0)*((1.0*x-0.0*(z-0.0))/1.0)+((0.0*(x-0.0)+1.0*(y-0.0))/1.0)*((0.0*(x-0.0)+1.0*(y-0.0))/1.0)+((0.0*x+1.0*(z-0.0))/1.0)*((0.0*x+1.0*(z-0.0))/1.0)-1.0,abs((0.0*x+1.0*(z-0.0))/1.0)+sqrt(((1.0*x-0.0*(z-0.0))/1.0)*((1.0*x-0.0*(z-0.0))/1.0)+((0.0*(x-0.0)+1.0*(y-0.0))/1.0)*((0.0*(x-0.0)+1.0*(y-0.0))/1.0))-1.0)) -1.;

}

float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) {
	float depth = start;
	for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
		float dist = sceneSDF(eye + depth * marchingDirection);
		if (dist < EPSILON) {
			return depth;
		}
		depth += dist;
		if (depth >= end) {
			return end;
		}
	}
	return end;
}

vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
	vec2 xy = fragCoord - size / 2.0;
	float z = size.y / tan(radians(fieldOfView) / 2.0);
	return normalize(vec3(xy, -z));
}

vec3 estimateNormal(vec3 p) {
	return normalize(vec3(
	sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
	sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
	sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
	));
}

vec3 phongContribForLight(vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye, vec3 lightPos, vec3 lightIntensity) {
	vec3 N = estimateNormal(p);
	vec3 L = normalize(lightPos - p);
	vec3 V = normalize(eye - p);
	vec3 R = normalize(reflect(-L, N));

	float dotLN = dot(L, N);
	float dotRV = dot(R, V);

	if (dotLN < 0.0) {
		// Light not visible from this point on the surface
		return vec3(0.0, 0.0, 0.0);
	}

	if (dotRV < 0.0) {
		// Light reflection in opposite direction as viewer, apply only diffuse
		// component
		return lightIntensity * (k_d * dotLN);
	}
	return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

vec3 phongIllumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye) {
	const vec3 ambientLight = 0.5 * vec3(1.0, 1.0, 1.0);
	vec3 color = ambientLight * k_a;
	vec3 light1Pos = vec3(4.0, 2.0, 4.0);
	vec3 light1Intensity = vec3(0.4, 0.4, 0.4);

	color += phongContribForLight(k_d, k_s, alpha, p, eye,
	light1Pos,
	light1Intensity);
	return color;
}

mat3 viewMatrix(vec3 eye, vec3 center, vec3 up) {
	// Based on gluLookAt man page
	vec3 f = normalize(center - eye);
	vec3 s = normalize(cross(f, up));
	vec3 u = cross(s, f);
	return mat3(s, u, -f);
}

void main()
{
	vec3 viewDir = rayDirection(90.0, resolution, gl_FragCoord.xy);
	vec3 eye = vec3(1.0, 1.0 , 7.0);

	mat3 viewToWorld = viewMatrix(eye, vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

	vec3 worldDir = viewToWorld * viewDir;

	float dist = shortestDistanceToSurface(eye, worldDir, MIN_DIST, MAX_DIST);

	if (dist > MAX_DIST - EPSILON) {
		// Didn't hit anything
		gl_FragColor = vec4(.0, 0.0, 0.0, 0.0);
		return;
	}

	// The closest point on the surface to the eyepoint along the view ray
	vec3 p = eye + dist * worldDir;

	// Use the surface normal as the ambient color of the material
	vec3 K_a = (estimateNormal(p) + vec3(1.0)) / 2.0;
	vec3 K_d = K_a;
	vec3 K_s = vec3(1.0, 1.0, 1.0);
	float shininess = 10.0;

	vec3 color = phongIllumination(K_a, K_d, K_s, shininess, p, eye);

	gl_FragColor = vec4(color, 1.0);
}
