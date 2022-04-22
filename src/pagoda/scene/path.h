#pragma once

#include <list>
#include <string>
#include <vector>

namespace pagoda::scene {
class Path {
public:
  ///----------------------------------------
  /// \name Ctors
  Path();
  explicit Path(const std::string &path);
  Path(const Path &p);
  Path(Path &&p);

  Path &operator=(const Path &p);
  Path &operator=(Path &&p);

  ///----------------------------------------
  /// \name Path Manipulation
  Path AppendChild(const Path &p) const;
  Path AppendPath(const Path &p) const;
  Path GetParent() const;
  Path GetName() const;
  Path ReplaceName(const Path &newName) const;

  ///----------------------------------------
  /// \name Queries
  bool IsAbsolute() const;
  bool IsRoot() const;
  bool IsValid() const;
  std::size_t GetNumComponents() const;
  bool HasPrefix(const Path &path) const;

  std::string ToString() const;

  ///----------------------------------------
  /// \name Logical Operators
  bool operator==(const Path& p) const;
private:
  std::vector<std::string> m_path;
  bool m_absoute;
  bool m_isValid;
};
} // namespace pagoda::scene

namespace std {
template <> struct hash<pagoda::scene::Path> {
  std::size_t operator()(const pagoda::scene::Path &p) const noexcept {
    return std::hash<std::string>{}(p.ToString());
  }
};
} // namespace std
