#include "plane.h"
#include "ray.h"
#include <cfloat>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    double dotp = dot(this->normal, ray.direction);
    double p = dot(this->normal, ray.endpoint-this->x1)/dotp;
    Hit temp;
    bool btemp;
    //endpoint on plane
    if(dotp)
    {
        if(p == 0)
        {
            if (dotp < 0)
                btemp = false;
            else
                btemp = true;
            temp = {this,0,btemp};
            hits.push_back(temp);
            return true;
        }
        else
        {
            if (-p >= 0)
            {
                if (dotp < 0)
                    btemp = false;
                 else
                    btemp = true;
                temp = {this,-p,btemp};
                hits.push_back(temp);
                return true;
            }
        }
    }
    return false;
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}
