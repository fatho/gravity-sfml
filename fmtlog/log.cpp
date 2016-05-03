#include "log.hpp"

#include <iostream>
#include <chrono>
#include <cstring>

namespace {
  auto ProgramStartTime = std::chrono::steady_clock::now();
  
  const char* severityToString(fmtlog::Severity sev) {
    switch (sev) {
      case fmtlog::Severity::Debug:
        return "DEBUG";
      case fmtlog::Severity::Info:
        return "INFO";
      case fmtlog::Severity::Warning:
        return "WARNING";
      case fmtlog::Severity::Error:
        return "ERROR";
      case fmtlog::Severity::Fatal:
        return "FATAL";
    }
    return "UNKOWN";
  }
}

namespace fmtlog {
  
  Log::Log(std::string scope) : m_scope(scope) {}
  
  void Log::message(Severity sev, const char* filename, int line, const std::string& msg) {
    // TODO: do something more clever with log entries
    auto curTime = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::ratio<1>> sinceProgramStart = curTime - ProgramStartTime;
    
    std::cerr << boost::format("%1$9.3f %2$s %3$s <%4$s:%5$d> %6$s\n") % sinceProgramStart.count() %
    severityToString(sev) % m_scope % filename % line % msg;
  }
  
}
