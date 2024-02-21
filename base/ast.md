## AST ##

```c++
                  program ::= (({use}*) ({definition}*))
                      use ::= (<use> <id>)
               definition ::= {procedure} | {method} | {globdecl}
                   method ::= (<id> ({decl}*) ({type}+) {block})
                procedure ::= (<id> ({decl}*) {block})
                 globdecl ::= (<id> {type}) | (<id> {type} {value_expr})
                     decl ::= (<id> {type})
                    block ::= ({stmt}*)
                     type ::= <int_type> | <bool_type> | {type}[{expr}] | {type}[]
               value_expr ::= <char_literal> | <int_literal> | <str_literal>
            array_literal ::= <lbrace> {array_contents} <rbrace>         |
                              <lbrace> {array_contents} <comma> <rbrace> |
                              <lbrace> <rbrace>
 non_empty_array_contents ::= {expr} | {expr} <comma> {array_contents}
                     expr ::= {value_expr} | {array_literal} | {method_call} | {op_expr} | <lparen> {expr} <rparen>
                  op_expr ::=   {bin_minus_expr} | {high_mul_expr} |
                                {mul_expr} | {div_expr} | {mod_expr} | {add_expr} |
                                {leq_than_expr} | {l_than_expr} | {geq_than_expr} |
                                {g_than_expr} | {eq_expr} | {neq_expr} | {land_expr} |
                                {lor_expr} | {unary_lnot_expr} | {unary_minus_expr}
                 lor_expr ::= {expr} <op_lor> {expr}
           bin_minus_expr ::= {expr} <op_minus> {expr}
         unary_minus_expr ::= <op_minus> {expr}
              method_call ::= <identifier> <lparen> {expr}* <rparen>
         ...

                
                  






```