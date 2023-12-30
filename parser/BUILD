load("@rules_bison//bison:bison.bzl", "bison_cc_library")

cc_library(
    name = "minic_parser_defs",
    srcs = ["parser.cpp"],
    hdrs = ["parser.hpp"],
    deps = ["//base"],
)

bison_cc_library(
    name = "minic_parser",
    src = "minic_parser.yy",
    deps = ["//lexer:minic_lexer", ":minic_parser_defs"],
)

cc_binary(
    name = "minic_parser_exe",
    srcs = ["main.cpp"],
    deps = ["//base", ":minic_parser", ":minic_parser_defs", "//lexer:minic_lexer"],
)