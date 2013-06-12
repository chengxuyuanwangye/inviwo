#define REF_SAMPLING_INTERVAL 150.0
uniform float samplingRate_;

#define RC_BEGIN_LOOP 

void raySetup(in vec3 entryPoint, in vec3 exitPoint, in vec3 volDimensions, out vec3 rayDirection, out float tIncr, out float tEnd) {
    rayDirection = exitPoint - entryPoint;
    tEnd = length(rayDirection);
    rayDirection = normalize(rayDirection);
    tIncr = 1.0/(samplingRate_*length(rayDirection*volDimensions));
}