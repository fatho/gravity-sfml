#include "fmtlog.hpp"

#include <iostream>
#include <chrono>
#include <cstring>

/// Implementation details of the logging library.
namespace {
  /// Captures the system time when the program is started.
  auto ProgramStartTime = std::chrono::steady_clock::now();

  /*! \brief Converts a severity class into a human readable string.
   *  \param sev the severity class
   *  \returns a human readable string describing the severity class.
   */
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

using namespace fmtlog;

Log::Log(const std::string& scope) : m_scope(scope) {}
Log::Log(const char* scope) : m_scope(scope) {}

void Log::message(Severity sev, const std::string& msg) {
  // TODO: do something more clever with log entries
  auto curTime = std::chrono::steady_clock::now();
  std::chrono::duration<double, std::ratio<1>> sinceProgramStart = curTime - ProgramStartTime;

  std::cerr << boost::format("%1$9.3f %2$s %3$s %4$s\n") % sinceProgramStart.count() %
    severityToString(sev) % m_scope % msg;
}
