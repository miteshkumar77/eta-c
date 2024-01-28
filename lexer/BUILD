load("@rules_flex//flex:flex.bzl", "flex_cc_library")

package(default_visibility = ["//visibility:public"])

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
        "//parser:lib_eta_parser",
    ],
)
