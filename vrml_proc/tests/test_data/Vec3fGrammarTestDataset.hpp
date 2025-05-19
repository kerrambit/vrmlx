#include <string>

std::string vec3f = R"( # This is a vector3
                              100.001001       5.55317     -3.05561E-2 
                               # End of a vector   )";

std::string invalidVec3f = R"( # This is a vector3
                              100.001001       fewfwef     -3.05561E-2 
                               # End of a vector   )";
