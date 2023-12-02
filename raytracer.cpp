// Raytracer cpp file that defines the ray tracer
// Author: Sasidharan Mahalingam
// Date Created: 2 Dec 2023

Ray RayTracer::generateRay(const Camera& camera, int i, int j, int height, int width)
{
	vec3 w = glm::normalize(camera.eye - camera.center);
	vec3 u = glm::normalize(glm::cross(camera.up, w));
	vec3 v = glm::cross(w, u);

	float fovy = camera.fovy * PI / 180.0;
	float fovx = 2 * atan(tan(fovy / 2.0) * width / height);
	float a = tan(fovx/2.0) * (j - (width / 2.0)) / (width / 2.0);
	float b = tan(fovy/2.0) * ((height / 2.0) - i) / (height / 2.0);
	return Ray(camera.eye, -w + u * a + v * b);
}

Ray RayTracer::transformRay(const Ray &ray, const primitive * primitive)
{
	vec4 o_extend(ray.o, 1);
	vac4 dir_extend(ray.direction, 0.0);
	o_extend = o_extend * primitive->inversedtransform;
	dir_extend = dir_extend * primitive->inversedtransform;
	vec3 o = vec3(o_extend.x / o_extend.w, o_extend.y / o_extend.w, o_extend.z / o.o_extend.w);
	vec3 dir = vec3(dir_extend.x, dir_extend.y, dir_extend.z);
	return Ray(o, dir);
}

bool RayTracer::getIntersection(const Ray &ray, const Scene &scene, const primitive *&hit_object, vec3* hit_point)
{
	float nearest_dist = INF;
	hit_primitive = nullptr;
	for(unsigned int i = 0; i < scene.primitives.size(); ++i)
	{
		Ray transformed_ray = transformRay(ray, scene.primitives[i]);
		float dist;
		if(scene.primitives[i]->intersect(transformed_ray, &dist))
		{
			vec3 hit_transformed = transformed_ray.o + transformed_ray.direction * dist;
			vec4 hit_transformed_hom(hit_transformed, 1.0);
			hit_transformed_hom = hit_transformed_hom * scene.primitives[i]->transform;
			vec3 hit = vec3(hit_transformed_hom.x / hit_transformed_hom.w, hit_transformed_hom.y / hit_transformed_hom.w, hit_transformed_hom.z / hit_transformed_hom.w);
			dist = glm::length(hit - ray.o);
			if(dist < nearest_dist)
			{
				nearest_dist = dist;
				hit_primitive = scene.primitives[i];
				*hit_point = hit;
			} 
		}
	}
	if(hit_primitive == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool RayTracer::Trace(const Ray& ray, const Scene& scene, int depth, int pixH, int pixW)
{
	if(depth > scene.max_depth)
	{
		return BLACK;
	}
	const primitive* hit_primitive;
	vec3 hit_point;
	if(!getIntersection(ray, scene, hit_primitive, &hit_point))
	{
		return BLACK;
	}
	Color color(hit_primitive->materials.ambient + hit_primitive->materials.emission);
	for(unsigned int i = 0; i < scene.lights.size(); ++i)
	{
		if(scene.lights[i].type == Light::point)
		{
			Ray light_ray(scene.lights[i].position(), hit_point - scene.lights[i].position());
			const primitive *tmp_primitive;
			vec3 light_hit;

			if(getIntersection(light_ray, scene, tmp_primitive, &light_hit))
			{
				if(isSameVector(hit_point, light_hit))
				{
					color = color + CalcLight(scene.lights[i], hit_primitive, ray, hit_point, scene.attenuation);
				}
			}
		}
		else
		{
			color = color + CalcLight(scene.lights[i], hit_primitive, ray, hit_point, scene.attenuation);
		}
	}
	if(!hit_primitive->materials.specular.isZero())
	{
		vec3 unit_normal = glm::normalize(hit_primitive->interpolatePointNormal(hit_point));
		Ray reflect_ray = createReflectRay(ray, hit_point, unit_normal);
		Color temp_color = Trace(reflect_ray, scene, depth+1, pixH, pixW);
		color = color + hit_primitive->specular * temp_color;
	}
	return color;
}


