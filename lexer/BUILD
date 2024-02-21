load("@rules_flex//flex:flex.bzl", "flex_cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "eta_lexer_defs",
  srcs = [
    "lexer_defs.cpp",
  ],
  hdrs = [
    "lexer_defs.hpp",
  ],
  deps = [
    "//base",
  ]
)

flex_cc_library(
    name = "lib_eta_lexer",
    src = "eta_lexer.ll",
    flex_options = [
        "--c++",
        "--noyywrap",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//base",
        "//lexer:eta_lexer_defs",
        "//parser:lib_eta_parser",
    ],
)
