cc_library(
    name = "test_lexer",
    srcs = ["test_lexer.cpp"],
    hdrs = ["test_lexer.hpp"],
    deps = [
        "//base",
        "//lexer:lib_eta_lexer",
        "//parser:lib_eta_parser_utils",
        "@boost//:iostreams",
    ],
)

cc_test(
    name = "lexer_test",
    size = "small",
    srcs = ["lexer_Test.cpp"],
    data = glob(['data/*.eta']),
    deps = [
        ":test_lexer",
        "//base",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@bazel_tools//tools/cpp/runfiles",
    ],
)

cc_binary(
  name = "main_exe",
  srcs = ["main.cpp"],
  data = glob(['data/*.eta']),
  deps = [
    "//base",
    ":test_lexer",
    "@bazel_tools//tools/cpp/runfiles",
  ],
)