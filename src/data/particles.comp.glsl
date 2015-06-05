#version 430 core

layout (std140, binding = 0) uniform attractorBlock
{
    vec4 attractor[64]; // xyz = coordinates, w = mass
};

layout (local_size_x = 128) in;

layout (rgba32f, binding = 0) uniform imageBuffer velocityBuffer;
layout (rgba32f, binding = 1) uniform imageBuffer positionBuffer;

uniform float dt;

void main() {
    // read the current position and velocity from the buffers
    vec4 vel = imageLoad(velocityBuffer, int(gl_GlobalInvocationID.x));
    vec4 pos = imageLoad(positionBuffer, int(gl_GlobalInvocationID.x));

    int i;

    //update position using current velocity
    pos.xyz += 5.0*vel.xyz * dt;
    pos.w -= 0.004 * dt;   //decrease lifetime

    // for each attractor
    for ( i = 0; i < 64; i++ ) {
        vec3 dist = attractor[i].xyz - pos.xyz;
        vel.xyz += dt*attractor[i].w * normalize(dist) / (dot(dist,dist) + 5.0 );
        //pos.xyz += 1000.0*attractor[i]*dt*dt*normalize(dist) / dot(dist, dist);
    }

    // if the particle expires, then reset it
    if ( pos.w < 0.0001 ) {
        pos.xyz = -pos.xyz * 0.1;
        vel.xyz *= 0.8;
        pos.w += 1.0;
    }

    //store the new positions and velocities
    imageStore(positionBuffer, int(gl_GlobalInvocationID.x), pos);
    imageStore(velocityBuffer, int(gl_GlobalInvocationID.x), vel);
}
