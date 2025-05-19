#include <string>

std::string first = R"(

	#VRML V2.0 utf8

	Test {
		
		field1 7.5 0.0 69.9
		field2 TRUE
	}
)";

std::string emptyArray = R"(

	#VRML V2.0 utf8

	Test {
		
		array []
	}
)";

std::string simpleEntityTypes = R"(

	#VRML V2.0 utf8

	Test {
		string "quoated string!" # string
        bool TRUE # boolean
        vec3f 100.001001       5.55317      -3.05561E-2 # vec3f
        vec3farray      [
                            8.59816       5.55317     -3.05561,
                            8.59816       2.49756      0.000000E+00,
                            8.59816       2.49756     -3.05561,
                            8.59816       5.55317E-1  -3.05561,
                        ] # vec3farray
        vec4f -5.05E-3 0.0 0.1 0.25E3 # vec4f
        int32array [ 42, 43, 44, 45, 46, 47, 48, 49, 50, ]
        float -0.1548E-5 # float
        int32 789456123
        vec2f 100.001001       5.55317      # vec2f
        vec2farray      [
                            8.59816       5.55317     ,
                            8.59816       2.49756     ,
                            8.59816       2.49756     ,
                            8.59816       5.55317E-1  ,
                        ] # vec2farray
	}
)";

std::string nodesArray = R"(

	#VRML V2.0 utf8

    Test {
       shape Shape {
          geometry IndexedFaceSet {
                  coordIndex [ 944, 942, 943, -1, 947, 945, 946, -1,
                      947, 949, 948, -1, 945, 947, 948, -1,
                      946, 942, 944, -1, 946, 944, 947, -1,
                      108889, 108856, 182463, -1, 108863, 108433, 182449, -1 ]
          }
          appearance 8.8 9.001 -0.1 -5.8E-2
          validUseId USE id
          invalidUseId USE idd
       }
    }

    DEF id Group {
        bboxCenter 58.7E-3 0.0 15
    }
)";

std::string children = R"(

	#VRML V2.0 utf8

    Group {
       children [
            IndexedFaceSet {
                  coordIndex [ 944, 942, 943, -1, 947, 945, 946, -1,
                      947, 949, 948, -1, 945, 947, 948, -1,
                      946, 942, 944, -1, 946, 944, 947, -1,
                      108889, 108856, 182463, -1, 108863, 108433, 182449, -1 ]
          },
          USE id,
          USE missingId
       ]
    }

    DEF id Group {
        bboxCenter 58.7E-3 0.0 15
    }
)";

std::string floatIntEdgeCase = R"(

	#VRML V2.0 utf8

	Test {
        int 0
        float_without_decimals 5.0
        float 5.5
	}
)";

std::string complicatedEdgeCase = R"(

    #VRML V2.0 utf8

    DEF id Test {
        int 0
        float 5.5
	}

    Group { children USE id }

    Group { children USE id }

    Test {
        int 10
        float 15.5
    }
)";
