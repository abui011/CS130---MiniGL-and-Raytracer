#include "sphere.h"
#include "ray.h"
#include <math.h>


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    vec3 w = ray.direction;
    vec3 v = ray.endpoint- this->center;
    double r = radius;
    double d = pow(dot(w,v),2)-(dot(w,w)*(dot(v,v)-r*r));
    
    if (d > 0)
    {
        double t1 = (-dot(w,v)+sqrt(d));
        Hit h1 = {this,t1,true};
    	double t2 = (-dot(w,v)-sqrt(d));
    	Hit h2 = {this,t2,false};
        if (t1 >= 0)
            hits.push_back(h1);
    	if (t2 >= 0)
    	   hits.push_back(h2);
    	return true;
    }

    return false;

}
//w = raydirection
//v = rayendpoint
//r = sphere radius symbol
vec3 Sphere::Normal(const vec3& point) const
{
    vec3 normal;
    // TODO: set the normal
    normal = (point - center).normalized();
    return normal;
    
}
