#include <vector>
#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(const Ray& ray, Hit& hit)
{
    // TODO
    double min_t = std::numeric_limits<double>::max();
    Object* closest_object = 0;
    for(size_t i=0;i<objects.size();i++)
    {
        std::vector<Hit> hits;
        if(objects[i]->Intersection(ray,hits))
        {
            for (size_t j=0;j<hits.size();j++)
            {
                if(hits[j].t < min_t && hits[j].t >= small_t)
                {
                    closest_object = objects[i];
                    hit = hits[j];
                    min_t = hits[j].t;
                }
            }
        }

    }
    return closest_object;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
   
    Ray ray;
    ray.endpoint = camera.position;
    ray.direction = (camera.World_Position(pixel_index)-ray.endpoint);
    ray.direction = ray.direction.normalized();
     // TODO: set up the initial view ray here
    vec3 color=Cast_Ray(ray,recursion_depth_limit);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    // TODO
    Hit hit;
    Object* closest = Closest_Intersection(ray, hit);
    vec3 color;
    vec3 zz;
    if(closest)
    {
        vec3 intPoint = ray.Point(hit.t);
        vec3 normal = closest->Normal(intPoint).normalized();
        // if exiting, reverse normal to same side as ray
        if (hit.ray_exiting)
            normal = -normal;
        color = closest->material_shader->Shade_Surface(
                ray,
                intPoint,
                normal,
                recursion_depth);
    }
    else
    {
        vec3 dummy;
        color = background_shader->Shade_Surface(ray,dummy,dummy,recursion_depth);
    }
    // determine the color here

    return color;
}
