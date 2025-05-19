#pragma once

namespace to_geom::core::io {
  /**
   * @brief Lists all export formats for mesh.
   */
  enum class ExportFormat { Stl, Ply, Obj };

  namespace ExportFormatUtils {
    /**
     * @brief Utility function, which converts string to export format.
     *
     * @param string string to convert
     * @param defaultFormat default ExportFormat to use if string is invalid
     * @param defaultUsed flag indicating if default format was used
     * @returns ExportFormat after conversion
     */
    inline ExportFormat StringToExportFormat(const std::string& string, ExportFormat defaultFormat, bool& defaultUsed) {
      defaultUsed = false;
      if (string == "stl" || string == "STL") return to_geom::core::io::ExportFormat::Stl;
      if (string == "ply" || string == "PLY") return to_geom::core::io::ExportFormat::Ply;
      if (string == "obj" || string == "OBJ") return to_geom::core::io::ExportFormat::Obj;
      defaultUsed = true;
      return defaultFormat;
    }
  }  // namespace ExportFormatUtils
}  // namespace to_geom::core::io
