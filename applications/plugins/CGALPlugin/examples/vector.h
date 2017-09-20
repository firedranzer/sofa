#include <cmath>
#include <algorithm>
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
	
	Vec3d mul(const double a) const {
		return Vec3d(x*a, y*a, z*a);
	}
	
	Vec3d div(const double a) const {
		return Vec3d(x/a, y/a, z/a);
	}

	Vec3d operator+(const Vec3d& v)const{
		return Vec3d(x+v.x, y+v.y, z+v.z);
	}
	
	Vec3d operator-(const Vec3d& v)const{
		return Vec3d(x-v.x, y-v.y, z-v.z);
	}
	
	double length(){
		return sqrt(x*x+y*y+z*z) ;
	}

    	Vec3d vmax(const Vec3d& v)const {
            return Vec3d(std::max(x, v.x),std::max(y, v.y),std::max(z, v.z) );
        }

 		Vec3d  mod(const Vec3d& v)const {
            return Vec3d( fmod(x, v.x), fmod(y, v.y), fmod(z, v.z) );
        }


		Vec3d vmin(const Vec3d& v)const {
            return Vec3d(std::min(x, v.x),std::min(y, v.y),std::min(z, v.z) );
        }

  		Vec3d vmax(const double& v)const {
            return Vec3d(std::max(x, v),std::max(y, v),std::max(z, v) );
        }

		Vec3d vmin(const double& v)const {
            return Vec3d( std::min(x, v), std::min(y, v), std::min(z, v) );
        }


Vec3d abs()const {
            return Vec3d( fabs(x), fabs(y), fabs(z) );
        }
};
