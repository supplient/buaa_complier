#include "Statement.h"
#include "StatementList.h"
#include "Expression.h"
#include "FuncCallExp.h"
#include "Condition.h"
#include "SwitchCaseList.h"
#include "SwitchCase.h"

Tuples IfStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    // check cond & state
    if(!cond){
        log::error << "IfStatement: cond is NULL.";
        return tuples;
    }
    if(!state){
        log::error << "IfStatement: state is NULL.";
        return tuples;
    }

    // evaluate cond
    Operand *cond_ord = NULL;
    Tuples cond_tuples = cond->dump(tab, func_name, tvp, &cond_ord);
    tuples.insert(tuples.end(), cond_tuples.begin(), cond_tuples.end());

    // create label tuple as if's exit
    string exit_label_name = NameUtil::genUniBranchLabel();

    Tuple *exit_tuple = new Tuple();
    exit_tuple->op = sem::LABEL;
    exit_tuple->res = new Operand(exit_label_name);

    // branch if zero
    Tuple *bez_tuple = new Tuple();
    bez_tuple->op = sem::BEZ;
    bez_tuple->left = cond_ord;
    bez_tuple->res = new Operand(exit_label_name);
    tuples.push_back(bez_tuple);

    // dump statement
    Tuples state_tuples = state->dump(tab, func_name);
    tuples.insert(tuples.end(), state_tuples.begin(), state_tuples.end());

    // mark exit
    tuples.push_back(exit_tuple);

    return tuples;
}

Tuples WhileStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    // check cond & state
    if(!cond){
        log::error << "WhileStatement: cond is NULL.";
        return tuples;
    }
    if(!state){
        log::error << "WhileStatement: state is NULL.";
        return tuples;
    }

    // create label tuple as entrance
    string ent_label_name = NameUtil::genUniBranchLabel();

    Tuple *ent_tuple = new Tuple();
    ent_tuple->op = sem::LABEL;
    ent_tuple->res = new Operand(ent_label_name);

    // mark entrance
    tuples.push_back(ent_tuple);

    // evaluate cond
    Operand *cond_ord = NULL;
    Tuples cond_tuples = cond->dump(tab, func_name, tvp, &cond_ord);
    tuples.insert(tuples.end(), cond_tuples.begin(), cond_tuples.end());

    // create label tuple as exit
    string exit_label_name = NameUtil::genUniBranchLabel();

    Tuple *exit_tuple = new Tuple();
    exit_tuple->op = sem::LABEL;
    exit_tuple->res = new Operand(exit_label_name);

    // branch if zero
    Tuple *bez_tuple = new Tuple();
    bez_tuple->op = sem::BEZ;
    bez_tuple->left = cond_ord;
    bez_tuple->res = new Operand(exit_label_name);
    tuples.push_back(bez_tuple);

    // dump statement
    Tuples state_tuples = state->dump(tab, func_name);
    tuples.insert(tuples.end(), state_tuples.begin(), state_tuples.end());

    // jump
    Tuple *jmp_tuple = new Tuple();
    jmp_tuple->op = sem::JMP;
    jmp_tuple->res = new Operand(ent_label_name);
    tuples.push_back(jmp_tuple);

    // mark exit
    tuples.push_back(exit_tuple);

    return tuples;
}


Tuples SwitchStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    // check exp & case_list
    if(!exp){
        log::error << "SwtichStatement: exp is NULL.";
        return tuples;
    }
    if(!case_list){
        log::error << "SwtichStatement: case_list is NULL.";
        return tuples;
    }

    // evaluate exp
    Operand *exp_ord = NULL;
    Tuples exp_tuples = exp->dump(tab, func_name, tvp, &exp_ord);
    tuples.insert(tuples.end(), exp_tuples.begin(), exp_tuples.end());

    // create exit label
    string exit_label_name = NameUtil::genUniBranchLabel();

    Tuple *exit_tuple = new Tuple();
    exit_tuple->op = sem::LABEL;
    exit_tuple->res = new Operand(exit_label_name);

    // init cases save vars
    vector<string> case_ent_name_list;
    vector<Tuples> case_tuples_list;
    vector<Operand *> case_ord_list;

    // dump cases to get its case condition value and statement.
    for(SwitchCase *swi_case: case_list->case_list){
        if(!swi_case)
            throw string("A NULL switchcase.");

        // create case entrance tuple
        string case_ent_name = NameUtil::genUniBranchLabel();
        Tuple *case_ent = new Tuple();
        case_ent->op = sem::LABEL;
        case_ent->res = new Operand(case_ent_name);

        Operand *case_ord = NULL;
        Tuples case_tuples = swi_case->dump(tab, func_name, tvp, &case_ord);

        // mark entrance
        case_tuples.insert(case_tuples.begin(),  case_ent);

        // jump to exit
        Tuple *case_jmp_tuple = new Tuple();
        case_jmp_tuple->op = sem::JMP;
        case_jmp_tuple->res = new Operand(exit_label_name);
        case_tuples.push_back(case_jmp_tuple);

        case_ent_name_list.push_back(case_ent_name);
        case_tuples_list.push_back(case_tuples);
        case_ord_list.push_back(case_ord);
    }

    if(case_ord_list.size() != case_ent_name_list.size()
        || case_ent_name_list.size() != case_tuples_list.size()){
        throw string("SwitchStatement: some error happend in dump cases.");
    }

    // dump case compare branch
    for(unsigned int i=0; i<case_ent_name_list.size(); i++){
        Tuple *beq_tuple = new Tuple();
        beq_tuple->op = sem::BEQ;
        beq_tuple->left = exp_ord;
        beq_tuple->right = case_ord_list[i];
        beq_tuple->res = new Operand(case_ent_name_list[i]);
        tuples.push_back(beq_tuple);

        exp_ord = new Operand(*exp_ord);
    }

    // dump default statement
    if(default_state){
        // dump default statement
        Tuples default_tuples = default_state->dump(tab, func_name);
        tuples.insert(tuples.end(), default_tuples.begin(), default_tuples.end());

        // jump to exit
        Tuple *jmp_tuple = new Tuple();
        jmp_tuple->op = sem::JMP;
        jmp_tuple->res = new Operand(exit_label_name);
        tuples.push_back(jmp_tuple);
    }

    // dump cases
    for(Tuples &case_tuples: case_tuples_list){
        tuples.insert(tuples.end(), case_tuples.begin(), case_tuples.end());
    }

    // mark exit label
    tuples.push_back(exit_tuple);

    return tuples;
}


Tuples FuncCallStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    if(!call_exp){
        log::error << "FunCallStatement: call_exp is NULL";
        Tuples tuples;
        return tuples;
    }
    return call_exp->dump(tab, func_name, tvp);
}


Tuples AssignStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    // check whether left value exists
    NameTableEntry *left_entry = tab.lookUp(func_name, ident);
    if(!left_entry){
        errorRepo("Identifier [" + ident + "] used before declare.");
        return tuples;
    }

    // cast to var entry
    VarEntry *left_var = dynamic_cast<VarEntry*>(left_entry);

    // check whether is var
    if(left_var == NULL || left_entry->entry_type != sem::VAR_ENTRY){
        errorRepo("Left_value must be variable.");
        return tuples;
    }

    // check whether is const var
    if(left_var->is_const){
        errorRepo("Left value should not be const.");
        return tuples;
    }

    // check right value
    if(!exp){
        log::error << "AssignMent: exp is NULL!";
        exit(-1);
    }

    // evaluate right value
    Operand *right_ord = NULL;
    Tuples sub_tuples = exp->dump(tab, func_name, tvp, &right_ord);
    tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());

    if(is_array){
        // check dim
        if(left_var->dim == 0){
            errorRepo("Cannot assign a selector for a non-array var.");
            return tuples;
        }

        // evaluate selector
        Operand *sel_ord = NULL;
        Tuples sel_tuples = select->dump(tab, func_name, tvp, &sel_ord);
        tuples.insert(tuples.end(), sel_tuples.begin(), sel_tuples.end());

        // assign
        Tuple *assign_tuple = new Tuple();
        assign_tuple->op = sem::WARRAY;
        assign_tuple->left = sel_ord;
        assign_tuple->right = right_ord;
        assign_tuple->res = new Operand(left_var);
        tuples.push_back(assign_tuple);
    }
    else{

        // check whether is an array name
        if(left_var->dim != 0){
            errorRepo("Cannot assign to an array!");
            return tuples;
        }

        // assign right value to left value
        Tuple *assign_tuple = new Tuple();
        assign_tuple->op = sem::ASSIGN;
        assign_tuple->left = right_ord;
        assign_tuple->res = new Operand(left_var);
        tuples.push_back(assign_tuple);
    }

    return tuples;
}

Tuples InputStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    for(const string &ident: ident_list){
        NameTableEntry *gen_entry = tab.lookUp(func_name, ident);
        // check if declared
        if(!gen_entry){
            errorRepo("Identifier " + ident + " is used before declared.");
            return tuples;
        }

        VarEntry *var_entry = dynamic_cast<VarEntry*>(gen_entry);
        // check if is var
        if(!var_entry || gen_entry->entry_type != sem::VAR_ENTRY){
            errorRepo("Cannot input into a non-var: " + ident);
            return tuples;
        }

        // do input
        Tuple *input_tuple = new Tuple();
        input_tuple->op = sem::INPUT;
        input_tuple->res = new Operand(gen_entry);
        tuples.push_back(input_tuple);
    }

    return tuples;
}


Tuples OutputStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    Operand *left_ord = NULL;
    Operand *right_ord = NULL;

    if(has_string){
        // dump string
        left_ord = new Operand(str_value);
    }

    if(exp_value){
        // dump exp
        Tuples sub_tuples = exp_value->dump(tab, func_name, tvp, &right_ord);
        tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
    }

    Tuple *output_tuple = new Tuple();
    output_tuple->op = sem::OUTPUT;
    output_tuple->left = left_ord;
    output_tuple->right = right_ord;
    tuples.push_back(output_tuple);

    return tuples;
}


Tuples ReturnStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    // check function's return type
    FuncEntry *func_entry = tab.lookUpFunc(func_name);
    if(func_entry->return_type == sym::VOID && exp){
        errorRepo("Function " + func_name + " should have no return value, while a return expression is given.");
        return tuples;
    }
    if(func_entry->return_type != sym::VOID && !exp){
        errorRepo("Function " + func_name + " must have a return value.");
        return tuples;
    }

    if(func_entry->return_type != sym::VOID){
        // function has return value

        // evaluate return value
        Operand *rv_ord = NULL;
        Tuples rv_tuples = exp->dump(tab, func_name, tvp, &rv_ord);
        tuples.insert(tuples.end(), rv_tuples.begin(), rv_tuples.end());

        // assign to return var
        string rv_name;
        switch(func_entry->return_type){
            case sym::INT:
                rv_name = NameUtil::getIntReturnVarName();
                break;
            case sym::CHAR:
                rv_name = NameUtil::getCharReturnVarName();
                break;
            default:
                log::error << "Invalid type for return value: " << func_entry->return_type;
                return tuples;
        }
        Tuple *assign_tuple = new Tuple();
        assign_tuple->op = sem::ASSIGN;
        assign_tuple->res = new Operand(tab.lookUp(func_name, rv_name));
        assign_tuple->left = rv_ord;
        tuples.push_back(assign_tuple);
    }

    // return
    Tuple *return_tuple = new Tuple();
    return_tuple->op = sem::RET;
    tuples.push_back(return_tuple);

    return tuples;
}

Tuples BracedStatement::dump(NameTable &tab, const string &func_name){
    Tuples tuples;

    if(state_list){
        Tuples sub_tuples = state_list->dump(tab, func_name);
        tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
    }
    else
        log::error << "NULL state_list in BracedStatement";

    return tuples;
}
