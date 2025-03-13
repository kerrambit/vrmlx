#include <string>

std::string validWorldInfoNode = R"(

	#VRML V2.0 utf8

	WorldInfo {

	   title "cube.wrl"

	   info "VRML file."

	}
)";

std::string invalidWorldInfoNodeWrongDataType = R"(

	#VRML V2.0 utf8

	WorldInfo {

	   title Node { size 5.505 }

	   info "VRML file."

	}
)";

std::string invalidWorldInfoNodeWrongFieldName = R"(

	#VRML V2.0 utf8

	WorldInfo {

	   titles "cube.wrl"

	   info "VRML file."

	}
)";

std::string validBoxNode = R"(

	#VRML V2.0 utf8

	Box {
        size 58.58 0.45 558.05
	}
)";

std::string validBoxNodeEmpty = R"(

	#VRML V2.0 utf8

	Box {
        
	}
)";

std::string invalidBoxNodeWrongDataType = R"(

	#VRML V2.0 utf8

	Box {
        size [58.58 0.45 558.05]
	}
)";

std::string invalidBoxNodeWrongFieldName = R"(

	#VRML V2.0 utf8

	Box {
        center 58.58 0.45 558.05
	}
)";

std::string invalidGroupUnknownNode = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            NoNode {
                size 58.58 0.45 558.05
            }
        ]
    }
)";

std::string validGroup = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            Box {
                size 1 1 1 
            },
            Box {
                size 2 2 2
            }
        ]
    }
)";

std::string validShape = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            DEF id Box {
                size 58.58 0.45 558.05
            },
            Shape {
                geometry USE id
            }
        ]
    }

)";

std::string validShapeMerge = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            DEF id Box {
                size 58.58 0.45 558.05
            },
            Shape {
                geometry USE id
            },
            Shape {
                geometry USE id
            },
            Shape {
                geometry USE id
            }
        ]
    }

)";

std::string validShapeEmpty = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            DEF id Box {
                size 58.58 0.45 558.05
            },
            Shape {}
        ]
    }

)";

std::string invalidShapeWrongNodeForGeometryField = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            DEF id WorldInfo {
                title "cube.wrl"
                info "VRML file."
            },
            Shape {
                geometry USE id
            }
        ]
    }

)";

std::string validIndexedFaceSetNode = R"(

    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,
                            0 0 1, 1 0 1, 1 1 1, 0 1 1
                        ]
                    }
                    coordIndex [
                        0, 1, 2, 3, -1,
                        4, 5, 6, 7, -1,
                        0, 1, 5, 4, -1,
                        2, 3, 7, 6, -1,
                        0, 3, 7, 4, -1,
                        1, 2, 6, 5, -1
                    ]

                    normal Normal {
                        vector [
                            0 0 -1, 0 0 1, -1 0 0, 1 0 0, 0 -1 0, 0 1 0
                        ]
                    }
                    normalIndex [0, 1, 2, 3, -1, 4, 5, 2, 3, -1]

                    texCoord TextureCoordinate {
                        point [
                            0 0, 1 0, 1 1, 0 1
                        ]
                    }
                    texCoordIndex [0, 1, 2, 3, -1, 3, 2, 1, 0, -1]

                    color Color {
                        color [
                            1 0 0, 0 1 0, 0 0 1, 1 1 0
                        ]
                    }
                    colorIndex [0, 1, 2, 3, -1, 3, 2, 1, 0, -1]

                    ccw TRUE
                    convex TRUE
                    colorPerVertex TRUE
                    normalPerVertex TRUE
                    solid FALSE
                    creaseAngle 0.5
                }
            }
        ]
    }
)";

std::string validIndexedFaceSetNodeNotInShape = R"(

    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
                IndexedFaceSet {
                
                coord Coordinate {
                    point [
                        0 0 0, 1 0 0, 1 1 0, 0 1 0,
                        0 0 1, 1 0 1, 1 1 1, 0 1 1
                    ]
                }
                coordIndex [
                    0, 1, 2, 3, -1,
                    4, 5, 6, 7, -1,
                    0, 1, 5, 4, -1,
                    2, 3, 7, 6, -1,
                    0, 3, 7, 4, -1,
                    1, 2, 6, 5, -1
                ]

                normal Normal {
                    vector [
                        0 0 -1, 0 0 1, -1 0 0, 1 0 0, 0 -1 0, 0 1 0
                    ]
                }
                normalIndex [0, 1, 2, 3, -1, 4, 5, 2, 3, -1]

                texCoord TextureCoordinate {
                    point [
                        0 0, 1 0, 1 1, 0 1
                    ]
                }
                texCoordIndex [0, 1, 2, 3, -1, 3, 2, 1, 0, -1]

                color Color {
                    color [
                        1 0 0, 0 1 0, 0 0 1, 1 1 0
                    ]
                }
                colorIndex [0, 1, 2, 3, -1, 3, 2, 1, 0, -1]

                ccw TRUE
                convex TRUE
                colorPerVertex TRUE
                normalPerVertex TRUE
                solid FALSE
                creaseAngle 0
            }
        ]
    }
)";

std::string invalidIndexedFaceSetNodeWrongFieldNodeHeader = R"(

    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                
                    coord Point {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,
                            0 0 1, 1 0 1, 1 1 1, 0 1 1
                        ]
                    }
                    coordIndex [
                        0, 1, 2, 3, -1,
                        4, 5, 6, 7, -1,
                        0, 1, 5, 4, -1,
                        2, 3, 7, 6, -1,
                        0, 3, 7, 4, -1,
                        1, 2, 6, 5, -1
                    ]

                    normal Normal {
                        vector [
                            0 0 -1, 0 0 1, -1 0 0, 1 0 0, 0 -1 0, 0 1 0
                        ]
                    }
                    normalIndex [0, 1, 2, 3, -1, 4, 5, 2, 3, -1]

                    texCoord TextureCoordinate {
                        point [
                            0 0, 1 0, 1 1, 0 1
                        ]
                    }
                    texCoordIndex [0, 1, 2, 3, -1, 3, 2, 1, 0, -1]

                    color Color {
                        color [
                            1 0 0, 0 1 0, 0 0 1, 1 1 0
                        ]
                    }
                    colorIndex [0, 1, 2, 3, -1, 3, 2, 1, 0, -1]

                    ccw TRUE
                    convex TRUE
                    colorPerVertex TRUE
                    normalPerVertex TRUE
                    solid FALSE
                    creaseAngle 0.5
                }
            }
        ]
    }
)";

std::string validTransformatioNode = R"(

    #VRML V2.0 utf8

    DEF box Box { } # size 2 2 2

    Transform {
      center 0.5 0.669 0.789
      children [
        Shape { geometry USE box }
      ]
      rotation 0.7 0.89 1.0 1.57
      scale 1.0 2.0 1.69
      scaleOrientation 1 0 0 1.57
      translation 5 0 0
      bboxCenter 0 0 0
      bboxSize -1 -1 -1
    }
)";

std::string validTransformatioNodeEmpty = R"(

    #VRML V2.0 utf8

    DEF box Box { } # size 2 2 2

    Transform {
    }
)";

std::string validTransformatioNodeNested = R"(

    #VRML V2.0 utf8

    DEF box VRMLBox { } # size 2 2 2

    Transform {
      center 0.5 0.669 0.789
      children [
        Transform {
              center 0.5 0.669 0.789
              children [
                    Transform {
                      center 0.5 0.669 0.789
                      children [
                            Shape { geometry USE box }
                      ]
                      rotation 0.7 0.89 1.0 1.57
                      scale 1.0 2.0 1.69
                      scaleOrientation 1 0 0 1.57
                      translation 5 0 0
                    }
              ]
              rotation 0.7 0.89 1.0 1.57
              scale 1.0 2.0 1.69
              scaleOrientation 1 0 0 1.57
              translation 5 0 0
            }
      ]
      rotation 0.7 0.89 1.0 1.57
      scale 1.0 2.0 1.69
      scaleOrientation 1 0 0 1.57
      translation 5 0 0
    }
)";

std::string invalidTransformatioNodeWrongFieldNameCenter = R"(

    #VRML V2.0 utf8

    DEF box Box { } # size 2 2 2

    Transform {
      Center 0.5 0.669 0.789
      children [
        Shape { geometry USE box }
      ]
      rotation 0.7 0.89 1.0 1.57
      scale 1.0 2.0 1.69
      scaleOrientation 1 0 0 1.57
      translation 5 0 0
      bboxCenter 0 0 0
      bboxSize -1 -1 -1
    }
)";

std::string invalidTransformatioNodeWrongFieldNameChildren = R"(

    #VRML V2.0 utf8

    DEF box Box { } # size 2 2 2

    Transform {
      center 0.5 0.669 0.789
      Children [
        Shape { geometry USE box }
      ]
      rotation 0.7 0.89 1.0 1.57
      scale 1.0 2.0 1.69
      scaleOrientation 1 0 0 1.57
      translation 5 0 0
      bboxCenter 0 0 0
      bboxSize -1 -1 -1
    }
)";

std::string invalidTransformatioNodeWrongFieldTypeChildren = R"(

    #VRML V2.0 utf8

    DEF box Box { } # size 2 2 2

    Transform {
      center 0.5 0.669 0.789
      children [ 0 0 0 ]
      rotation 0.7 0.89 1.0 1.57
      scale 1.0 2.0 1.69
      scaleOrientation 1 0 0 1.57
      translation 5 0 0
      bboxCenter 0 0 0
      bboxSize -1 -1 -1
    }
)";

std::string invalidTransformatioNodeWrongFieldTypeScaleOrientation = R"(

    #VRML V2.0 utf8

    DEF box Box { } # size 2 2 2

    Transform {
      center 0.5 0.669 0.789
      children [
        Shape { geometry USE box }
      ]
      rotation 0.7 0.89 1.0 1.57
      scale 1.0 2.0 1.69
      scaleOrientation 1 0 0
      translation 5 0 0
      bboxCenter 0 0 0
      bboxSize -1 -1 -1
    }
)";

std::string validSwitchNodeWithOneChoiceEmpty = R"(

	#VRML V2.0 utf8

    Group {
          children [ 
            DEF A1 Switch {
                whichChoice 0
                choice DEF C2 Group { }
              }
           ]
         }
)";

std::string validSwitchNodeWithMulipleChoicesEmpty = R"(

	#VRML V2.0 utf8

    Group {
          children [ 
            DEF A1 Switch {
                whichChoice 17
                choice [
                    DEF C1 Group { },
                    DEF C2 Group { },
                    DEF C3 Group { },
                    DEF C4 Group { },
                    DEF C5 Group { },
                    DEF C6 Group { },
                    DEF C7 Group { },
                    DEF C8 Group { },
                    DEF C9 Group { },
                    DEF CA Group { },
                    DEF CB Group { },
                    DEF CC Group { },
                    DEF CD Group { },
                    DEF CE Group { },
                    DEF CF Group { },
                    DEF CG Group { },
                    DEF CH Group { },
                    DEF CI Group { }
                ]
              }
           ]
         }
)";

std::string validSwitchNodeWithMulipleChoices = R"(

	#VRML V2.0 utf8

    DEF box Box { } # size 2 2 2

    Group {
          children [ 
            DEF A1 Switch {
                whichChoice 3
                choice [
                    DEF C1 Group { },
                    DEF C2 Group { },
                    DEF C3 Group { },
                    DEF C4 Transform {
                        scale 2 2 2
                        children [
                            Shape { geometry USE box },
                            Shape { geometry USE box }
                        ]
                    }
                ]
              }
           ]
         }
)";

std::string validSwitchNodeEmpty = R"(

	#VRML V2.0 utf8

    Group {
          children [ 
                Switch {
              }
           ]
         }
)";

std::string invalidSwitchNodeWithWrongMulipleChoices = R"(

	#VRML V2.0 utf8

    Group {
          children [ 
            DEF A1 Switch {
                whichChoice 3
                choice DEF C1 Group { }
                choice DEF C2 Group { }
                choice DEF C3 Group { }
                choice DEF C4 Group { }
              }
           ]
         }
)";

std::string invalidSwitchNodeInvalidFieldType = R"(

	#VRML V2.0 utf8

    Group {
          children [ 
            DEF A1 Switch {
                whichChoice 0
                choice [ 7, 8, 5 ]
              }
           ]
         }
)";

std::string invalidSwitchNodeWrongIndexUpper = R"(

	#VRML V2.0 utf8

    Group {
          children [ 
            DEF A1 Switch {
                whichChoice 18
                choice [
                    DEF C1 Group { },
                    DEF C2 Group { },
                    DEF C3 Group { },
                    DEF C4 Group { },
                    DEF C5 Group { },
                    DEF C6 Group { },
                    DEF C7 Group { },
                    DEF C8 Group { },
                    DEF C9 Group { },
                    DEF CA Group { },
                    DEF CB Group { },
                    DEF CC Group { },
                    DEF CD Group { },
                    DEF CE Group { },
                    DEF CF Group { },
                    DEF CG Group { },
                    DEF CH Group { },
                    DEF CI Group { }
                ]
              }
           ]
         }
)";

std::string invalidSwitchNodeWrongIndexLower = R"(

	#VRML V2.0 utf8

    Group {
          children [ 
            DEF A1 Switch {
                whichChoice -1
                choice [
                    DEF C1 Group { },
                    DEF C2 Group { },
                    DEF C3 Group { },
                    DEF C4 Group { },
                    DEF C5 Group { },
                    DEF C6 Group { },
                    DEF C7 Group { },
                    DEF C8 Group { },
                    DEF C9 Group { },
                    DEF CA Group { },
                    DEF CB Group { },
                    DEF CC Group { },
                    DEF CD Group { },
                    DEF CE Group { },
                    DEF CF Group { },
                    DEF CG Group { },
                    DEF CH Group { },
                    DEF CI Group { }
                ]
              }
           ]
         }
)";

std::string invalidSwitchNodeEmptyChoice = R"(

	#VRML V2.0 utf8

    Group {
          children [ 
            DEF A1 Switch {
                whichChoice 0
                choice [ ]
              }
           ]
         }
)";

std::string validIndexedFaceSetPyramid = R"(
    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry VRMLIndexedFaceSet {
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,  # Base square (0-3)
                            0.5 0.5 1                    # Pyramid peak (4)
                        ]
                    }
                    coordIndex [
                        0, 1, 2, -1,
                        0, 2, 3, -1,  # Base square
                        0, 1, 4, -1,  # Side 1
                        1, 2, 4, -1,  # Side 2
                        2, 3, 4, -1,  # Side 3
                        3, 0, 4, -1   # Side 4
                    ]
                    convex FALSE
                    solid FALSE
                }
            }
        ]
    }
)";

std::string validAppearanceNodeEmpty = R"(
    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,  # Base square (0-3)
                            0.5 0.5 1                    # Pyramid peak (4)
                        ]
                    }
                    coordIndex [
                        0, 1, 2, -1,
                        0, 2, 3, -1,  # Base square
                        0, 1, 4, -1,  # Side 1
                        1, 2, 4, -1,  # Side 2
                        2, 3, 4, -1,  # Side 3
                        3, 0, 4, -1   # Side 4
                    ]
                    convex FALSE
                    solid FALSE
                }
                appearance Appearance {
                  
                }
            }
        ]
    }
)";

std::string validAppearanceNode = R"(
    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,  # Base square (0-3)
                            0.5 0.5 1                    # Pyramid peak (4)
                        ]
                    }
                    coordIndex [
                        0, 1, 2, -1,
                        0, 2, 3, -1,  # Base square
                        0, 1, 4, -1,  # Side 1
                        1, 2, 4, -1,  # Side 2
                        2, 3, 4, -1,  # Side 3
                        3, 0, 4, -1   # Side 4
                    ]
                    convex FALSE
                    solid FALSE
                }

                appearance Appearance {
                  material Material {
                    ambientIntensity 0.2
                    diffuseColor 0.8 0.8 0.8
                    emissiveColor 0 0 0
                    shininess 0.2
                    specularColor 0 0 0
                    transparency 0
                  }
                  texture ImageTexture {
                    url "image.jpg"
                    repeatS TRUE
                    repeatT TRUE
                  }
                  textureTransform TextureTransform {
                    center 0 0
                    rotation 0
                    scale 1 1
                    translation 0 0
                  }
                }
            }
        ]
    }
)";

std::string validAppearanceNode2 = R"(
    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,  # Base square (0-3)
                            0.5 0.5 1                    # Pyramid peak (4)
                        ]
                    }
                    coordIndex [
                        0, 1, 2, -1,
                        0, 2, 3, -1,  # Base square
                        0, 1, 4, -1,  # Side 1
                        1, 2, 4, -1,  # Side 2
                        2, 3, 4, -1,  # Side 3
                        3, 0, 4, -1   # Side 4
                    ]
                    convex FALSE
                    solid FALSE
                }

                appearance Appearance {
                  material Material {
                    ambientIntensity 0.2
                    diffuseColor 0.8 0.8 0.8
                    emissiveColor 0 0 0
                    shininess 0.2
                    specularColor 0 0 0
                    transparency 0
                  }
                  texture PixelTexture {
                    image 0 0 0
                    repeatS TRUE
                    repeatT TRUE
                  }
                  textureTransform TextureTransform {
                    center 0 0
                    rotation 0
                    scale 1 1
                    translation 0 0
                  }
                }
            }
        ]
    }
)";

std::string invalidAppearanceNodeWrongMaterialFieldName = R"(
    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,  # Base square (0-3)
                            0.5 0.5 1                    # Pyramid peak (4)
                        ]
                    }
                    coordIndex [
                        0, 1, 2, -1,
                        0, 2, 3, -1,  # Base square
                        0, 1, 4, -1,  # Side 1
                        1, 2, 4, -1,  # Side 2
                        2, 3, 4, -1,  # Side 3
                        3, 0, 4, -1   # Side 4
                    ]
                    convex FALSE
                    solid FALSE
                }

                appearance Appearance {
                  materiall Material {
                    ambientIntensity 0.2
                    diffuseColor 0.8 0.8 0.8
                    emissiveColor 0 0 0
                    shininess 0.2
                    specularColor 0 0 0
                    transparency 0
                  }
                  texture ImageTexture {
                    url "image.jpg"
                    repeatS TRUE
                    repeatT TRUE
                  }
                  textureTransform TextureTransform {
                    center 0 0
                    rotation 0
                    scale 1 1
                    translation 0 0
                  }
                }
            }
        ]
    }
)";

std::string invalidAppearanceNodeWrongMaterialNodeHeader = R"(
    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,  # Base square (0-3)
                            0.5 0.5 1                    # Pyramid peak (4)
                        ]
                    }
                    coordIndex [
                        0, 1, 2, -1,
                        0, 2, 3, -1,  # Base square
                        0, 1, 4, -1,  # Side 1
                        1, 2, 4, -1,  # Side 2
                        2, 3, 4, -1,  # Side 3
                        3, 0, 4, -1   # Side 4
                    ]
                    convex FALSE
                    solid FALSE
                }

                appearance Appearance {
                  material InnerData {
                    ambientIntensity 0.2
                    diffuseColor 0.8 0.8 0.8
                    emissiveColor 0 0 0
                    shininess 0.2
                    specularColor 0 0 0
                    transparency 0
                  }
                  texture ImageTexture {
                    url "image.jpg"
                    repeatS TRUE
                    repeatT TRUE
                  }
                  textureTransform TextureTransform {
                    center 0 0
                    rotation 0
                    scale 1 1
                    translation 0 0
                  }
                }
            }
        ]
    }
)";

std::string invalidAppearanceNodeWrongDataType = R"(
    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,  # Base square (0-3)
                            0.5 0.5 1                    # Pyramid peak (4)
                        ]
                    }
                    coordIndex [
                        0, 1, 2, -1,
                        0, 2, 3, -1,  # Base square
                        0, 1, 4, -1,  # Side 1
                        1, 2, 4, -1,  # Side 2
                        2, 3, 4, -1,  # Side 3
                        3, 0, 4, -1   # Side 4
                    ]
                    convex FALSE
                    solid FALSE
                }

                appearance Appearance {
                  material Material {
                    ambientIntensity 0.2
                    diffuseColor 0.8 0.8 0.8
                    emissiveColor 0 0 0
                    shininess 0.2
                    specularColor 0 0 0
                    transparency 0
                  }
                  texture ImageTexture {
                    url "image.jpg"
                    repeatS TRUE
                    repeatT TRUE
                  }
                  textureTransform TextureTransform {
                    center 0 0
                    rotation 0
                    scale 1 1
                    translation [ Group {} ]
                  }
                }
            }
        ]
    }
)";

std::string validIndexedLineSetNode = R"(

    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedLineSet {
                
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,
                            0 0 1, 1 0 1, 1 1 1, 0 1 1
                        ]
                    }
                    coordIndex [
                        0, 1, 2, 3, -1,
                        4, 5, 6, 7, -1,
                        0, 1, 5, 4, -1,
                        2, 3, 7, 6, -1,
                        0, 3, 7, 4, -1,
                        1, 2, 6, 5, -1
                    ]

                    color Color {
                        color [
                            1 0 0, 0 1 0, 0 0 1, 1 1 0
                        ]
                    }
                    colorIndex [0, 1, 2, 3, -1, 3, 2, 1, 0, -1]

                    colorPerVertex TRUE
                }
            }
        ]
    }
)";

std::string invalidIndexedLineSetNodeWrongFieldType = R"(

    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedLineSet {
                
                    coord Coordinate {
                        point [
                            0, 5, 0
                        ]
                    }
                    coordIndex [
                        0, 1, 2, 3, -1,
                        4, 5, 6, 7, -1,
                        0, 1, 5, 4, -1,
                        2, 3, 7, 6, -1,
                        0, 3, 7, 4, -1,
                        1, 2, 6, 5, -1
                    ]

                    color Color {
                        color [
                            1 0 0, 0 1 0, 0 0 1, 1 1 0
                        ]
                    }
                    colorIndex [0, 1, 2, 3, -1, 3, 2, 1, 0, -1]

                    colorPerVertex TRUE
                }
            }
        ]
    }
)";

std::string invalidIndexedLineSetNodeWrongUnknownField = R"(

    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedLineSet {
                
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0,
                            0 0 1, 1 0 1, 1 1 1, 0 1 1
                        ]
                    }
                    coordIndex [
                        0, 1, 2, 3, -1,
                        4, 5, 6, 7, -1,
                        0, 1, 5, 4, -1,
                        2, 3, 7, 6, -1,
                        0, 3, 7, 4, -1,
                        1, 2, 6, 5, -1
                    ]

                    color Color {
                        color [
                            1 0 0, 0 1 0, 0 0 1, 1 1 0
                        ]
                    }
                    colorIndex [0, 1, 2, 3, -1, 3, 2, 1, 0, -1]

                    colorPerVertex TRUE
                    convex FALSE
                }
            }
        ]
    }
)";
