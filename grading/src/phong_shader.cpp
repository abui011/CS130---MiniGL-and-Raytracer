#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"


vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth) const
{
    vec3 color;
    //vec3 L, /*R*/h, light_ambient, light_diffuse, light_specular, light_color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 light_color;
    vec3 L;
    vec3 h;
    // ambient light is calculated independent of light source
    ambient = color_ambient * (world.ambient_intensity * world.ambient_color);
    
    for (unsigned i = 0 ; i < world.lights.size(); ++i) {
        // check if light source is obstructed by object
        // diffuse
        L = (world.lights.at(i)->position - intersection_point);
        light_color = (world.lights.at(i)->Emitted_Light(ray) / pow(L.magnitude(),2));
        double ldotn = dot(same_side_normal, L.normalized());
        if (ldotn < 0)
            ldotn = 0;
        diffuse = (color_diffuse * ldotn * light_color); 
        
        // specular
        vec3 R = (2 * dot(L.normalized(),same_side_normal) * same_side_normal - L.normalized()).normalized();
        vec3 V = (ray.endpoint - intersection_point);
        double rdotv = dot(R,V.normalized());
        if(rdotv <0)
            rdotv = 0;
        specular = pow(rdotv, specular_power) * color_specular * light_color;
       
        if (world.enable_shadows) {
            Ray shadow_ray(intersection_point, world.lights.at(i)->position - intersection_point);
            Hit h;
            Object *obstruction = world.Closest_Intersection(shadow_ray, h);
            // there exists an object that casts a shadow here
            if (obstruction != 0) {
                // no contribution of diffuse and specular light
                continue;
            }
        }

        color = color + diffuse + specular;
        
    }
    color = color + ambient;
        
    //std::cout << "Wow light: "<< color << std::endl;
    return color;
}