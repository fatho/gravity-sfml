#pragma once

namespace octo {
namespace util {

/*! \brief Encapsulates values that are interpolated between two points in time.
 *
 *  It stores the current and previous value of some varying quantity, and an
 *  interpolated value at some point in between.
 *
 *  The underlying type \p T must support right multiplication with \c float values
 *  and addition of two \p T values.
 *  Furthermore, it must be copy-assignable.
 *
 *  \tparam T the underlying interpolated type. Required to be a vector space with float scalars.
 */
template<typename T>
class Interpolated {
public:
  /*! \brief Intializes the current, previous and interpolated value with the same value.
   *  \param value the initial value.
   */
  Interpolated(const T& value) {
    reset(value);
  }

  /*! \brief A reference to the current value.
   *  \returns a modifiable reference to the current value.
   *  \note Modifying the returned reference does not update the \ref interpolated value.
   *  That is done explicitly via \ref interpolate.
   */
  T& current() {
    return m_current;
  }

  /*! \brief A const-reference to the current value.
   *  \returns a const-reference to the current value.
   */
  const T& current() const {
    return m_current;
  }

  /*! \brief A reference to the previous value.
   *  \returns a modifiable reference to the previous value.
   *  \note Modifying the returned reference does not update the \ref interpolated value.
   *  That is done explicitly via \ref interpolate.
   */
  T& previous() {
    return m_previous;
  }

  /*! \brief A const-reference to the previous value.
   *  \returns a const-reference to the previous value.
   */
  const T& previous() const {
    return m_previous;
  }

  /*! \brief A reference to the interpolated value.
   *  \returns a modifiable reference to the interpolated value.
   */
  T& interpolated() {
    return m_interpolated;
  }

  /*! \brief A const-reference to the interpolated value.
   *  \returns a const-reference to the interpolated value.
   */
  const T& interpolated() const {
    return m_interpolated;
  }

  /*! \brief Interpolates between the previous and current value and stores the result.
   *
   *  Performs a linear interpolation between \ref previous and \ref current with blend factor \p alpha.
   *
   *  \param alpha the blend factor, zero leads to the previous value, one to the current value.
   */
  void interpolate(float alpha) {
    m_interpolated = m_previous * (1 - alpha) + m_current * alpha;
  }

  /*! \brief Resets all three components to the same value.
   *  \param value the new current, previous and interpolated value.
   */
  void reset(const T& value) {
    m_current = m_previous = m_interpolated = value;
  }
private:
  /// previous value
  T m_previous;
  /// current value
  T m_current;
  /// interpolated value
  T m_interpolated;
};

}
}
