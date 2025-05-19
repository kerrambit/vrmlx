#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>

#include <result.hpp>

#include "Config.hpp"
#include "Error.hpp"
#include "JsonError.hpp"
#include "JsonFileReader.hpp"

namespace vrml_proc::traversor::node_descriptor {
  /**
   * @brief Represents a class storing mapping for synonyms of VRML nodes headers.
   */
  class VrmlHeaders : public vrml_proc::core::config::Config {
   public:
    /**
     * @brief Creates new object.
     */
    VrmlHeaders() = default;

    /**
     * @brief Loads a JSON file with synonyms definitions.
     *
     * @param filepath file path
     * @returns error if error occurs
     */
    cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>> Load(
        const std::filesystem::path& filepath) override {  //

      vrml_proc::core::io::JsonFileReader reader;
      auto json = reader.Read(filepath);

      if (json.has_value()) {
        try {
          for (auto it = (json.value()).begin(); it != (json.value()).end(); ++it) {
            const std::string& synonym = it.key();
            const std::string& canonical = it.value();

            m_synonymsToCanonicalNames[synonym] = canonical;
          }
        } catch (const nlohmann::json::exception& e) {
          return cpp::fail(std::make_shared<vrml_proc::core::io::error::JsonError>(e.what()));
        }

        return {};
      }

      return cpp::fail(json.error());
    }

    /**
     * @brief Adds new synonym.
     *
     * @param synonym new synonym
     * @param canonical canonical name, to which `synonym` resolves
     */
    void AddSynonym(const std::string& synonym, const std::string& canonical) {
      m_synonymsToCanonicalNames[synonym] = canonical;
    }

    /**
     * @brief Converts a header name to canonical form.
     *
     * @param header header to convert
     *
     * @returns canonical header or if no canonical header was found, the passed header is returned
     */
    std::string ConvertToCanonicalHeader(const std::string& header) const {
      auto it = m_synonymsToCanonicalNames.find(header);
      if (it != m_synonymsToCanonicalNames.end()) {
        return it->second;
      }
      return header;
    }

    /**
     * @brief Retrieves set of all synonyms for given set of canonical headers.
     *
     * @param canonicalHeaders set of canonical headers
     * @returns set of all synonyms for given canonical headers
     */
    std::unordered_set<std::string> GetSynonymsForCanonicalHeaders(
        const std::unordered_set<std::string>& canonicalHeaders) const {  //

      std::unordered_set<std::string> result;
      for (const auto& canonicalHeader : canonicalHeaders) {
        for (const auto& [synonym, canonical] : m_synonymsToCanonicalNames) {
          if (canonical == canonicalHeader) {
            result.insert(synonym);
          }
        }
      }

      return result;
    }

   private:
    /**
     * @brief Maps keys (synonyms) to values (canonical headers).
     */
    std::map<std::string, std::string> m_synonymsToCanonicalNames = {{"Group", "Group"}, {"Transform", "Transform"},
        {"Switch", "Switch"}, {"IndexedFaceSet", "IndexedFaceSet"}, {"IndexedLineSet", "IndexedLineSet"},
        {"Box", "Box"}, {"Shape", "Shape"}, {"Color", "Color"}, {"Coordinate", "Coordinate"}, {"Normal", "Normal"},
        {"TextureCoordinate", "TextureCoordinate"}, {"Appearance", "Appearance"}, {"WorldInfo", "WorldInfo"},
        {"Material", "Material"}, {"ImageTexture", "ImageTexture"}, {"PixelTexture", "PixelTexture"},
        {"TextureTransform", "TextureTransform"}, {"Cone", "Cone"}, {"Cylinder", "Cylinder"},
        {"ElevationGrid", "ElevationGrid"}, {"Extrusion", "Extrusion"}, {"PointSet", "PointSet"}, {"Sphere", "Sphere"},
        {"Text", "Text"}, {"Anchor", "Anchor"}, {"Billboard", "Billboard"}, {"Collision", "Collision"},
        {"Inline", "Inline"}, {"LOD", "LOD"}, {"FontStyle", "FontStyle"}};
  };
}  // namespace vrml_proc::traversor::node_descriptor
