load("@rules_bison//bison:bison.bzl", "bison_cc_library")

package(default_visibility = ["//visibility:public"])

bison_cc_library(
  name = "lib_eta_parser",
  src = "eta_parser.ypp",
  deps = ["//base", "//lexer:eta_lexer_defs",],
)

cc_library(
  name = "lib_eta_parser_utils",
  hdrs = ["lib_eta_parser_utils.hpp",],
  srcs = ["lib_eta_parser_utils.cpp",],
  deps = [
    ":lib_eta_parser",
  ],
)