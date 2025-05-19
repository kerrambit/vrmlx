#pragma once

#include <optional>

#include "NodeDescriptor.hpp"
#include "Vec2fArray.hpp"
#include "Vec3fArray.hpp"
#include "Vec4f.hpp"

namespace vrml_proc::traversor::node_descriptor {

  /**
   * @brief Fucntion which creates a node descriptor.
   */
  using NodeDescriptorFactory = std::function<NodeDescriptor()>;

  /**
   * @brief Type alias for mapping: node name and its factory.
   */
  using NodeDescriptorMap = std::map<std::string, NodeDescriptorFactory>;

  /**
   * @brief Lists all node types based on the VRML 2.0 standart.
   */
  inline NodeDescriptorMap GetNodeDescriptorMap() {
    static NodeDescriptorMap nodeDescriptionMap;
    static bool initialized = false;

    if (initialized) {
      return nodeDescriptionMap;
    }

    if (!initialized) {
      initialized = true;
    }

    nodeDescriptionMap["Group"] = []() {
      auto nd = NodeDescriptor("Group");
      static vrml_proc::parser::model::Vec3f defaultBoxCenter = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::model::Vec3f defaultBoxSize = {-1.0f, -1.0f, -1.0f};
      nd.BindField("bboxSize", defaultBoxSize);
      nd.BindField("bboxCenter", defaultBoxCenter);
      nd.BindVrmlNodeArray("children");

      return nd;
    };

    nodeDescriptionMap["Anchor"] = []() {
      auto nd = NodeDescriptor("Anchor");
      static vrml_proc::parser::model::Vec3f defaultBoxCenter = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::model::Vec3f defaultBoxSize = {-1.0f, -1.0f, -1.0f};
      static std::string defaultDescription = "";
      static std::string defaultParameter = "";
      static std::string defaultUrl = "";
      nd.BindField("description", defaultDescription);
      nd.BindField("parameter", defaultParameter);
      nd.BindField("url", defaultUrl);
      nd.BindField("bboxSize", defaultBoxSize);
      nd.BindField("bboxCenter", defaultBoxCenter);
      nd.BindVrmlNodeArray("children");

      return nd;
    };

    nodeDescriptionMap["Billboard"] = []() {
      auto nd = NodeDescriptor("Billboard");
      static vrml_proc::parser::model::Vec3f defaultBoxCenter = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::model::Vec3f defaultBoxSize = {-1.0f, -1.0f, -1.0f};
      static vrml_proc::parser::model::Vec3f defaultAxisOfRotation = {0.0f, 1.0f, 0.0f};
      nd.BindField("bboxSize", defaultBoxSize);
      nd.BindField("bboxCenter", defaultBoxCenter);
      nd.BindField("axisOfRotation", defaultAxisOfRotation);
      nd.BindVrmlNodeArray("children");

      return nd;
    };

    nodeDescriptionMap["Collision"] = []() {
      auto nd = NodeDescriptor("Collision");
      static vrml_proc::parser::model::Vec3f defaultBoxCenter = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::model::Vec3f defaultBoxSize = {-1.0f, -1.0f, -1.0f};
      static bool defaultCollide = true;
      static vrml_proc::parser::model::VrmlNode defaultProxy;
      nd.BindField("bboxSize", defaultBoxSize);
      nd.BindField("bboxCenter", defaultBoxCenter);
      nd.BindField("collide", defaultCollide);
      nd.BindVrmlNode("proxy",
          {"Anchor", "LOD", "Sound", "Background", "NavigationInfo", "SpotLight", "Billboard", "NormalInterpolator",
              "SphereSensor", "Collision", "OrientationInterpolator", "Switch", "ColorInterpolator", "PlaneSensor",
              "TimeSensor", "CoordinateInterpolator", "PointLight", "TouchSensor", "CylinderSensor",
              "PositionInterpolator", "Transform", "DirectionalLight", "ProximitySensor", "Viewpoint", "Fog",
              "ScalarInterpolator", "VisibilitySensor", "Group", "Script", "WorldInfo", "Inline", "Shape"},
          defaultProxy);
      nd.BindVrmlNodeArray("children");

      return nd;
    };

    nodeDescriptionMap["Inline"] = []() {
      auto nd = NodeDescriptor("Inline");
      static vrml_proc::parser::model::Vec3f defaultBoxCenter = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::model::Vec3f defaultBoxSize = {-1.0f, -1.0f, -1.0f};
      static std::string defaultUrl = "";
      nd.BindField("bboxSize", defaultBoxSize);
      nd.BindField("bboxCenter", defaultBoxCenter);
      nd.BindField("url", defaultUrl);
      nd.BindVrmlNodeArray("children");

      return nd;
    };

    nodeDescriptionMap["FontStyle"] = []() {
      auto nd = NodeDescriptor("FontStyle");

      static std::string defaultFamily = "SERIF";
      static bool defaultHorizontal = true;
      static std::string defaultJustify = "BEGIN";
      static std::string defaultLanguage = "";
      static bool defaultLeftToRight = true;
      static vrml_proc::parser::model::float32_t defaultSize = 1.0f;
      static vrml_proc::parser::model::float32_t defaultSpacing = 1.0f;
      static std::string defaultStyle = "PLAIN";
      static bool defaultTopToBottom = true;

      nd.BindField("family", defaultFamily);
      nd.ConstrainStringFieldValues("family", {"SERIF", "SANS", "TYPEWRITER", ""});
      nd.BindField("horizontal", defaultHorizontal);
      nd.BindField("justify", defaultJustify);
      nd.ConstrainStringFieldValues("justify", {"FIRST", "BEGIN", "MIDDLE", "END", ""});
      nd.BindField("language", defaultLanguage);
      nd.BindField("leftToRight", defaultLeftToRight);
      nd.BindField("size", defaultSize);
      nd.BindField("spacing", defaultSpacing);
      nd.BindField("style", defaultStyle);
      nd.ConstrainStringFieldValues("style", {"PLAIN", "BOLD", "ITALIC", "BOLDITALIC", ""});
      nd.BindField("topToBottom", defaultTopToBottom);

      return nd;
    };

    nodeDescriptionMap["Shape"] = []() {
      auto nd = NodeDescriptor("Shape");
      static vrml_proc::parser::model::VrmlNode defaultAppearance;
      static vrml_proc::parser::model::VrmlNode defaultGeometry;
      nd.BindVrmlNode("appearance", {"Appearance"}, defaultAppearance);
      nd.BindVrmlNode("geometry",
          {"Box", "Cone", "Cylinder", "ElevationGrid", "Extrusion", "IndexedFaceSet", "IndexedLineSet", "PointSet",
              "Sphere", "Text"},
          defaultGeometry);

      return nd;
    };

    nodeDescriptionMap["Box"] = []() {
      auto nd = NodeDescriptor("Box");
      static vrml_proc::parser::model::Vec3f defaultSize = {2.0f, 2.0f, 2.0f};
      nd.BindField("size", defaultSize);

      return nd;
    };

    nodeDescriptionMap["Color"] = []() {
      auto nd = NodeDescriptor("Color");
      static vrml_proc::parser::model::Vec3fArray defaultColor;
      nd.BindField("color", defaultColor);

      return nd;
    };

    nodeDescriptionMap["Coordinate"] = []() {
      auto nd = NodeDescriptor("Coordinate");
      static vrml_proc::parser::model::Vec3fArray defaultPoint;
      nd.BindField("point", defaultPoint);

      return nd;
    };

    nodeDescriptionMap["IndexedFaceSet"] = []() {
      auto nd = NodeDescriptor("IndexedFaceSet");

      static vrml_proc::parser::model::VrmlNode defaultColor;
      static vrml_proc::parser::model::VrmlNode defaultCoord;
      static vrml_proc::parser::model::VrmlNode defaultNormal;
      static vrml_proc::parser::model::VrmlNode defaultTexCoord;

      static bool defaultCcw = true;
      static vrml_proc::parser::model::Int32Array defaultColorIndex;
      static bool defaultColorPerVertex = true;
      static bool defaultConvex = true;
      static vrml_proc::parser::model::Int32Array defaultCoordIndex;
      static vrml_proc::parser::model::float32_t defaultCreaseangle = 0.0f;
      static vrml_proc::parser::model::Int32Array defaultNormalIndex;
      static bool defaultNormalPerVertex = true;
      static bool defaultSolid = true;
      static vrml_proc::parser::model::Int32Array defaultTexCoordIndex;

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

      nd.BindVrmlNode("color", {"Color"}, defaultColor);
      nd.BindVrmlNode("coord", {"Coordinate"}, defaultCoord);
      nd.BindVrmlNode("normal", {"Normal"}, defaultNormal);
      nd.BindVrmlNode("texCoord", {"TextureCoordinate"}, defaultTexCoord);

      return nd;
    };

    nodeDescriptionMap["IndexedLineSet"] = []() {
      auto nd = NodeDescriptor("IndexedLineSet");

      static vrml_proc::parser::model::VrmlNode defaultColor;
      static vrml_proc::parser::model::VrmlNode defaultCoord;

      static vrml_proc::parser::model::Int32Array defaultColorIndex;
      static bool defaultColorPerVertex = true;
      static vrml_proc::parser::model::Int32Array defaultCoordIndex;

      nd.BindField("colorIndex", defaultColorIndex);
      nd.BindField("colorPerVertex", defaultColorPerVertex);
      nd.BindField("coordIndex", defaultCoordIndex);

      nd.BindVrmlNode("color", {"Color"}, defaultColor);
      nd.BindVrmlNode("coord", {"Coordinate"}, defaultCoord);

      return nd;
    };

    nodeDescriptionMap["Normal"] = []() {
      auto nd = NodeDescriptor("Normal");
      static vrml_proc::parser::model::Vec3fArray defaultVector;
      nd.BindField("vector", defaultVector);

      return nd;
    };

    nodeDescriptionMap["Switch"] = []() {
      auto nd = NodeDescriptor("Switch");
      static int32_t defaultWhichChoice = -1;
      nd.BindField("whichChoice", defaultWhichChoice);
      nd.BindVrmlNodeArray("choice");

      return nd;
    };

    nodeDescriptionMap["TextureCoordinate"] = []() {
      auto nd = NodeDescriptor("TextureCoordinate");
      static vrml_proc::parser::model::Vec2fArray defaultPoint;
      nd.BindField("point", defaultPoint);

      return nd;
    };

    nodeDescriptionMap["Transform"] = []() {
      auto nd = NodeDescriptor("Transform");
      static vrml_proc::parser::model::Vec3f defaultCenter;
      static vrml_proc::parser::model::Vec4f defaultRotation = vrml_proc::parser::model::Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
      static vrml_proc::parser::model::Vec3f defaultScale = vrml_proc::parser::model::Vec3f(1.0f, 1.0f, 1.0f);
      static vrml_proc::parser::model::Vec4f defaultScaleOrientation =
          vrml_proc::parser::model::Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
      static vrml_proc::parser::model::Vec3f defaultTranslation;
      static vrml_proc::parser::model::Vec3f defaultBoxCenter = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::model::Vec3f defaultBoxSize = {-1.0f, -1.0f, -1.0f};
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
      auto nd = NodeDescriptor("WorldInfo");
      static std::string defaultInfo = "";
      static std::string defaultTitle = "";
      nd.BindField("info", defaultInfo);
      nd.BindField("title", defaultTitle);

      return nd;
    };

    nodeDescriptionMap["LOD"] = []() {
      auto nd = NodeDescriptor("LOD");
      /**
       * @todo New type `Float32Array` have to be implemented in parser.
       */

      // static vrml_proc::parser::Float32Array defaultRange;
      static vrml_proc::parser::model::Vec3f defaultCenter = {0.0f, 0.0f, 0.0f};
      nd.BindField("center", defaultCenter);
      nd.BindVrmlNodeArray("level");

      return nd;
    };

    nodeDescriptionMap["Text"] = []() {
      auto nd = NodeDescriptor("Text");
      /**
       * @todo New type `Float32Array` have to be implemented in parser.
       */

      // static vrml_proc::parser::Float32Array defaultLength;
      static std::string defaultString = "";
      static vrml_proc::parser::model::VrmlNode defaultFontStyle;
      static vrml_proc::parser::model::float32_t defaultMaxExtent = 0.0f;

      nd.BindField("string", defaultString);
      nd.BindVrmlNode("fontStyle", {"FontStyle"}, defaultFontStyle);
      nd.BindField("maxExtent", defaultMaxExtent);

      return nd;
    };

    nodeDescriptionMap["Material"] = []() {
      auto nd = NodeDescriptor("Material");
      static vrml_proc::parser::model::float32_t defaultAmbientIntensity = 0.2f;
      static vrml_proc::parser::model::Vec3f defaultDiffuseColor = {0.8f, 0.8f, 0.8f};
      static vrml_proc::parser::model::Vec3f defaultEmissiveColor = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::model::float32_t defaultShiness = 0.2f;
      static vrml_proc::parser::model::Vec3f defaultSpecularColor = {0.0f, 0.0f, 0.0f};
      static vrml_proc::parser::model::float32_t defaultTransparency = 0.0f;
      nd.BindField("ambientIntensity", defaultAmbientIntensity);
      nd.BindField("diffuseColor", defaultDiffuseColor);
      nd.BindField("emissiveColor", defaultEmissiveColor);
      nd.BindField("shininess", defaultShiness);
      nd.BindField("specularColor", defaultSpecularColor);
      nd.BindField("transparency", defaultTransparency);

      return nd;
    };

    nodeDescriptionMap["ImageTexture"] = []() {
      auto nd = NodeDescriptor("ImageTexture");
      static std::string defaultUrl = "";
      static bool defaultRepeatS = true;
      static bool defaultRepeatT = true;
      nd.BindField("url", defaultUrl);
      nd.BindField("repeatS", defaultRepeatS);
      nd.BindField("repeatT", defaultRepeatT);

      return nd;
    };

    nodeDescriptionMap["PixelTexture"] = []() {
      auto nd = NodeDescriptor("PixelTexture");
      static vrml_proc::parser::model::Vec3f defaultImage = {0.0f, 0.0f, 0.0f};
      static bool defaultRepeatS = true;
      static bool defaultRepeatT = true;
      nd.BindField("image", defaultImage);
      nd.BindField("repeatS", defaultRepeatS);
      nd.BindField("repeatT", defaultRepeatT);

      return nd;
    };

    nodeDescriptionMap["TextureTransform"] = []() {
      auto nd = NodeDescriptor("TextureTransform");
      static vrml_proc::parser::model::Vec2f defaultCenter = {0.0f, 0.0f};
      static vrml_proc::parser::model::float32_t defaultRotation = 0.0f;
      static vrml_proc::parser::model::Vec2f defaultScale = {1.0f, 1.0f};
      static vrml_proc::parser::model::Vec2f defaultTranslation = {0.0f, 0.0f};
      nd.BindField("center", defaultCenter);
      nd.BindField("rotation", defaultRotation);
      nd.BindField("scale", defaultScale);
      nd.BindField("translation", defaultTranslation);

      return nd;
    };

    nodeDescriptionMap["Appearance"] = []() {
      auto nd = NodeDescriptor("Appearance");

      static vrml_proc::parser::model::VrmlNode defaultMaterial;
      static vrml_proc::parser::model::VrmlNode defaultTexture;
      static vrml_proc::parser::model::VrmlNode defaultTextureTransform;

      nd.BindVrmlNode("material", {"Material"}, defaultMaterial);
      nd.BindVrmlNode("texture", {"ImageTexture", "MovieTexture", "PixelTexture"}, defaultTexture);
      nd.BindVrmlNode("textureTransform", {"TextureTransform"}, defaultTextureTransform);

      return nd;
    };

    nodeDescriptionMap["Cone"] = []() {
      auto nd = NodeDescriptor("Cone");

      static vrml_proc::parser::model::float32_t defaultBottomRadius = 1.0f;
      static vrml_proc::parser::model::float32_t defaultHeight = 2.0f;
      static bool defaultSide = true;
      static bool defaultBottom = true;
      nd.BindField("bottomRadius", defaultBottomRadius);
      nd.BindField("height", defaultHeight);
      nd.BindField("side", defaultSide);
      nd.BindField("bottom", defaultBottom);

      return nd;
    };

    nodeDescriptionMap["Cylinder"] = []() {
      auto nd = NodeDescriptor("Cylinder");

      static bool defaultBottom = true;
      static vrml_proc::parser::model::float32_t defaultHeight = 2.0f;
      static vrml_proc::parser::model::float32_t defaultRadius = 1.0f;
      static bool defaultSide = true;
      static bool defaultTop = true;
      nd.BindField("bottom", defaultBottom);
      nd.BindField("height", defaultHeight);
      nd.BindField("radius", defaultRadius);
      nd.BindField("side", defaultSide);
      nd.BindField("top", defaultTop);

      return nd;
    };

    nodeDescriptionMap["ElevationGrid"] = []() {
      auto nd = NodeDescriptor("ElevationGrid");

      static vrml_proc::parser::model::VrmlNode defaultColor;
      static vrml_proc::parser::model::VrmlNode defaultNormal;
      static vrml_proc::parser::model::VrmlNode defaultTexCoord;

      /**
       * @todo New type `Float32Array` have to be implemented in parser.
       */

      // static vrml_proc::parser::Float32Array defaultHeight;
      static bool defaultCcw = true;
      static bool defaultColorPerVertex = true;
      static vrml_proc::parser::model::float32_t defaultCreaseAngle = 0.0f;
      static bool defaultNormalPerVertex = true;
      static bool defaultSolid = true;
      static int32_t defaultXDimension = 0;
      static float_t defaultXSpacing = 0.0f;
      static int32_t defaultZDimension = 0;
      static float_t defaultZSpacing = 0.0f;

      // nd.BindField("height", defaultheight);
      nd.BindField("ccw", defaultCcw);
      nd.BindField("colorPerVertex", defaultColorPerVertex);
      nd.BindField("creaseAngle", defaultCreaseAngle);
      nd.BindField("normalPerVertex", defaultNormalPerVertex);
      nd.BindField("solid", defaultSolid);
      nd.BindField("xDimension", defaultXDimension);
      nd.BindField("xSpacing", defaultXSpacing);
      nd.BindField("zDimension", defaultZDimension);
      nd.BindField("zSpacing", defaultZSpacing);

      nd.BindVrmlNode("color", {"Color"}, defaultColor);
      nd.BindVrmlNode("normal", {"Normal"}, defaultNormal);
      nd.BindVrmlNode("texCoord", {"TextureCoordinate"}, defaultTexCoord);

      return nd;
    };

    nodeDescriptionMap["Extrusion"] = []() {
      auto nd = NodeDescriptor("Extrusion");

      static bool defaultBeginCap = true;
      static bool defaultCcw = true;
      static bool defaultConvex = true;
      static vrml_proc::parser::model::float32_t defaultCreaseAngle = 0.0f;
      static vrml_proc::parser::model::Vec2fArray defaultCrossSection;
      defaultCrossSection.vectors = {{1.0f, 1.0f}, {1.0f, -1.0f}, {-1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, 1.0f}};
      static bool defaultEndCap = true;
      static vrml_proc::parser::model::Vec4f defaultOrientation = {0.0f, 0.0f, 1.0f, 0.0f};
      static vrml_proc::parser::model::Vec2f defaultScale = {1.0f, 1.0f};
      static bool defaultSolid = true;
      static vrml_proc::parser::model::Vec3fArray defaultSpine;
      defaultSpine.vectors = {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};

      nd.BindField("beginCap", defaultBeginCap);
      nd.BindField("ccw", defaultCcw);
      nd.BindField("convex", defaultConvex);
      nd.BindField("creaseAngle", defaultCreaseAngle);
      nd.BindField("crossSection", defaultCrossSection);
      nd.BindField("endCap", defaultEndCap);
      nd.BindField("orientation", defaultOrientation);
      nd.BindField("scale", defaultScale);
      nd.BindField("solid", defaultSolid);
      nd.BindField("spine", defaultSpine);

      return nd;
    };

    nodeDescriptionMap["PointSet"] = []() {
      auto nd = NodeDescriptor("PointSet");

      static vrml_proc::parser::model::VrmlNode defaultColor;
      static vrml_proc::parser::model::VrmlNode defaultCoord;

      nd.BindVrmlNode("color", {"Color"}, defaultColor);
      nd.BindVrmlNode("coord", {"Coordinate"}, defaultCoord);

      return nd;
    };

    nodeDescriptionMap["Sphere"] = []() {
      auto nd = NodeDescriptor("Sphere");

      static vrml_proc::parser::model::float32_t defaultRadius = 1.0f;
      nd.BindField("radius", defaultRadius);

      return nd;
    };

    return nodeDescriptionMap;
  }

  /**
   * @brief Helper function, which queires a NodeDescriptor from NodeDescriptorMap.
   *
   * @param name canonical name of the node to create
   * @returns node descriptor or nullopt, if given node descriptor cannot be found
   *
   * @todo This functionality rather should be excapsulated in the class together with NodeDescriptorMap.
   */
  inline std::optional<NodeDescriptor> CreateNodeDescriptor(const std::string& name) {
    auto descriptorMap = GetNodeDescriptorMap();
    auto it = descriptorMap.find(name);
    if (it != descriptorMap.end()) {
      return it->second();
    }

    return std::nullopt;
  }
}  // namespace vrml_proc::traversor::node_descriptor
