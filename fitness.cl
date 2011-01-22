

__kernel void computeFitness(
	__global double* cart_position,
	__global double* cart_velocity,
	__global double* cart_acceleration,
	__global double* pole_angle,
	__global double* pole_angle_origin,
	__global double* pole_velocity,
	__global double* pole_acceleration,
	__global long int* c_cart_position,
	__global long int* c_cart_velocity,
	__global long int* c_pole_angle,
	__global long int* c_pole_velocity,
	__global double* force,	
	__global double* fitness
)
{
	int gid = get_global_id(0);
	fitness[gid] = cart_position[gid] + pole_angle[gid] + 1;
}
