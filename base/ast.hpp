#pragma once

#include "tokens.hpp"
#include "logger.hpp"

#include <functional>
#include <string>
#include <variant>
#include <vector>

namespace minic::ast
{

  // state that must be present in all AST nodes
  // e.g. line number
  struct BaseNodeArgs
  {
    // for generating ast dump only
    const bool new_line = false;
    const int indent = {0};
  };

  class AstNode
  {
  protected:
    using StreamFieldFnT = std::function<void(LoggerT &)>;
    using StreamKeyValueFnT = std::function<void(const StreamFieldFnT &, const StreamFieldFnT &, bool)>;
    virtual void stream_each_member(const StreamKeyValueFnT &fn) const;
    virtual const char *get_name() const;

    void ostream_helper(LoggerT &logger) const;

  public:
    AstNode(const BaseNodeArgs &args) : m_args{args} {};
    virtual ~AstNode() = default;

    friend LoggerT &operator<<(LoggerT &logger, AstNode &astNode);

  private:
    [[maybe_unused]] BaseNodeArgs m_args;
  };

  enum class basic_type
  {
    int_type = 1,
    double_type = 2,
    bool_type = 3,
    string_type = 4,
  };

  class TypeId : public AstNode
  {
  public:
    explicit TypeId(
        const BaseNodeArgs &args,
        basic_type basic_type_id) : AstNode{args}, m_basic_type_id{basic_type_id} {}

  private:
    void stream_each_member(const StreamKeyValueFnT &fn) const override;
    const char *get_name() const override;

  public:
    friend LoggerT &operator<<(LoggerT &logger, const TypeId &typeId);

  private:
    const basic_type m_basic_type_id;
  };

  class Identifier : public AstNode
  {
  public:
    explicit Identifier(
        const BaseNodeArgs &args,
        const std::string &name) : AstNode{args}, m_name{name} {};

  private:
    void stream_each_member(const StreamKeyValueFnT &fn) const override;
    const char *get_name() const override;

  public:
    friend LoggerT &operator<<(LoggerT &logger, const Identifier &identifier);

  private:
    const std::string m_name;
  };

  class ArbitraryLiteralExpression : public AstNode
  {
  public:
    explicit ArbitraryLiteralExpression(
        const BaseNodeArgs &args,
        const std::string &value) : AstNode{args},
                                    m_value{value} {}

  private:
    void stream_each_member(const StreamKeyValueFnT &fn) const override;
    const char *get_name() const override;

  public:
    friend LoggerT &operator<<(LoggerT &logger, const ArbitraryLiteralExpression &identifier);

  private:
    const std::string m_value;
  };

  class BooleanLiteralExpression : public AstNode
  {
  public:
    explicit BooleanLiteralExpression(
        const BaseNodeArgs &args,
        bool value) : AstNode{args}, m_value{value} {}

  private:
    void stream_each_member(const StreamKeyValueFnT &fn) const override;
    const char *get_name() const override;

  public:
    friend LoggerT &operator<<(LoggerT &logger, const BooleanLiteralExpression &identifier);

  private:
    const bool m_value;
  };

  class Expression : public AstNode
  {
  public:
    using expr_t = std::variant<ArbitraryLiteralExpression, BooleanLiteralExpression>;
    explicit Expression(
        const BaseNodeArgs &args,
        basic_type expr_type, expr_t expr) : AstNode{args},
                                             m_expr{expr} {}

  private:
    void stream_each_member(const StreamKeyValueFnT &fn) const override;
    const char *get_name() const override;

  public:
    friend LoggerT &operator<<(LoggerT &logger, const Expression &expression);

  private:
    const expr_t m_expr;
  };

  // statement
  class Declaration : public AstNode
  {
  public:
    explicit Declaration(const BaseNodeArgs &args, TypeId &&type_id,
                         Identifier &&identifier, Expression &&expression) : AstNode{args}, m_type_id{std::move(type_id)},
                                                                             m_identifier{std::move(identifier)}, m_expr{std::move(expression)} {}

  private:
    void stream_each_member(const StreamKeyValueFnT &fn) const override;
    const char *get_name() const override;

  public:
    friend LoggerT &operator<<(LoggerT &logger, const Declaration &declaration);

  private:
    const TypeId m_type_id;
    const Identifier m_identifier;
    const Expression m_expr;
  };

  // statement
  class Assignment : public AstNode
  {
  public:
    explicit Assignment(const BaseNodeArgs &args,
                        Identifier &&identifier, Expression &&expression) : AstNode{args},
                                                                            m_identifier{std::move(identifier)}, m_expr{std::move(expression)} {}

  private:
    void stream_each_member(const StreamKeyValueFnT &fn) const override;
    const char *get_name() const override;

  public:
    friend LoggerT &operator<<(LoggerT &logger, const Assignment &assignment);

  private:
    const Identifier m_identifier;
    const Expression m_expr;
  };

  class Statement;

  class Block : public AstNode
  {
  public:
    using stmt_list_t = std::vector<Statement>;
    explicit Block(const BaseNodeArgs &args, stmt_list_t &&statement_list)
        : AstNode{args}, m_stmt_list{statement_list} {}

  private:
    void stream_each_member(const StreamKeyValueFnT &fn) const override;
    const char *get_name() const override;

  public:
    friend LoggerT &operator<<(LoggerT &logger, const Block &block);

  private:
    const stmt_list_t m_stmt_list;
  };

  class Statement : public AstNode
  {
  public:
    using statement_t = std::variant<Assignment, Declaration, Block>;
    explicit Statement(const BaseNodeArgs &args,
                       statement_t &&statement) : AstNode{args}, m_stmt{statement} {}

  private:
    void stream_each_member(const StreamKeyValueFnT &fn) const override;
    const char *get_name() const override;

  public:
    friend LoggerT &operator<<(LoggerT &logger, const Statement &statement);

  private:
    const statement_t m_stmt;
  };
}