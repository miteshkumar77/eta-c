package(default_visibility = ["//visibility:public"])

cc_library(
  name = "base",
  srcs = ["ast.cpp", "defs.cpp", "format.cpp"],
  hdrs = ["defs.hpp", "ast.hpp", "logger.hpp", "format.hpp"],
)

cc_binary(
  name = "main_exe",
  srcs = ["main.cpp"],
  deps = [":base"],
)