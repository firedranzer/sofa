class Vec3d
{
public:
	double x{0};
	double y{0};
	double z{0};
	Vec3d(){}
	Vec3d(double ax,double ay, double az){
		x=ax;
		y=ay;
		z=az;
	}
	
	void set(double ax, double ay, double az)
	{
		x = ax;
		y = ay;
		z = az;
	}
	
	Vec3d operator+(Vec3d& v){
		return Vec3d(x+v.x, y+v.y, z+v.z);
	}
	
	Vec3d operator-(Vec3d& v){
		return Vec3d(x-v.x, y-v.y, z-v.z);
	}
	
	double length(){
		return sqrt(x*x+y*y+z*z) ;
	}
};
