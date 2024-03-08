#pragma once

#include "base/logger.hpp"

#include <cassert>
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
using ExprAstNodeListT = std::vector<std::reference_wrapper<ExprAstNode>>;

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
  inline explicit MethodCallAstNode(const BaseNodeArgs &baseArgs,
                                    const ExprAstNodeListT &arguments)
      : base_t{baseArgs}, m_arguments{arguments} {}

  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  const std::string m_identifier;
  const ExprAstNodeListT m_arguments;
};

/**
 * Dereference:
 * <identifier>\[[<expr 1>][<expr 2>]...[<expr n>]\]
 */
class ReferringExprAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;

  inline explicit ReferringExprAstNode(const BaseNodeArgs &baseArgs,
                                       const std::string &identifier,
                                       const ExprAstNodeListT &indices)
      : base_t{baseArgs}, m_identifier{identifier}, m_indices{indices} {}

  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  const std::string m_identifier;
  const ExprAstNodeListT m_indices;
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
  std::variant<ValueExprAstNode, ReferringExprAstNode, ArithBinOpExprAstNode,
               LogicalBinOpExprAstNode, UnaryMinusOperatorAstNode,
               UnaryLnotOperatorAstNode, MethodCallAstNode>
      m_expr;
};

/**
 * int[x][y][z][][][]...[]
 *
 * <basic_type>{}
 *
 * <array_type>{
 *  <basic_type>
 *  dimension 1, ..., dimension n
 * }
 *
 * <pointer_type>{
 *  {<basic_type>, <array_type>}
 *  depth
 * }
 *
 * <type>{
 *  {<basic_type>, <array_type>, <pointer_type>}
 * }
 */

class BasicTypeAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;

  template <typename UnderlyingT>
  inline explicit BasicTypeAstNode(const BaseNodeArgs &baseArgs)
      : base_t{baseArgs}, m_val{UnderlyingT{}} {}

  struct BoolType {};
  struct IntType {};

  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  const std::variant<BoolType, IntType> m_val;
};

class ArrayTypeAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;

  inline explicit ArrayTypeAstNode(const BaseNodeArgs &baseArgs,
                                   const BasicTypeAstNode &basic_type,
                                   const std::vector<size_t> &dimensions)
      : base_t{baseArgs}, m_basic_type{basic_type}, m_dimensions{dimensions} {
    assert(!m_dimensions.empty());
  }

  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  const BasicTypeAstNode m_basic_type;
  const std::vector<size_t> m_dimensions;
};

class PointerTypeAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;

  template <typename UnderlyingT>
  inline explicit PointerTypeAstNode(const BaseNodeArgs &baseArgs,
                                     const UnderlyingT &pod_type,
                                     const size_t pointer_depth)
      : base_t{baseArgs}, m_val{pod_type}, m_pointer_depth{pointer_depth} {
    assert(m_pointer_depth);
  }

  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  const std::variant<BasicTypeAstNode, ArrayTypeAstNode> m_val;
  const size_t m_pointer_depth;
};

class TypeAnnotationAstNode : public BaseAstNode {
protected:
  using base_t = BaseAstNode;

  template <typename UnderlyingT>
  inline explicit TypeAnnotationAstNode(const BaseNodeArgs &baseArgs,
                                        const UnderlyingT &val)
      : base_t{baseArgs}, m_val{val} {}

  void stream_each_member(const StreamKeyValueFnT &fn) const override;
  const char *get_name() const override;

private:
  std::variant<BasicTypeAstNode, ArrayTypeAstNode, PointerTypeAstNode> m_val;
};

template <typename DerivedAstNodeT>
OstreamRval_t<DerivedAstNodeT> operator<<(LoggerT &logger,
                                          const DerivedAstNodeT &derivedNode) {
  derivedNode.ostream_helper(logger);
  return logger;
}

} // namespace eta::ast