#include "base/ast.hpp"
#include "base/format.hpp"
#include <exception>

namespace {
class NotImplemented : public std::logic_error {
  using base_t = std::logic_error;

public:
  NotImplemented(char const *const function)
      : base_t{"Function not yet implemented" + std::string(function)} {}
};
} // namespace

namespace eta::ast {
void BaseAstNode::stream_each_member(const StreamKeyValueFnT & /*fn*/) const {}

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

void UseAstNode::stream_each_member(const StreamKeyValueFnT &fn) const {
  fn([](LoggerT &logger) { logger << "identifier"; },
     [self = this](LoggerT &logger) {
       logger << util::to_wstring(self->m_identifier.c_str());
     },
     true);
}

const char *UseAstNode::get_name() const { return "UseAstNode"; }

void MethodCallAstNode::stream_each_member(const StreamKeyValueFnT &fn) const {
  fn([](LoggerT &logger) { logger << "identifier"; },
     [self = this](LoggerT &logger) {
       logger << util::to_wstring(self->m_identifier.c_str());
     },
     m_arguments.empty());
  for (size_t i = 0; i < m_arguments.size(); ++i) {
    fn([i = i](LoggerT &logger) { logger << "argument_" << i; },
       [self = this, i = i](LoggerT &logger) {
         logger << self->m_arguments[i].get();
       },
       i + 1 == m_arguments.size());
  }
}

const char *MethodCallAstNode::get_name() const { return "MethodCallAstNode"; }

void ExprAstNode::stream_each_member(const StreamKeyValueFnT &fn) const {
  fn([](LoggerT &logger) { logger << "expr"; },
     [self = this](LoggerT &logger) {
       std::visit([&logger](auto &&expr) -> void { logger << expr; },
                  self->m_expr);
     },
     true);
}

const char *ExprAstNode::get_name() const { return "ExprAstNode"; }

void ValueExprAstNode::stream_each_member(const StreamKeyValueFnT &fn) const {
  fn([](LoggerT &logger) { logger << "value"; },
     [self = this](LoggerT &logger) {
       std::visit(
           [&logger](auto &&expr) -> void {
             if constexpr (std::is_same_v<int64_t,
                                          std::decay_t<decltype(expr)>>) {
               logger << "INT_CHAR_LITERAL(";
             } else if constexpr (std::is_same_v<
                                      std::wstring,
                                      std::decay_t<decltype(expr)>>) {
               logger << "STRING_LITERAL(";
             }
             logger << expr << ")";
           },
           self->m_value);
     },
     true);
}

const char *ValueExprAstNode::get_name() const { return "ValueExprAstNode"; }

const char *ArithBinOpExprAstNode::get_name() const {
  return "ArithBinOpExprAstNode";
}

const char *ArithBinOpExprAstNode::operator_to_str(BinaryArithOperators oper) {
  switch (oper) {
  case BinaryArithOperators::minus:
    return "minus";
  case BinaryArithOperators::mul:
    return "mul";
  case BinaryArithOperators::highmul:
    return "highmul";
  case BinaryArithOperators::div:
    return "div";
  case BinaryArithOperators::mod:
    return "mod";
  case BinaryArithOperators::add:
    return "add";
  }
  return "unknown";
}

const char *LogicalBinOpExprAstNode::get_name() const {
  return "LogicalBinOpExprAstNode";
}

const char *
LogicalBinOpExprAstNode::operator_to_str(BinaryLogicalOperators oper) {
  switch (oper) {
  case BinaryLogicalOperators::l_than:
    return "l_than";
  case BinaryLogicalOperators::leq_than:
    return "leq_than";
  case BinaryLogicalOperators::geq_than:
    return "geq_than";
  case BinaryLogicalOperators::g_than:
    return "g_than";
  case BinaryLogicalOperators::eq:
    return "eq";
  case BinaryLogicalOperators::neq:
    return "neq";
  case BinaryLogicalOperators::land:
    return "land";
  case BinaryLogicalOperators::lor:
    return "lor";
  }
  return "unknown";
}

void UnaryMinusOperatorAstNode::stream_each_member(
    const StreamKeyValueFnT &fn) const {
  fn([](LoggerT &logger) { logger << "operand"; },
     [self = this](LoggerT &logger) { logger << self->m_operand; }, true);
}
const char *UnaryMinusOperatorAstNode::get_name() const {
  return "UnaryMinusOperatorAstNode";
}

void UnaryLnotOperatorAstNode::stream_each_member(
    const StreamKeyValueFnT &fn) const {
  fn([](LoggerT &logger) { logger << "operand"; },
     [self = this](LoggerT &logger) { logger << self->m_operand; }, true);
}
const char *UnaryLnotOperatorAstNode::get_name() const {
  return "UnaryLnotOperatorAstNode";
}

} // namespace eta::ast