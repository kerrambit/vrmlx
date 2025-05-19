#pragma once

namespace vrml_proc::parser {
  /**
   * @brief Contains two const char pointers indicating begin and end of view into some file.
   */
  struct BufferView {
    /**
     * @brief Constructs new BufferView.
     */
    BufferView(const char* start, const char* finish) : begin(start), end(finish) {}

    const char* begin;
    const char* end;
  };
}  // namespace vrml_proc::parser
