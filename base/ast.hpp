#pragma once

#include "base/logger.hpp"

#include <functional>
#include <type_traits>
#include <variant>

namespace eta::ast {

struct BaseNodeArgs {};

class BaseAstNode;

template <typename DerivedAstNodeT>
using OstreamRval_t =
    std::enable_if_t<std::is_base_of_v<BaseAstNode, DerivedAstNodeT>,
                     LoggerT &>;

class BaseAstNode {
protected:
  inline explicit BaseAstNode(const BaseNodeArgs &args) {}
  using StreamFieldFnT = std::function<void(LoggerT &)>;
  using StreamKeyValueFnT =
      std::function<void(const StreamFieldFnT &, const StreamFieldFnT &, bool)>;

  virtual void stream_each_member(const StreamKeyValueFnT &fn) const;
  virtual const char *get_name() const = 0; // abstract class

  template <typename DerivedAstNodeT>
  friend OstreamRval_t<DerivedAstNodeT>
  operator<<(LoggerT &logger, const DerivedAstNodeT &derivedNode);

  void ostream_helper(LoggerT &logger) const;
};

class UseAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;
  inline explicit UseAstNode(BaseNodeArgs &baseArgs,
                             const std::string &identifier)
      : base_t{baseArgs}, m_identifier{identifier} {}

  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  const std::string m_identifier;
};

class ValueExprAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;
  inline explicit ValueExprAstNode(BaseNodeArgs &baseArgs,
                                   std::variant<int64_t, std::wstring> &value)
      : base_t{baseArgs}, m_value{value} {}

  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  std::variant<int64_t, std::wstring> m_value;
};

class ExprAstNode;

template <typename DerivedOpAstNodeT> struct OpAstNodeTraits;

class ArithBinOpExprAstNode;
class LogicalBinOpExprAstNode;
template <> struct OpAstNodeTraits<ArithBinOpExprAstNode> {
  enum class operator_t : int8_t { minus, mul, highmul, div, mod, add };
};

template <> struct OpAstNodeTraits<LogicalBinOpExprAstNode> {
  enum class operator_t : int8_t {
    l_than,
    leq_than,
    geq_than,
    g_than,
    eq,
    neq,
    land,
    lor
  };
};

template <typename BaseT> class BinOpExprAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;
  void stream_each_member(const StreamKeyValueFnT &fn) const override;

  BinOpExprAstNode(const BaseNodeArgs &baseArgs,
                   typename OpAstNodeTraits<BaseT>::operator_t oper,
                   const ExprAstNode &lhs, const ExprAstNode &rhs)
      : base_t{baseArgs}, m_operator{oper}, m_lhs{lhs}, m_rhs{rhs} {}

protected:
  const typename OpAstNodeTraits<BaseT>::operator_t m_operator;
  const ExprAstNode &m_lhs;
  const ExprAstNode &m_rhs;
};

template <typename BaseT>
void BinOpExprAstNode<BaseT>::stream_each_member(
    const StreamKeyValueFnT &fn) const {

  fn([](LoggerT &logger) { logger << "operator"; },
     [self = this](LoggerT &logger) {
       logger << BaseT::operator_to_str(self->m_operator);
     },
     false);

  fn([](LoggerT &logger) { logger << "lhs"; },
     [self = this](LoggerT &logger) { logger << self->m_lhs; }, false);

  fn([](LoggerT &logger) { logger << "rhs"; },
     [self = this](LoggerT &logger) { logger << self->m_rhs; }, true);
}

class ArithBinOpExprAstNode : public BinOpExprAstNode<ArithBinOpExprAstNode> {
  using base_t = BinOpExprAstNode<ArithBinOpExprAstNode>;

protected:
  friend base_t;
  using base_t::base_t;
  const char *get_name() const override;
  using BinaryArithOperators =
      typename OpAstNodeTraits<ArithBinOpExprAstNode>::operator_t;

  static const char *operator_to_str(BinaryArithOperators oper);
};

class LogicalBinOpExprAstNode
    : public BinOpExprAstNode<LogicalBinOpExprAstNode> {
  using base_t = BinOpExprAstNode<LogicalBinOpExprAstNode>;

protected:
  using base_t::base_t;
  friend base_t;
  const char *get_name() const override;
  using BinaryLogicalOperators =
      typename OpAstNodeTraits<LogicalBinOpExprAstNode>::operator_t;
  static const char *operator_to_str(BinaryLogicalOperators oper);
};

class UnaryMinusOperatorAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;
  inline explicit UnaryMinusOperatorAstNode(const BaseNodeArgs &baseArgs,
                                            const ExprAstNode &operand)
      : base_t{baseArgs}, m_operand{operand} {}
  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  const ExprAstNode &m_operand;
};

class UnaryLnotOperatorAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;
  inline explicit UnaryLnotOperatorAstNode(const BaseNodeArgs &baseArgs,
                                           const ExprAstNode &operand)
      : base_t{baseArgs}, m_operand{operand} {}
  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  const ExprAstNode &m_operand;
};

class MethodCallAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;
  inline explicit MethodCallAstNode(
      const BaseNodeArgs &baseArgs,
      const std::vector<std::reference_wrapper<ExprAstNode>> &arguments)
      : base_t{baseArgs}, m_arguments{arguments} {}
  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  const std::string m_identifier;
  const std::vector<std::reference_wrapper<ExprAstNode>> m_arguments;
};

class ExprAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;
  template <typename ExprT, typename... ArgsT>
  inline explicit ExprAstNode(const BaseNodeArgs &baseArgs, ArgsT &&... args)
      : base_t{baseArgs}, m_expr{ExprT{std::forward<ArgsT>(args)...}} {}

  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  std::variant<ValueExprAstNode, ArithBinOpExprAstNode, LogicalBinOpExprAstNode,
               UnaryMinusOperatorAstNode, UnaryLnotOperatorAstNode,
               MethodCallAstNode>
      m_expr;
};

template <typename DerivedAstNodeT>
OstreamRval_t<DerivedAstNodeT> operator<<(LoggerT &logger,
                                          const DerivedAstNodeT &derivedNode) {
  derivedNode.ostream_helper(logger);
  return logger;
}

} // namespace eta::ast