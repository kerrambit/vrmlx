#pragma once

#include <cstdint>
#include <ostream>
#include <string>

#include "Int32Array.hpp"
#include "UseNode.hpp"
#include "Vec2f.hpp"
#include "Vec2fArray.hpp"
#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "Vec4f.hpp"
#include "VrmlField.hpp"
#include "VrmlNode.hpp"

#include "VrmlProcessingExport.hpp"

#define ONE_INDENTATION_LEVEL_LENGTH 2

namespace vrml_proc::parser::model::utils {

  /**
   * @brief Represent a class which prints a tree-like strcture of VRML node.
   */
  class VRMLPROCESSING_API VrmlTreePrinter {
   public:
    /**
     * @brief Constructs new VrmlTreePrinter.
     *
     * @param defaultOutputStream pointer to output stream, the caller is responsible for closing the file etc.
     */
    VrmlTreePrinter(std::ostream& defaultOutputStream) : m_stream(&defaultOutputStream) {}

    /**
     * @brief Represents a number of indentation objects (such an indentation object can be for example a space).
     */
    using IndentationLevel = uint16_t;

    /**
     * @brief Prints a possibly a a tree-like structure.
     *
     * @param object object (root) which will be printed (and possible its children etc.)
     * @param indentationLevel how many indentation objects should be used
     */
    void Print(const vrml_proc::parser::VrmlNode& object, IndentationLevel indentationLevel = 0) const;
    /**
     * @brief Prints a possibly a a tree-like structure.
     *
     * @param object object (root) which will be printed (and possible its children etc.)
     * @param indentationLevel how many indentation objects should be used
     */
    void Print(const vrml_proc::parser::UseNode& object, IndentationLevel indentationLevel = 0) const;
    /**
     * @brief Prints a possibly a a tree-like structure.
     *
     * @param object object (root) which will be printed (and possible its children etc.)
     * @param indentationLevel how many indentation objects should be used
     */
    void Print(const vrml_proc::parser::Int32Array& object, IndentationLevel indentationLevel = 0) const;
    /**
     * @brief Prints a possibly a a tree-like structure.
     *
     * @param object object (root) which will be printed (and possible its children etc.)
     * @param indentationLevel how many indentation objects should be used
     */
    void Print(const vrml_proc::parser::Vec2f& object, IndentationLevel indentationLevel = 0) const;
    /**
     * @brief Prints a possibly a a tree-like structure.
     *
     * @param object object (root) which will be printed (and possible its children etc.)
     * @param indentationLevel how many indentation objects should be used
     */
    void Print(const vrml_proc::parser::Vec2fArray& object, IndentationLevel indentationLevel = 0) const;
    /**
     * @brief Prints a possibly a a tree-like structure.
     *
     * @param object object (root) which will be printed (and possible its children etc.)
     * @param indentationLevel how many indentation objects should be used
     */
    void Print(const vrml_proc::parser::Vec3f& object, IndentationLevel indentationLevel = 0) const;
    /**
     * @brief Prints a possibly a a tree-like structure.
     *
     * @param object object (root) which will be printed (and possible its children etc.)
     * @param indentationLevel how many indentation objects should be used
     */
    void Print(const vrml_proc::parser::Vec3fArray& object, IndentationLevel indentationLevel = 0) const;
    /**
     * @brief Prints a possibly a a tree-like structure.
     *
     * @param object object (root) which will be printed (and possible its children etc.)
     * @param indentationLevel how many indentation objects should be used
     */
    void Print(const vrml_proc::parser::Vec4f& object, IndentationLevel indentationLevel = 0) const;
    /**
     * @brief Prints a possibly a a tree-like structure.
     *
     * @param object object (root) which will be printed (and possible its children etc.)
     * @param indentationLevel how many indentation objects should be used
     */
    void Print(const vrml_proc::parser::VrmlField& object, IndentationLevel indentationLevel = 0) const;

    /**
     * @brief Creates an indentation object as a string.
     *
     * Indentation object in this case is a space.
     *
     * @param indentationLevel number of indentation objects
     * @returns string of an indentation object
     */
    inline static std::string CreateIndentationString(IndentationLevel indentationLevel) {
      return std::string(ONE_INDENTATION_LEVEL_LENGTH * indentationLevel, ' ');
    }

   private:
    std::ostream* m_stream;
  };
}  // namespace vrml_proc::parser::model::utils