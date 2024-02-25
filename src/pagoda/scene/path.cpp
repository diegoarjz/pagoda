#include "path.h"

#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/phoenix/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

#include <iostream>
#include <sstream>

namespace pagoda::scene {

Path::Path() : m_isValid{false} {}

Path::Path(const std::string &path) : m_absoute{false}, m_isValid{false} {
  using namespace boost::spirit;
  using namespace boost::spirit::qi;
  using boost::phoenix::ref;
  using boost::spirit::qi::phrase_parse;

  using Iterator = std::string::const_iterator;
  Iterator first = path.cbegin();
  Iterator last = path.cend();

  rule<Iterator, std::string(), space_type> identifier =
      lexeme[(+(char_('_') | alpha) >> *(alnum | char_('_')))];
  rule<Iterator, void(), space_type> separator = char_('/');

  auto backInserter = std::back_inserter(m_path);

  m_isValid =
      boost::spirit::qi::phrase_parse(
          first, last,
          //
          -(separator[boost::phoenix::ref(m_absoute) = true]) >>
              -(identifier[boost::phoenix::ref(backInserter) = _1] % separator),
          //
          space) &&
      (first == last);
}

Path::Path(const Path &p)
    : m_path{p.m_path}, m_absoute{p.m_absoute}, m_isValid{p.m_isValid} {}

Path::Path(Path &&p)
    : m_path{std::move(p.m_path)}, m_absoute{p.m_absoute}, m_isValid{
                                                               p.m_isValid} {
  p.m_isValid = false;
}

Path &Path::operator=(const Path &p) {
  m_path = p.m_path;
  m_isValid = p.m_isValid;
  m_absoute = p.m_absoute;
  return *this;
}

Path &Path::operator=(Path &&p) {
  m_path = std::move(p.m_path);
  m_isValid = p.m_isValid;
  m_absoute = p.m_absoute;
  p.m_isValid = false;
  return *this;
}

Path Path::AppendChild(const Path &p) const {
  if (p.GetNumComponents() != 1 || p.m_absoute) {
    return Path{};
  }
  return AppendPath(p);
}

Path Path::AppendPath(const Path &p) const {
  Path newPath;
  if (p.m_absoute) {
    return newPath;
  }
  newPath.m_isValid = m_isValid;
  newPath.m_absoute = m_absoute;
  newPath.m_path.reserve(m_path.size() + 1);
  for (const auto &path : m_path) {
    newPath.m_path.push_back(path);
  }
  newPath.m_path.push_back(p.m_path.front());
  return newPath;
}

Path Path::GetParent() const {
  Path parent;
  parent.m_isValid = m_isValid;
  parent.m_absoute = m_absoute;
  parent.m_path =
      std::vector<std::string>(m_path.begin(), std::prev(m_path.end()));
  return parent;
}

Path Path::GetName() const { return Path{m_path.back()}; }

Path Path::ReplaceName(const Path &newName) const {
  if (newName.GetNumComponents() != 1 || newName.IsAbsolute()) {
    return Path{};
  }
  Path p = GetParent();
  p.m_path.push_back(newName.m_path.front());
  return p;
}

bool Path::IsAbsolute() const { return m_absoute; }

bool Path::IsRoot() const { return m_absoute && m_path.size() == 0; }

bool Path::IsValid() const { return m_isValid; }

std::size_t Path::GetNumComponents() const { return m_path.size(); }

bool Path::HasPrefix(const Path &p) const {
  if (p.m_absoute != m_absoute || m_path.size() < p.m_path.size()) {
    return false;
  }

  for (auto i = 0; i < p.m_path.size(); ++i) {
    if (m_path[i] != p.m_path[i]) {
      return false;
    }
  }

  return true;
}

std::string Path::ToString() const {
  if (!IsValid()) {
    return "";
  }

  std::stringstream s;
  if (IsAbsolute()) {
    s << "/";
  }
  for (const auto &p : m_path) {
    s << p;
    if (p != m_path.back()) {
      s << "/";
    }
  }
  return s.str();
}

bool Path::operator==(const Path& p) const{
  if (m_isValid != p.m_isValid || m_absoute != p.m_absoute || m_path.size() != p.m_path.size()) {
    return false;
  }

  for (auto i=0; i< m_path.size(); ++i) {
    if (m_path[i] != p.m_path[i]) {
      return false;
    }
  }

  return true;
}
} // namespace pagoda::scene
