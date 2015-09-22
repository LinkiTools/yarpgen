#include "type.h"
#include "variable.h"
#include "node.h"

int main () {
    std::shared_ptr<Type> type;
    type = Type::init (Type::TypeID::UINT);
    type->dbg_dump();

    Variable var = Variable("i", Type::TypeID::UINT, Variable::Mod::NTHNG, false);
    var.set_modifier (Variable::Mod::CONST);
    var.set_value (10);
    var.set_min (5);
    var.set_max (50);
    var.dbg_dump ();

    Array arr = Array ("a", Type::TypeID::UINT, Variable::Mod::CONST_VOLAT, true, 20,
                           Array::Ess::STD_ARR);
    arr.dbg_dump();

    VarUseExpr var_use;
    var_use.set_variable (std::make_shared<Variable> (var));
    std::cout << "VarUseExpr: " << var_use.emit () << std::endl;

    AssignExpr assign;
    assign.set_to (std::make_shared<VarUseExpr> (var_use));
    assign.set_from  (std::make_shared<VarUseExpr> (var_use));
    std::cout << "AssignExpr: " << assign.emit () << std::endl;

    IndexExpr index;
    index.set_base (std::make_shared<Array> (arr));
    index.set_index (std::make_shared<VarUseExpr> (var_use));
    index.set_is_subscr(true);
    std::cout << "IndexExpr: " << index.emit () << std::endl;
    index.set_is_subscr(false);
    std::cout << "IndexExpr: " << index.emit () << std::endl;

    BinaryExpr bin_add;
    bin_add.set_op (BinaryExpr::Op::Add);
    bin_add.set_lhs (std::make_shared<VarUseExpr> (var_use));
    bin_add.set_rhs (std::make_shared<VarUseExpr> (var_use));
    std::cout << "BinaryExpr: " << bin_add.emit () << std::endl;

    AssignExpr assign2;
    assign2.set_to (std::make_shared<IndexExpr> (index));
    assign2.set_from  (std::make_shared<BinaryExpr> (bin_add));
    std::cout << "AssignExpr: " << assign2.emit () << std::endl;

    UnaryExpr unary;
    unary.set_op (UnaryExpr::Op::BitNot);
    unary.set_arg (std::make_shared<IndexExpr> (index));
    std::cout << "UnaryExpr: " << unary.emit () << std::endl;

    ConstExpr cnst;
    cnst.set_type (Type::TypeID::ULLINT);
    cnst.set_data (123321);
    std::cout << "ConstExpr: " << cnst.emit () << std::endl;

    DeclStmnt decl;
    decl.set_data (std::make_shared<Array> (arr));
    std::cout << "DeclStmnt: " << decl.emit () << std::endl;
    decl.set_is_extern (true);
    std::cout << "DeclStmnt: " << decl.emit () << std::endl;

    ExprStmnt ex_st;
    ex_st.set_expr (std::make_shared<AssignExpr>(assign2));
    std::cout << "ExprStmnt: " << ex_st.emit () << std::endl;

    BinaryExpr cond;
    cond.set_op(BinaryExpr::Op::Le);
    cond.set_lhs (std::make_shared<VarUseExpr> (var_use));
    cond.set_rhs (std::make_shared<ConstExpr> (cnst));

    UnaryExpr step;
    step.set_op (UnaryExpr::Op::PreInc);
    step.set_arg (std::make_shared<VarUseExpr> (var_use));

    DeclStmnt iter_decl;
    iter_decl.set_data (std::make_shared<Variable> (var));

    CntLoopStmnt cnt_loop;
    cnt_loop.set_loop_type(LoopStmnt::LoopID::FOR);
    cnt_loop.add_to_body(std::make_shared<ExprStmnt> (ex_st));
    cnt_loop.set_cond(std::make_shared<BinaryExpr> (cond));
    cnt_loop.set_iter(std::make_shared<Variable> (var));
    cnt_loop.set_iter_decl(std::make_shared<DeclStmnt> (iter_decl));
    cnt_loop.set_step_expr (std::make_shared<UnaryExpr> (step));
    std::cout << "CntLoopStmnt: " << cnt_loop.emit () << std::endl;

    TypeCastExpr tc;
    tc.set_type(Type::init(Type::TypeID::ULLINT));
    tc.set_expr(std::make_shared<UnaryExpr> (unary));
    std::cout << "TypeCastExpr: " << tc.emit () << std::endl;

    return 0;
}
