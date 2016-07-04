/*! \file
 *  \brief Defines a simple logging interface.
 */
#pragma once

#include <boost/format.hpp>
#include <boost/type_index.hpp>
#include <string>

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

/*! \brief Implements internal helper function used by the logging facilites.
 *
 *  Not intended for use outside of fmtlog.
 */
namespace internal {

/*! \brief Passes values to boost::format from a variadic template.
 *
 *  \param fmt a reference to the format object
 *  \param args the format arguments
 */
template<typename... Args>
inline void formatWithArgs(boost::format& fmt, Args&&... args);

/*! \brief Specialization for when there are no arguments.
 *
 *  This function does nothing.
 *  \see void formatWithArgs(boost::format& fmt, Args&&... args)
 */
template<>
inline void formatWithArgs(boost::format&) {}

/*! \brief Specialization for when there is at least one argument.
 *
 *  \param fmt the format object
 *  \param first the first format argument
 *  \param args the remaining format arguments
 */
template<typename First, typename... Args>
inline void formatWithArgs(boost::format& fmt, First&& first, Args&&... args) {
  fmt % std::forward<First>(first);
  formatWithArgs(fmt, std::forward<Args>(args)...);
}
}

/*! \brief Helper class for getting the name of a class.
 *
 *  It is used in a constructor of Log to set the name of the log scope to
 *  the name of type \c C.
 */
template<class C>
struct For {
  /*! \brief Returns the name of the class \c C.
   *  \returns the pretty name (according to Boost) of class \c C.
   */
  std::string operator()() {
    return boost::typeindex::type_id<C>().pretty_name();
  }
};

/*! \brief A scoped log target.
 *
 *  \todo Provide global Log configuration like severity filters.
 *  \todo Implement log file instead of using stderr.
 */
class Log {
public:
  /// Initializes the log scope with the name of type \c C.
  template<class C>
  Log(For<C> f) : Log(f()) {}

  /*! Initializes the log scope with a user defined name.
   * \param scope the scope's name
   */
  Log(const std::string& scope);

  /*! Initializes the log scope with a user defined name.
   * \param scope the scope's name
   */
  Log(const char* scope);

  /*! \brief Emits a log message.
   *
   *  \param sev the severity of the message
   *  \param msg the log message.
   */
  void message(Severity sev, const std::string& msg);

  /*! \brief Emits a formatted log message.
   *
   *  The formatting relies on the Boost format library.
   *  \param sev the severity of the message
   *  \param fmt the format string for producing the log message.
   *  \param args formatting arguments.
   */
  template <typename... Args>
  void message(Severity sev, const std::string& fmt, Args&&... args) {
    boost::format f(fmt);
    internal::formatWithArgs(f, std::forward<Args>(args)...);
    message(sev, boost::str(f));
  }

  /*! \brief Emits a debug log message.
   *
   *  \param fmt the format string for producing the log message.
   *  \param args formatting arguments.
   *  \see Log::message
   */
  template<typename... Args>
  void debug(const std::string& fmt, Args&&... args) {
    message(Severity::Debug, fmt, std::forward<Args>(args)...);
  }

  /*! \brief Emits an info log message.
   *
   *  \param fmt the format string for producing the log message.
   *  \param args formatting arguments.
   *  \see Log::message
   */
  template<typename... Args>
  void info(const std::string& fmt, Args&&... args) {
    message(Severity::Info, fmt, std::forward<Args>(args)...);
  }

  /*! \brief Emits a warning log message.
   *
   *  \param fmt the format string for producing the log message.
   *  \param args formatting arguments.
   *  \see Log::message
   */
  template<typename... Args>
  void warning(const std::string& fmt, Args&&... args) {
    message(Severity::Warning, fmt, std::forward<Args>(args)...);
  }

  /*! \brief Emits an error log message.
   *
   *  \param fmt the format string for producing the log message.
   *  \param args formatting arguments.
   *  \see Log::message
   */
  template<typename... Args>
  void error(const std::string& fmt, Args&&... args) {
    message(Severity::Error, fmt, std::forward<Args>(args)...);
  }

  /*! \brief Emits a fatal log message.
   *
   *  \param fmt the format string for producing the log message.
   *  \param args formatting arguments.
   *  \see Log::message
   */
  template<typename... Args>
  void fatal(const std::string& fmt, Args&&... args) {
    message(Severity::Fatal, fmt, std::forward<Args>(args)...);
  }

private:
  /// the name of the log scope
  std::string m_scope;
};
}
