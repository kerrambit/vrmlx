#pragma once

#include <unordered_set>
#include <string>
#include <map>

#include "Hash.hpp"

namespace vrml_proc::traversor::node_descriptor {

  /**
   * @brief Groups all posible header names and their mapping to canonical VRML header name.
   * Each pair is {POSSIBLE_NAME, CANONICAL_FORM}.
   */
  inline const std::map<std::string, std::string>& GetHeaderToCanonicalNameMap() {
    static const std::map<std::string, std::string> headerToCanonicalName = {
        {"Group", "Group"},
        {"VRMLGroup", "Group"},
        {"Transform", "Transform"},
        {"VRMLTransform", "Transform"},
        {"Switch", "Switch"},
        {"VRMLSwitch", "Switch"},
        {"IndexedFaceSet", "IndexedFaceSet"},
        {"VRMLIndexedFaceSet", "IndexedFaceSet"},
        {"IndexedLineSet", "IndexedLineSet"},
        {"VRMLIndexedLineSet", "IndexedLineSet"},
        {"Box", "Box"},
        {"VRMLBox", "Box"},
        {"Shape", "Shape"},
        {"VRMLShape", "Shape"},
        {"Color", "Color"},
        {"VRMLColor", "Color"},
        {"Coordinate", "Coordinate"},
        {"VRMLCoordinate", "Coordinate"},
        {"Normal", "Normal"},
        {"VRMLNormal", "Normal"},
        {"TextureCoordinate", "TextureCoordinate"},
        {"VRMLTextureCoordinate", "TextureCoordinate"},
        {"Appearance", "Appearance"},
        {"VRMLAppearance", "Appearance"},
        {"WorldInfo", "WorldInfo"},
        {"VRMLWorldInfo", "WorldInfo"},
        {"Material", "Material"},
        {"VRMLMaterial", "Material"},
        {"ImageTexture", "ImageTexture"},
        {"VRMLImageTexture", "ImageTexture"},
        {"PixelTexture", "PixelTexture"},
        {"VRMLPixelTexture", "PixelTexture"},
        {"TextureTransform", "TextureTransform"},
        {"VRMLTextureTransform", "TextureTransform"},
        {"Cone", "Cone"},
        {"VRMLCone", "Cone"},
        {"Cylinder", "Cylinder"},
        {"VRMLCylinder", "Cylinder"},
        {"ElevationGrid", "ElevationGrid"},
        {"VRMLElevationGrid", "ElevationGrid"},
        {"Extrusion", "Extrusion"},
        {"VRMLExtrusion", "Extrusion"},
        {"PointSet", "PointSet"},
        {"VRMLPointSet", "PointSet"},
        {"Sphere", "Sphere"},
        {"VRMLSphere", "Sphere"},
    };
    return headerToCanonicalName;
  }

  namespace CanonicalHeaderHashes {
    using namespace vrml_proc::core::utils;

    constexpr uint32_t WorldInfo = Hash("WorldInfo");
    constexpr uint32_t Group = Hash("Group");
    constexpr uint32_t Transform = Hash("Transform");
    constexpr uint32_t Shape = Hash("Shape");
    constexpr uint32_t IndexedFaceSet = Hash("IndexedFaceSet");
    constexpr uint32_t IndexedLineSet = Hash("IndexedLineSet");
    constexpr uint32_t Coordinate = Hash("Coordinate");
    constexpr uint32_t Normal = Hash("Normal");
    constexpr uint32_t TextureCoordinate = Hash("TextureCoordinate");
    constexpr uint32_t Color = Hash("Color");
    constexpr uint32_t Box = Hash("Box");
    constexpr uint32_t Switch = Hash("Switch");
    constexpr uint32_t Material = Hash("Material");
    constexpr uint32_t ImageTexture = Hash("ImageTexture");
    constexpr uint32_t PixelTexture = Hash("PixelTexture");
    constexpr uint32_t TextureTransform = Hash("TextureTransform");
    constexpr uint32_t Appearance = Hash("Appearance");
    constexpr uint32_t Cone = Hash("Cone");
    constexpr uint32_t Cylinder = Hash("Cylinder");
    constexpr uint32_t ElevationGrid = Hash("ElevationGrid");
    constexpr uint32_t Extrusion = Hash("Extrusion");
    constexpr uint32_t PointSet = Hash("PointSet");
    constexpr uint32_t Sphere = Hash("Sphere");
  }  // namespace CanonicalHeaderHashes

  /**
   * @brief Converts a header name to canonical form.
   *
   * @param header header to convert
   *
   * @returns canonical header or if no canonical header was found, the passed header is returned
   */
  inline std::string ConvertToCanonicalHeader(const std::string& header) {
    auto it = GetHeaderToCanonicalNameMap().find(header);
    if (it != GetHeaderToCanonicalNameMap().end()) {
      return it->second;
    }
    return header;
  }

  inline std::unordered_set<std::string> GetPossibleNamesForCanonical(const std::string& canonicalName) {
    std::unordered_set<std::string> result;
    for (const auto& [possible, canonical] : GetHeaderToCanonicalNameMap()) {
      if (canonical == canonicalName) {
        result.insert(possible);
      }
    }
    return result;
  }

  inline std::unordered_set<std::string> GetPossibleNamesForCanonical(
      const std::string& canonicalName1, const std::string& canonicalName2) {
    std::unordered_set<std::string> result;
    for (const auto& [possible, canonical] : GetHeaderToCanonicalNameMap()) {
      if (canonical == canonicalName1 || canonical == canonicalName2) {
        result.insert(possible);
      }
    }
    return result;
  }

  inline std::unordered_set<std::string> GetPossibleNamesForCanonical(
      const std::string& canonicalName1, const std::string& canonicalName2, const std::string& canonicalName3) {
    std::unordered_set<std::string> result;
    for (const auto& [possible, canonical] : GetHeaderToCanonicalNameMap()) {
      if (canonical == canonicalName1 || canonical == canonicalName2 || canonical == canonicalName3) {
        result.insert(possible);
      }
    }
    return result;
  }
}  // namespace vrml_proc::traversor::node_descriptor
