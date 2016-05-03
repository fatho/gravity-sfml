#pragma once

#include <string>
#include <boost/format.hpp>

#define LOG_GENERAL(lg, sev, ...) lg.message(sev, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Debug, __VA_ARGS__)
#define LOG_INFO(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Info, __VA_ARGS__)
#define LOG_WARN(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Warning, __VA_ARGS__)
#define LOG_ERROR(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Error, __VA_ARGS__)
#define LOG_FATAL(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Fatal, __VA_ARGS__)

namespace fmtlog {
  
enum class Severity {
  Debug,
  Info,
  Warning,
  Error,
  Fatal
};

class Log {
public:
  Log(std::string scope);
  
  void message(Severity sev, const char* filename, int line, const std::string& msg);
  
  template <typename... Args>
  void message(Severity sev, const char* filename, int line, const std::string& fmt, Args&&... args) {
    boost::format f(fmt);
    using unroll = int[];
    static_cast<void>(unroll{0, (f % std::forward<Args>(args), 0)...});
    message(sev, filename, line, boost::str(f));
  }
  
private:
  std::string m_scope;
};

}
