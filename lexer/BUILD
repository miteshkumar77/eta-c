load("@rules_flex//flex:flex.bzl", "flex_cc_library", "flex")

cc_library(
    name = "minic_lexer_defs",
    srcs = ["lexer.cpp"],
    hdrs = ["lexer.hpp"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "minic_utils",
    srcs = ["utils.cpp"],
    hdrs = ["utils.hpp"],
    visibility = ["//visibility:public"],
)

flex_cc_library(
    name = "minic_lexer",
    src = "minic_lexer.ll",
    deps = [":minic_utils", ":minic_lexer_defs",],
    visibility = ["//visibility:public"],
    flex_options = ["--c++", "--noyywrap",],
)