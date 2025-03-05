#pragma once

#include "NodeDescriptor.hpp"
#include "VrmlCanonicalHeaders.hpp"
#include "Vec2fArray.hpp"
#include "Vec3fArray.hpp"
#include "Vec4f.hpp"

namespace vrml_proc::traversor::node_descriptor {

  using NodeDescriptorFactory = std::function<NodeDescriptor()>;
  using NodeDescriptorMap = std::map<std::string, NodeDescriptorFactory>;

  inline NodeDescriptorMap CreateNodeDescriptorMap() {
    static NodeDescriptorMap nodeDescriptionMap;
    static bool initialized = false;

    if (initialized) {
      return nodeDescriptionMap;
    }

    if (!initialized) {
      initialized = true;
    }

    nodeDescriptionMap["Group"] = []() {
      auto nd = NodeDescriptor("Group", GetPossibleNamesForCanonical("Group"));
      static vrml_proc::parser::Vec3f defaultBoxCenter = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::Vec3f defaultBoxSize = {-1.0f, -1.0f, -1.0f};
      nd.BindField("bboxSize", defaultBoxSize);
      nd.BindField("bboxCenter", defaultBoxCenter);
      nd.BindVrmlNodeArray("children");

      return nd;
    };

    nodeDescriptionMap["Shape"] = []() {
      auto nd = NodeDescriptor("Shape", GetPossibleNamesForCanonical("Shape"));
      static vrml_proc::parser::VrmlNode defaultAppearance;
      static vrml_proc::parser::VrmlNode defaultGeometry;
      nd.BindVrmlNode("appearance", GetPossibleNamesForCanonical("Appearance"), defaultAppearance);
      nd.BindVrmlNode("geometry",
                      {"Box",
                       "VRMLBox",
                       "Cone",
                       "VRMLCone",
                       "Cylinder",
                       "VRMLCylinder",
                       "ElevationGrid",
                       "VRMLElevationGrid",
                       "Extrusion",
                       "VRMLExtrusion",
                       "IndexedFaceSet",
                       "VRMLIndexedFaceSet",
                       "IndexedLineSet",
                       "VRMLIndexedLineSet",
                       "PointSet",
                       "VRMLPointSet",
                       "Sphere",
                       "VRMLSphere",
                       "Text",
                       "VRMLText"},
                      defaultGeometry);  // TODO: fucntion will take set of names

      return nd;
    };

    nodeDescriptionMap["Box"] = []() {
      auto nd = NodeDescriptor("Box", GetPossibleNamesForCanonical("Box"));
      static vrml_proc::parser::Vec3f defaultSize = {2.0f, 2.0f, 2.0f};
      nd.BindField("size", defaultSize);

      return nd;
    };

    nodeDescriptionMap["Color"] = []() {
      auto nd = NodeDescriptor("Color", GetPossibleNamesForCanonical("Color"));
      static vrml_proc::parser::Vec3fArray defaultColor;
      nd.BindField("color", defaultColor);

      return nd;
    };

    nodeDescriptionMap["Coordinate"] = []() {
      auto nd = NodeDescriptor("Coordinate", GetPossibleNamesForCanonical("Coordinate"));
      static vrml_proc::parser::Vec3fArray defaultPoint;
      nd.BindField("point", defaultPoint);

      return nd;
    };

    nodeDescriptionMap["IndexedFaceSet"] = []() {
      auto nd = NodeDescriptor("IndexedFaceSet", GetPossibleNamesForCanonical("IndexedFaceSet"));

      static vrml_proc::parser::VrmlNode defaultColor;
      static vrml_proc::parser::VrmlNode defaultCoord;
      static vrml_proc::parser::VrmlNode defaultNormal;
      static vrml_proc::parser::VrmlNode defaultTexCoord;

      static bool defaultCcw = true;
      static vrml_proc::parser::Int32Array defaultColorIndex;
      static bool defaultColorPerVertex = true;
      static bool defaultConvex = true;
      static vrml_proc::parser::Int32Array defaultCoordIndex;
      static vrml_proc::parser::float32_t defaultCreaseangle = 0.0f;
      static vrml_proc::parser::Int32Array defaultNormalIndex;
      static bool defaultNormalPerVertex = true;
      static bool defaultSolid = true;
      static vrml_proc::parser::Int32Array defaultTexCoordIndex;

      nd.BindField("ccw", defaultCcw);
      nd.BindField("colorIndex", defaultColorIndex);
      nd.BindField("colorPerVertex", defaultColorPerVertex);
      nd.BindField("convex", defaultConvex);
      nd.BindField("coordIndex", defaultCoordIndex);
      nd.BindField("creaseAngle", defaultCreaseangle);
      nd.BindField("normalIndex", defaultNormalIndex);
      nd.BindField("normalPerVertex", defaultNormalPerVertex);
      nd.BindField("solid", defaultSolid);
      nd.BindField("texCoordIndex", defaultTexCoordIndex);

      nd.BindVrmlNode("color", GetPossibleNamesForCanonical("Color"), defaultColor);
      nd.BindVrmlNode("coord", GetPossibleNamesForCanonical("Coordinate"), defaultCoord);
      nd.BindVrmlNode("normal", GetPossibleNamesForCanonical("Normal"), defaultNormal);
      nd.BindVrmlNode("texCoord", GetPossibleNamesForCanonical("TextureCoordinate"), defaultTexCoord);

      return nd;
    };

    nodeDescriptionMap["Normal"] = []() {
      auto nd = NodeDescriptor("Normal", GetPossibleNamesForCanonical("Normal"));
      static vrml_proc::parser::Vec3fArray defaultVector;
      nd.BindField("vector", defaultVector);

      return nd;
    };

    nodeDescriptionMap["Switch"] = []() {
      auto nd = NodeDescriptor("Switch", GetPossibleNamesForCanonical("Switch"));
      static int32_t defaultWhichChoice = -1;
      nd.BindField("whichChoice", defaultWhichChoice);
      nd.BindVrmlNodeArray("choice");

      return nd;
    };

    nodeDescriptionMap["TextureCoordinate"] = []() {
      auto nd = NodeDescriptor("TextureCoordinate", GetPossibleNamesForCanonical("TextureCoordinate"));
      static vrml_proc::parser::Vec2fArray defaultPoint;
      nd.BindField("point", defaultPoint);

      return nd;
    };

    nodeDescriptionMap["Transform"] = []() {
      auto nd = NodeDescriptor("Transform", GetPossibleNamesForCanonical("Transform"));
      static vrml_proc::parser::Vec3f defaultCenter;
      static vrml_proc::parser::Vec4f defaultRotation = vrml_proc::parser::Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
      static vrml_proc::parser::Vec3f defaultScale = vrml_proc::parser::Vec3f(1.0f, 1.0f, 1.0f);
      static vrml_proc::parser::Vec4f defaultScaleOrientation = vrml_proc::parser::Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
      static vrml_proc::parser::Vec3f defaultTranslation;
      static vrml_proc::parser::Vec3f defaultBoxCenter = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::Vec3f defaultBoxSize = {-1.0f, -1.0f, -1.0f};
      nd.BindField("bboxSize", defaultBoxSize);
      nd.BindField("bboxCenter", defaultBoxCenter);
      nd.BindField("center", defaultCenter);
      nd.BindField("rotation", defaultRotation);
      nd.BindField("scale", defaultScale);
      nd.BindField("scaleOrientation", defaultScaleOrientation);
      nd.BindField("translation", defaultTranslation);
      nd.BindVrmlNodeArray("children");

      return nd;
    };

    nodeDescriptionMap["WorldInfo"] = []() {
      auto nd = NodeDescriptor("WorldInfo", GetPossibleNamesForCanonical("WorldInfo"));
      static std::string defaultInfo = "";
      static std::string defaultTitle = "";
      nd.BindField("info", defaultInfo);
      nd.BindField("title", defaultTitle);

      return nd;
    };

    nodeDescriptionMap["Material"] = []() {
      auto nd = NodeDescriptor("Material", GetPossibleNamesForCanonical("Material"));
      static vrml_proc::parser::float32_t defaultAmbientIntensity = 0.2f;
      static vrml_proc::parser::Vec3f defaultDiffuseColor = {0.8f, 0.8f, 0.8f};
      static vrml_proc::parser::Vec3f defaultEmissiveColor = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::float32_t defaultShiness = 0.2f;
      static vrml_proc::parser::Vec3f defaultSpecularColor = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::float32_t defaultTransparency = 0.0f;
      nd.BindField("ambientIntensity", defaultAmbientIntensity);
      nd.BindField("diffuseColor", defaultDiffuseColor);
      nd.BindField("emissiveColor", defaultEmissiveColor);
      nd.BindField("shininess", defaultShiness);
      nd.BindField("specularColor", defaultSpecularColor);
      nd.BindField("transparency", defaultTransparency);

      return nd;
    };

    nodeDescriptionMap["ImageTexture"] = []() {
      auto nd = NodeDescriptor("ImageTexture", GetPossibleNamesForCanonical("ImageTexture"));
      static std::string defaultUrl = "";
      static bool defaultRepeatS = true;
      static bool defaultRepeatT = true;
      nd.BindField("url", defaultUrl);
      nd.BindField("repeatS", defaultRepeatS);
      nd.BindField("repeatT", defaultRepeatT);

      return nd;
    };

    nodeDescriptionMap["PixelTexture"] = []() {
      auto nd = NodeDescriptor("PixelTexture", GetPossibleNamesForCanonical("PixelTexture"));
      static vrml_proc::parser::Vec3f defaultImage = {0.0f, 0.0f, 0.0f};
      static bool defaultRepeatS = true;
      static bool defaultRepeatT = true;
      nd.BindField("image", defaultImage);
      nd.BindField("repeatS", defaultRepeatS);
      nd.BindField("repeatT", defaultRepeatT);

      return nd;
    };

    nodeDescriptionMap["TextureTransform"] = []() {
      auto nd = NodeDescriptor("TextureTransform", GetPossibleNamesForCanonical("TextureTransform"));
      static vrml_proc::parser::Vec2f defaultCenter = {0.0f, 0.0f};
      static vrml_proc::parser::float32_t defaultRotation = 0.0f;
      static vrml_proc::parser::Vec2f defaultScale = {1.0f, 1.0f};
      static vrml_proc::parser::Vec2f defaultTranslation = {0.0f, 0.0f};
      nd.BindField("center", defaultCenter);
      nd.BindField("rotation", defaultRotation);
      nd.BindField("scale", defaultScale);
      nd.BindField("translation", defaultTranslation);

      return nd;
    };

    nodeDescriptionMap["Appearance"] = []() {
      auto nd = NodeDescriptor("Appearance", GetPossibleNamesForCanonical("Appearance"));

      static vrml_proc::parser::VrmlNode defaultMaterial;
      static vrml_proc::parser::VrmlNode defaultTexture;
      static vrml_proc::parser::VrmlNode defaultTextureTransform;

      nd.BindVrmlNode("material", GetPossibleNamesForCanonical("Material"), defaultMaterial);
      nd.BindVrmlNode("texture", GetPossibleNamesForCanonical("ImageTexture", "MovieTexture", "PixelTexture"),
                      defaultTexture);
      nd.BindVrmlNode("textureTransform", GetPossibleNamesForCanonical("TextureTransform"), defaultTextureTransform);

      return nd;
    };

    return nodeDescriptionMap;
  }
}  // namespace vrml_proc::traversor::node_descriptor
