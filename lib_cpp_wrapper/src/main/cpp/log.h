//
// Created by YongGyu Lee on 2021/11/27.
//

#ifndef LOG_H_
#define LOG_H_

#include <android/log.h>

#include <ios>
#include <sstream>
#include <type_traits>
#include <utility>

#define ANDROID_LOG_TAG ""

namespace seeso {

namespace detail {

template<typename...>
using void_t = void;

template<typename T, typename = void>
struct is_printable : std::false_type {};

template<typename T>
struct is_printable<T, void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>>
    : std::true_type {};

template<typename ...B> struct conjunction;
template<> struct conjunction<> : std::true_type {};
template<typename B1> struct conjunction<B1> : B1 {};
template<typename B1, typename ...BN> struct conjunction<B1, BN...> :
    std::conditional<bool(B1::value), conjunction<BN...>, B1>::type {};

} // namespace detail


class Logger {
 public:
  template<typename ...Args>
  typename std::enable_if<detail::conjunction<detail::is_printable<Args>...>::value>::type
  print(android_LogPriority priority, const Args&... arg) {
    auto ss = default_stream();
    print_impl(ss, arg...);
    const auto str = ss.str();

    __android_log_print(priority, ANDROID_LOG_TAG, "%s", str.c_str());
  }

 private:
  template<typename Arg>
  inline void print_impl(std::ostream& os, const Arg& arg) {
    os << arg;
  }

  template<typename Arg, typename ...Args>
  void print_impl(std::ostream& os, const Arg& arg, const Args&... args) {
    print_impl(os, arg);
    print_impl(os, " ");
    print_impl(os, args...);
  }

  std::stringstream default_stream() {
    std::stringstream ss;
    ss << std::boolalpha;
    return ss;
  }
};

const struct {
  template<typename ...Args>
  typename std::enable_if<detail::conjunction<detail::is_printable<Args>...>::value>::type
  v(const Args&... args) const { Logger().print(ANDROID_LOG_VERBOSE, args...); }

  template<typename ...Args>
  typename std::enable_if<detail::conjunction<detail::is_printable<Args>...>::value>::type
  d(const Args&... args) const { Logger().print(ANDROID_LOG_DEBUG, args...); }

  template<typename ...Args>
  typename std::enable_if<detail::conjunction<detail::is_printable<Args>...>::value>::type
  i(const Args&... args) const { Logger().print(ANDROID_LOG_INFO, args...); }

  template<typename ...Args>
  typename std::enable_if<detail::conjunction<detail::is_printable<Args>...>::value>::type
  w(const Args&... args) const { Logger().print(ANDROID_LOG_WARN, args...); }

  template<typename ...Args>
  typename std::enable_if<detail::conjunction<detail::is_printable<Args>...>::value>::type
  e(const Args&... args) const { Logger().print(ANDROID_LOG_ERROR, args...); }

  template<typename ...Args>
  typename std::enable_if<detail::conjunction<detail::is_printable<Args>...>::value>::type
  f(const Args&... args) const { Logger().print(ANDROID_LOG_FATAL, args...); }
} Log;

#define LOG_ERROR_HERE                                            \
    __android_log_print(ANDROID_LOG_ERROR,                        \
                        ANDROID_LOG_TAG,                          \
                        "Error at %s, %s, line %d",               \
                        __FILE__, __PRETTY_FUNCTION__, __LINE__)

} // namespace seeso

#endif // LOG_H_
