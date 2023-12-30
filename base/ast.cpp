#include "ast.hpp"

namespace
{
  void stream_basic_type(minic::LoggerT &logger, minic::ast::basic_type basic_type)
  {
    switch (basic_type)
    {
    case minic::ast::basic_type::int_type:
      logger << "basic_type::int_type";
      return;
    case minic::ast::basic_type::double_type:
      logger << "basic_type::double_type";
      return;
    case minic::ast::basic_type::bool_type:
      logger << "basic_type::bool_type";
      return;
    case minic::ast::basic_type::string_type:
      logger << "basic_type::string_type";
      return;
    }
  }
}

namespace minic::ast
{
  void AstNode::stream_each_member(const StreamKeyValueFnT &fn) const
  {
  }
  const char *AstNode::get_name() const
  {
    static constexpr char name[] = "AstNode";
    return name;
  }
  void AstNode::ostream_helper(LoggerT &logger) const
  {
    if (this->m_args.new_line)
    {
      logger << '\n';
    }
    for (int i = 0; i < this->m_args.indent; ++i)
    {
      logger << '\t';
    }
    logger << this->get_name() << "{";
    stream_each_member([&logger](const StreamFieldFnT &pKey, const StreamFieldFnT &pVal, bool isLast)
                       {
      logger << ".";
      pKey(logger);
      logger << "=";
      pVal(logger);
      if (!isLast) logger << ", "; });
    logger << "}";

    return;
  }

  LoggerT &operator<<(LoggerT &logger, AstNode &astNode)
  {
    astNode.ostream_helper(logger);
    return logger;
  }

  void TypeId::stream_each_member(const StreamKeyValueFnT &fn) const
  {
    fn([](LoggerT &l)
       { l << "basic_type_id"; },

       [self = this](LoggerT &l)
       {
         stream_basic_type(l, self->m_basic_type_id);
       },
       true);
  }

  const char *TypeId::get_name() const
  {
    static constexpr char name[] = "TypeId";
    return name;
  }

  LoggerT &operator<<(LoggerT &logger, const TypeId &typeId)
  {
    typeId.ostream_helper(logger);
    return logger;
  }

  void Identifier::stream_each_member(const StreamKeyValueFnT &fn) const
  {
    fn([](LoggerT &l)
       { l << "name"; },
       [self = this](LoggerT &l)
       { l << self->m_name; },
       true);
  }

  const char *Identifier::get_name() const
  {
    static constexpr char name[] = "Identifier";
    return name;
  }

  LoggerT &operator<<(LoggerT &logger, const Identifier &identifier)
  {
    identifier.ostream_helper(logger);
    return logger;
  }

  void ArbitraryLiteralExpression::stream_each_member(const StreamKeyValueFnT &fn) const
  {
    fn(
        [](LoggerT &l)
        { l << "value"; },
        [self = this](LoggerT &l)
        { l << self->m_value; },
        true);
  }

  const char *ArbitraryLiteralExpression::get_name() const
  {
    static constexpr char name[] = "ArbitraryLiteralExpression";
    return name;
  }

  LoggerT &operator<<(LoggerT &logger, const ArbitraryLiteralExpression &astNode)
  {
    astNode.ostream_helper(logger);
    return logger;
  }

  void BooleanLiteralExpression::stream_each_member(const StreamKeyValueFnT &fn) const
  {
    fn(
        [](LoggerT &l)
        { l << "value"; },
        [self = this](LoggerT &l)
        { l << std::boolalpha << self->m_value; },
        true);
  }

  const char *BooleanLiteralExpression::get_name() const
  {
    static constexpr char name[] = "BooleanLiteralExpression";
    return name;
  }

  LoggerT &operator<<(LoggerT &logger, const BooleanLiteralExpression &astNode)
  {
    astNode.ostream_helper(logger);
    return logger;
  }

  void Expression::stream_each_member(const StreamKeyValueFnT &fn) const
  {
    fn([](LoggerT &l)
       { l << "expr_type"; },
       [self = this](LoggerT &l)
       {
         stream_basic_type(l, self->m_expr_type);
       },
       false);
    fn([](LoggerT &l)
       { l << "expr"; },
       [self = this](LoggerT &l)
       {
         std::visit([&l](auto &&arg)
                    { l << arg; },
                    self->m_expr);
       },
       true);
  }

  const char *Expression::get_name() const
  {
    static constexpr char name[] = "Expression";
    return name;
  }

  LoggerT &operator<<(LoggerT &logger, const Expression &expression)
  {
    expression.ostream_helper(logger);
    return logger;
  }

  void Declaration::stream_each_member(const StreamKeyValueFnT &fn) const
  {
    fn([](LoggerT &l)
       { l << "type_id"; },
       [self = this](LoggerT &l)
       {
         l << self->m_type_id;
       },
       false);
    fn([](LoggerT &l)
       { l << "identifier"; },
       [self = this](LoggerT &l)
       {
         l << self->m_identifier;
       },
       false);
    fn([](LoggerT &l)
       { l << "expr"; },
       [self = this](LoggerT &l)
       {
         l << self->m_expr;
       },
       true);
  }

  const char *Declaration::get_name() const
  {
    static constexpr char name[] = "Declaration";
    return name;
  }

  LoggerT &operator<<(LoggerT &logger, const Declaration &declaration)
  {
    declaration.ostream_helper(logger);
    return logger;
  }

  void Assignment::stream_each_member(const StreamKeyValueFnT &fn) const
  {
    fn([](LoggerT &l)
       { l << "identifier"; },
       [self = this](LoggerT &l)
       {
         l << self->m_identifier;
       },
       false);
    fn([](LoggerT &l)
       { l << "expr"; },
       [self = this](LoggerT &l)
       {
         l << self->m_expr;
       },
       true);
  }

  const char *Assignment::get_name() const
  {
    static constexpr char name[] = "Assignment";
    return name;
  }

  LoggerT &operator<<(LoggerT &logger, const Assignment &declaration)
  {
    declaration.ostream_helper(logger);
    return logger;
  }

  void Statement::stream_each_member(const StreamKeyValueFnT &fn) const
  {
    fn([](LoggerT &l)
       { l << "stmt"; },
       [self = this](LoggerT &l)
       {
         std::visit([&l](auto &&arg)
                    { l << arg; },
                    self->m_stmt);
       },
       true);
  }

  const char *Statement::get_name() const
  {
    static constexpr char name[] = "Statement";
    return name;
  }

  LoggerT &operator<<(LoggerT &logger, const Statement &statement)
  {
    statement.ostream_helper(logger);
    return logger;
  }

  void Block::stream_each_member(const StreamKeyValueFnT &fn) const
  {
    fn([](LoggerT &l)
       { l << "stmt_list"; },
       [self = this](LoggerT &l)
       {
         l << "{";
         for (size_t i = 0; i < self->m_stmt_list.size(); ++i)
         {
           l << "#[" << i << "]:<"
             << self->m_stmt_list[i] << ">";
         }
         l << "}";
       },
       true);
  }

  const char *Block::get_name() const
  {
    static constexpr char name[] = "Block";
    return name;
  }

  LoggerT &operator<<(LoggerT &logger, const Block &block)
  {
    block.ostream_helper(logger);
    return logger;
  }
}