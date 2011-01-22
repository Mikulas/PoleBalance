/*
 *  opencl.h
 *  PoleBalance
 *
 *  Created by Mikulas Dite on 22/01/11.
 *  Copyright 2010 GChD. All rights reserved.
 *
 */

#include <assert.h>
#include <sys/sysctl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#include <OpenCL/OpenCL.h>

char * load_program_source(const char *filename)
{
	struct stat statbuf;
	FILE *fh;
	char *source;

	fh = fopen(filename, "r");
	if (fh == 0)
		return 0;

	stat(filename, &statbuf);
	source = (char *) malloc(statbuf.st_size + 1);
	fread(source, statbuf.st_size, 1, fh);
	source[statbuf.st_size] = '\0';

	return source;
}

// Main OpenCL Routine
int getEntityFitnessAccelerated(
	double* cart_position,
	double* cart_velocity,
	double* cart_acceleration,
	double* pole_angle,
	double* pole_angle_origin,
	double* pole_velocity,
	double* pole_acceleration,
	double* force,
	double* fitness,
	long int* c_cart_position,
	long int* c_cart_velocity,
	long int* c_pole_angle,
	long int* c_pole_velocity
)
{
	cl_program program[1];
	cl_kernel kernel[2];
	
	cl_command_queue cmd_queue;
	cl_context context;
	
	cl_device_id cpu = NULL, device = NULL;
	
	cl_int err = 0;
	size_t returned_size = 0;
	size_t buffer_size;
	
	cl_mem mem_cart_position,
		mem_cart_velocity,
		mem_cart_acceleration,
		mem_pole_angle,
		mem_pole_angle_origin,
		mem_pole_velocity,
		mem_pole_acceleration,
		mem_c_cart_position,
		mem_c_cart_velocity,
		mem_c_pole_angle,
		mem_c_pole_velocity,
		mem_force,
		mem_fitness;
	
// Device Information
	{
		// Find the CPU CL device, as a fallback
		err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_CPU, 1, &cpu, NULL);
		assert(err == CL_SUCCESS);
		
		// Find the GPU CL device, this is what we really want
		// If there is no GPU device is CL capable, fall back to CPU
		err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
		if (err != CL_SUCCESS) device = cpu;
		assert(device);
		
		// Get some information about the returned device
		cl_char vendor_name[1024] = {0};
		cl_char device_name[1024] = {0};
		err = clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(vendor_name), vendor_name, &returned_size);
		err |= clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), device_name, &returned_size);
		assert(err == CL_SUCCESS);
		printf("Connecting to %s %s...\n", vendor_name, device_name);
	}
	
// Context and Command Queue
	{
		// Now create a context to perform our calculation with the 
		// specified device 
		context = clCreateContext(0, 1, &device, NULL, NULL, &err);
		assert(err == CL_SUCCESS);
		
		// And also a command queue for the context
		cmd_queue = clCreateCommandQueue(context, device, 0, NULL);
	}
	
// Program and Kernel Creation
	{
		// Load the program source from disk
		// The kernel/program is the project directory and in Xcode the executable
		// is set to launch from that directory hence we use a relative path
		const char * filename = "fitness.cl";
		char *program_source = load_program_source(filename);
		program[0] = clCreateProgramWithSource(context, 1, (const char**)&program_source, NULL, &err);
		assert(err == CL_SUCCESS);
		
		err = clBuildProgram(program[0], 0, NULL, NULL, NULL, NULL);
		assert(err == CL_SUCCESS);
		
		// Now create the kernel "objects" that we want to use in the example file 
		kernel[0] = clCreateKernel(program[0], "computeFitness", &err);
	}
	
// Memory Allocation
	{
		// Allocate memory on the device to hold our data and store the results into
		buffer_size = sizeof(double);// * cart_position;
		mem_cart_position = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err = clEnqueueWriteBuffer(cmd_queue, mem_cart_position, CL_TRUE, 0, buffer_size, (void*)cart_position, 0, NULL, NULL);
		mem_cart_velocity = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_cart_velocity, CL_TRUE, 0, buffer_size, (void*)cart_velocity, 0, NULL, NULL);
		mem_cart_acceleration = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_cart_acceleration, CL_TRUE, 0, buffer_size, (void*)cart_acceleration, 0, NULL, NULL);
		mem_pole_angle = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_pole_angle, CL_TRUE, 0, buffer_size, (void*)pole_angle, 0, NULL, NULL);
		mem_pole_angle_origin = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_pole_angle_origin, CL_TRUE, 0, buffer_size, (void*)pole_angle_origin, 0, NULL, NULL);
		mem_pole_velocity = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_pole_velocity, CL_TRUE, 0, buffer_size, (void*)pole_velocity, 0, NULL, NULL);
		mem_pole_acceleration = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_pole_acceleration, CL_TRUE, 0, buffer_size, (void*)pole_acceleration, 0, NULL, NULL);
		mem_force = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_force, CL_TRUE, 0, buffer_size, (void*)force, 0, NULL, NULL);
		mem_fitness = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_fitness, CL_TRUE, 0, buffer_size, (void*)fitness, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
		
		buffer_size = sizeof(long int);// * c_cart_position;
		mem_c_cart_position = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err = clEnqueueWriteBuffer(cmd_queue, mem_c_cart_position, CL_TRUE, 0, buffer_size, (void*)c_cart_position, 0, NULL, NULL);
		mem_c_cart_velocity = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_c_cart_velocity, CL_TRUE, 0, buffer_size, (void*)c_cart_velocity, 0, NULL, NULL);
		mem_c_pole_angle = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_c_pole_angle, CL_TRUE, 0, buffer_size, (void*)c_pole_angle, 0, NULL, NULL);
		mem_c_pole_velocity = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		err |= clEnqueueWriteBuffer(cmd_queue, mem_c_pole_velocity, CL_TRUE, 0, buffer_size, (void*)c_pole_velocity, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
		
		// Get all of the stuff written and allocated 
		clFinish(cmd_queue);
	}
	
// Kernel Arguments
	{
		// Now setup the arguments to our kernel
		err  = clSetKernelArg(kernel[0],  0, sizeof(cl_mem), &mem_cart_position);
		err |= clSetKernelArg(kernel[0],  1, sizeof(cl_mem), &mem_cart_velocity);
		err |= clSetKernelArg(kernel[0],  2, sizeof(cl_mem), &mem_cart_acceleration);
		err |= clSetKernelArg(kernel[0],  3, sizeof(cl_mem), &mem_pole_angle);
		err |= clSetKernelArg(kernel[0],  4, sizeof(cl_mem), &mem_pole_angle_origin);
		err |= clSetKernelArg(kernel[0],  5, sizeof(cl_mem), &mem_pole_velocity);
		err |= clSetKernelArg(kernel[0],  6, sizeof(cl_mem), &mem_pole_acceleration);
		err |= clSetKernelArg(kernel[0],  7, sizeof(cl_mem), &mem_c_cart_position);
		err |= clSetKernelArg(kernel[0],  8, sizeof(cl_mem), &mem_c_cart_velocity);
		err |= clSetKernelArg(kernel[0],  9, sizeof(cl_mem), &mem_c_pole_angle);
		err |= clSetKernelArg(kernel[0],  10, sizeof(cl_mem), &mem_c_pole_velocity);
		err |= clSetKernelArg(kernel[0],  11, sizeof(cl_mem), &mem_force);
		err |= clSetKernelArg(kernel[0],  12, sizeof(cl_mem), &mem_fitness);

		assert(err == CL_SUCCESS);
	}
	
// Execution and Read
	{
		// Run the calculation by enqueuing it and forcing the 
		// command queue to complete the task
		int n = 1;
		size_t global_work_size = n;
		err = clEnqueueNDRangeKernel(cmd_queue, kernel[0], 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
		clFinish(cmd_queue);
		
		// Once finished read back the results from the answer 
		// array into the results array
		err = clEnqueueReadBuffer(cmd_queue, mem_fitness, CL_TRUE, 0, buffer_size, fitness, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
		clFinish(cmd_queue);
	}
	
// Teardown
	{
		clReleaseMemObject(mem_cart_position);
		clReleaseMemObject(mem_cart_velocity);
		clReleaseMemObject(mem_cart_acceleration);
		clReleaseMemObject(mem_pole_angle);
		clReleaseMemObject(mem_pole_angle_origin);
		clReleaseMemObject(mem_pole_velocity);
		clReleaseMemObject(mem_pole_acceleration);
		clReleaseMemObject(mem_c_cart_position);
		clReleaseMemObject(mem_c_cart_velocity);
		clReleaseMemObject(mem_c_pole_angle);
		clReleaseMemObject(mem_c_pole_velocity);
		clReleaseMemObject(mem_force);
		clReleaseMemObject(mem_fitness);
		
	
		clReleaseCommandQueue(cmd_queue);
		clReleaseContext(context);
	}
	
	return CL_SUCCESS;
}
