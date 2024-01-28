#include "ast.hpp"

namespace eta::ast {
void BaseAstNode::stream_each_member(const StreamKeyValueFnT & /*fn*/) const {}

const char *BaseAstNode::get_name() const {
  static constexpr char name[] = "BaseAstNode";
  return name;
}

void BaseAstNode::ostream_helper(LoggerT &logger) const {
  logger << this->get_name() << "{";
  stream_each_member([&logger](const StreamFieldFnT &pKey,
                               const StreamFieldFnT &pVal, bool isLast) {
    logger << ".";
    pKey(logger);
    logger << "=";
    pVal(logger);
    if (!isLast)
      logger << ", ";
  });
  logger << "}";
}
} // namespace eta::ast