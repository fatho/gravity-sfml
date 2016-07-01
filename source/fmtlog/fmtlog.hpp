/*! \file
 *  \brief Defines a simple logging interface.
 */
#pragma once

#include <boost/format.hpp>
#include <boost/type_index.hpp>
#include <string>

/*! \brief Emits a log message on the logger \p lg with severity \p sev.
 *
 *  The log message and formatting arguments are specified in the varargs
 *  part of the macro. At least the formatting string must always be specified.
 *  It is only part of the varargs, because forwarding empty varargs to a C++ function
 *  leads to problems some compilers.
 *
 *  \param lg the logger to send the message to.
 *  \param sev the severity of the message.
 */
#define LOG_GENERAL(lg, sev, ...)                                                                  \
  static_cast<::fmtlog::Log>(lg).message(sev, __FILE__, __LINE__, __VA_ARGS__)
/// Emits a debug message using \ref LOG_GENERAL.
#define LOG_DEBUG(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Debug, __VA_ARGS__)
/// Emits an info message using \ref LOG_GENERAL.
#define LOG_INFO(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Info, __VA_ARGS__)
/// Emits a warning message using \ref LOG_GENERAL.
#define LOG_WARN(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Warning, __VA_ARGS__)
/// Emits a error message using \ref LOG_GENERAL.
#define LOG_ERROR(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Error, __VA_ARGS__)
/// Emits a fatal error message using \ref LOG_GENERAL.
#define LOG_FATAL(lg, ...) LOG_GENERAL(lg, ::fmtlog::Severity::Fatal, __VA_ARGS__)

/*! \brief Namespace containing the logging functions.
 */
namespace fmtlog {

/*! \brief Severity classes of log messages.
 */
enum class Severity {
  /// Indicates messages only relevant for debugging/traces.
  Debug,
  /*! \brief Indicates messages informing about the general program flow.
   *
   *  This should be used for messages informing about rare, but expected events.
   */
  Info,
  /*! \brief Indicates messages about unexpected, but easily recoverable events.
   *
   *  An exemplary use would be invalid values in a config files that can easily be recovered.
   */
  Warning,
  /*! \brief Indicates messages about recoverable errors that can still be handled in a user
   * friendly way.
   *
   *  An example for events in this class are corrupted content files. They might prevent the game
   * from
   *  progressing further, but the user can be informed about this condition and be asked to
   * reinstall the game.
   */
  Error,
  /*! \brief Indicates messages about fatal (unrecoverable) program conditions.
   *
   *  This should be used when internal invariants have become corrupted,
   *  or important system calls have failed.
   */
  Fatal
};

/*! \brief A scoped log target.
 */
class Log {
public:
  /// Initializes the log scope with the name of a type.
  template <typename T>
  Log() : Log(boost::typeindex::type_id<T>().pretty_name()) {}

  /*! Initializes the log scope with a user defined name.
   * \param scope the scope's name
   */
  Log(std::string scope);

  /*! \brief Emits a log message.
   *
   *  \param sev the severity of the message
   *  \param filename the name of the source file containing the log invocation
   *  \param line the line number of the log invocation
   *  \param msg the log message.
   */
  void message(Severity sev, const char* filename, int line, const std::string& msg);

  /*! \brief Emits a formatted log message.
   *
   *  The formatting relies on the Boost format library.
   *  \param sev the severity of the message
   *  \param filename the name of the source file containing the log invocation
   *  \param line the line number of the log invocation
   *  \param fmt the format string for producing the log message.
   *  \param args formatting arguments.
   */
  template <typename... Args>
  void message(Severity sev, const char* filename, int line, const std::string& fmt,
               Args&&... args) {
    boost::format f(fmt);
    using unroll = int[];
    static_cast<void>(unroll{0, (f % std::forward<Args>(args), 0)...});
    message(sev, filename, line, boost::str(f));
  }

private:
  /// the name of the log scope
  std::string m_scope;
};
}
