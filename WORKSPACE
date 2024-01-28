workspace(name = "etac")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_googletest",
    strip_prefix = "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
    urls = ["https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"],
)

http_archive(
    name = "com_github_nelhage_rules_boost",
    strip_prefix = "rules_boost-f8c0ca01016abafed8ce145a554fb1a5f52f06e6",
    url = "https://github.com/nelhage/rules_boost/archive/f8c0ca01016abafed8ce145a554fb1a5f52f06e6.tar.gz",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()