/*
 * Header file del parser.c
 */

/*
 * Definizione metodi e tipi
 */


int yylex();

void match(int), next(), parserror();

Pnode nontermnode(Nonterminal),
        idnode(),
        opnode(Typenode),
        keynode(Typenode),
        charconstnode(),
        intconstnode(),
        realconstnode(),
        strconstnode(),
        boolconstnode(),
        newnode(Typenode),
        program(),
        module_decl(),
        opt_param_list(),
        param_list(),
        param_decl(),
        type(),
        opt_var_sect(),
        decl_list(),
        decl(),
        id_list(),
        opt_const_sect(),
        const_list(),
        const_decl(),
        opt_module_list(),
        module_body(),
        stat_list(),
        stat(),
        assign_stat(Pnode),
        if_stat(),
        opt_elseif_stat_list(),
        opt_else_stat(),
        while_stat(),
        return_stat(),
        opt_expr(),
        read_stat(),
        write_stat(),
        expr_list(),
        expr(),
        expr1(),
        bool_op(),
        bool_term(),
        rel_op(),
        rel_term(),
        rel_term1(),
        low_binop(),
        low_term(),
        low_term1(),
        high_binop(),
        factor(),
        unary_op(),
        constant(),
        module_call(Pnode),
        opt_expr_list(),
        cond_expr(),
        opt_elseif_expr_list(),
        parse();

