#pragma once

#include "base/logger.hpp"

#include <functional>
#include <type_traits>

namespace eta::ast {

struct BaseNodeArgs {};

class BaseAstNode;

template <typename DerivedAstNodeT>
using OstreamRval_t =
    std::enable_if_t<std::is_base_of_v<BaseAstNode, DerivedAstNodeT>,
                     LoggerT &>;

class BaseAstNode {
protected:
  using StreamFieldFnT = std::function<void(LoggerT &)>;
  using StreamKeyValueFnT =
      std::function<void(const StreamFieldFnT &, const StreamFieldFnT &, bool)>;

  virtual void stream_each_member(const StreamKeyValueFnT &fn) const;
  virtual const char *get_name() const;

  template <typename DerivedAstNodeT>
  friend OstreamRval_t<DerivedAstNodeT>
  operator<<(LoggerT &logger, const DerivedAstNodeT &derivedNode);

  void ostream_helper(LoggerT &logger) const;
};

template <typename DerivedAstNodeT>
OstreamRval_t<DerivedAstNodeT> operator<<(LoggerT &logger,
                                          const DerivedAstNodeT &derivedNode) {
  derivedNode.ostream_helper(logger);
  return logger;
}

} // namespace eta::ast