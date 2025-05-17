#pragma once

#include <map>
#include <string>
#include <unordered_set>

#include "Hash.hpp"

/**
 * @brief Groups all hashes to canonical names.
 */
namespace vrml_proc::traversor::node_descriptor::CanonicalHeaderHashes {  //

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
  constexpr uint32_t Anchor = Hash("Anchor");
  constexpr uint32_t Billboard = Hash("Billboard");
  constexpr uint32_t Collision = Hash("Collision");
  constexpr uint32_t Inline = Hash("Inline");
  constexpr uint32_t LOD = Hash("LOD");
}  // namespace vrml_proc::traversor::node_descriptor::CanonicalHeaderHashes
