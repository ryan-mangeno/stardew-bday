#pragma once

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace stardew {

inline spdlog::logger *logger() {
  static const auto logger = spdlog::stderr_color_mt("stardew");
  return logger.get();
}

} // namespace stardew

#define LOG_TRACE(...)                                                         \
  SPDLOG_LOGGER_CALL(::stardew::logger(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...)                                                         \
  SPDLOG_LOGGER_CALL(::stardew::logger(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)                                                          \
  SPDLOG_LOGGER_CALL(::stardew::logger(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...)                                                          \
  SPDLOG_LOGGER_CALL(::stardew::logger(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)                                                         \
  SPDLOG_LOGGER_CALL(::stardew::logger(), spdlog::level::err, __VA_ARGS__)
#define LOG_FATAL(...)                                                         \
  SPDLOG_LOGGER_CALL(::stardew::logger(), spdlog::level::critical, __VA_ARGS__)
