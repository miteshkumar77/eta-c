load("@rules_flex//flex:flex.bzl", "flex_cc_library", "flex")

flex_cc_library(
    name = "minic_lexer",
    src = "minic_lexer.ll",
    deps = ["//base"],
    visibility = ["//visibility:public"],
    flex_options = ["--c++", "--noyywrap"],
)

cc_binary(
    name = "minic_lexer_exe",
    srcs = ["main.cpp"],
    deps = [":minic_lexer"]
)