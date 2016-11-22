//===- PrintFunctionNames.cpp ---------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
//#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"

using namespace clang;

namespace {

    // it stores an array access information, e.g. for ab[2*i+1]
    // StringRef will store 'ab', Expr * will refer to the AST of 2*i+1
    //typedef std::vector<std::pair<StringRef, Expr *>> VarList;
    typedef enum { SCALA, ARRAY } ElemType;
    typedef std::tuple<StringRef, std::string, ElemType> ElemExpr;
    typedef std::vector<ElemExpr> VarList;

    class CoverageVisitor: public RecursiveASTVisitor<CoverageVisitor> {
    public:
        explicit CoverageVisitor(ASTContext *Context): Context(Context) {}

        bool VisitFunctionDecl(FunctionDecl *func) {
            std::string funcName = func->getNameInfo().getName().getAsString();
            llvm::errs() << funcName << "(";
            int param_size = func->getNumParams();

            // ToDo: parameter defines variable used in the function, need to save them to graph table
            for (int i = 0; i < param_size; i++) {
                ParmVarDecl *param = func->getParamDecl(i);
                std::string p_name = param->getNameAsString();
                llvm::errs() << p_name;
                if (i < param_size - 1) {
                    llvm::errs() << ", ";
                } else {
                    llvm::errs() << "):\n";
                }
            }

            if (!func->hasBody()) {
                llvm::errs() << "declaration, no body\n";
                return true;
            }

            // ToDo: handle the function body,
            // The body is a CompoundStmt, which contains multiple other statements,
            // e.g. DeclStmt, ForStmt, ReturnStmt etc. Clang has around 40 types of statements
            // see here: http://clang.llvm.org/doxygen/classclang_1_1Stmt.html
            Stmt * st = func->getBody();
            DEBUG_WITH_TYPE("Coverage", st->viewAST());
            DEBUG_WITH_TYPE("Coverage", st->dump());
            //DEBUG_WITH_TYPE("Coverage-AST",func->dump());
            //DEBUG_WITH_TYPE("Coverage-AST", st->viewAST());
            //func->dump();
            //st->viewAST();

            //Handling function body, which is a CompoundStmt
            handleCompoundStmt(dyn_cast<CompoundStmt>(st));
            llvm::errs() << "\n\n";

            return true;
        }

    private:
        ASTContext *Context;
        void handleCompoundStmt(CompoundStmt *st);
        void handleDeclStmt(DeclStmt *st);
        VarList handleNonAssignBinOpStmt(BinaryOperator *st);
        void handleAssignBinOpStmt(BinaryOperator *st);
        void handleBinOpStmt(BinaryOperator *st);
        ElemExpr handleArraySubscriptExpr(ArraySubscriptExpr *st);
        void handleForStmt(ForStmt *st);
        void handleIfStmt(IfStmt *st);
        void handleWhileStmt(WhileStmt *st);
        void printVarList(VarList result);
        std::string getArrayIndexAsString(BinaryOperator * expr);
        void printBinaryOp(BinaryOperator * expr);
        // handleHSExpr is to handle either LHS or RHS of a Bin Operator;
        VarList handleHSExpr(Expr * HS);

    };

    class CoverageConsumer : public ASTConsumer {
    public:
        explicit CoverageConsumer(ASTContext *Context) : Visitor(new CoverageVisitor(Context)) {}

        virtual void HandleTranslationUnit(ASTContext &Context) {
            Visitor->TraverseDecl(Context.getTranslationUnitDecl());
            return;
        }
    private:
        CoverageVisitor *Visitor;
    };

    class CoverageAction : public PluginASTAction {
    protected:
        ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
            return new CoverageConsumer(&CI.getASTContext());
        }

        bool ParseArgs(const CompilerInstance &CI,
                       const std::vector<std::string>& args) {
            for (unsigned i = 0, e = args.size(); i != e; ++i) {
                llvm::errs() << "Coverage arg = " << args[i] << "\n";

                // Example error handling.
                if (args[i] == "-an-error") {
                    DiagnosticsEngine &D = CI.getDiagnostics();
                    unsigned DiagID = D.getCustomDiagID(DiagnosticsEngine::Error,
                                                        "invalid argument '%0'");
                    D.Report(DiagID) << args[i];
                    return false;
                }
            }
            if (args.size() && args[0] == "help")
                PrintHelp(llvm::errs());

            return true;
        }
        void PrintHelp(llvm::raw_ostream& ros) {
            ros << "Help for PrintFunctionNames plugin goes here\n";
        }

    };

}

static FrontendPluginRegistry::Add<CoverageAction>
        X("coverage", "calculate coverage map");

void CoverageVisitor::handleCompoundStmt(CompoundStmt *st) {
    assert(st != NULL);
    //llvm::errs() << "Stmt size: " << st->size() << "\n";
    int size = 0;

    for (CompoundStmt::body_iterator it = st->body_begin(), end = st->body_end(); it != end; it++) {
        size++;
        Stmt * tmp = *it;

        // ToDo: recheck what kinds of statements need to handle, currently focusing on array related binary operators
        // ToDo: DeclStmt may define new variables, need to save them to the final graph table;
        switch (tmp->getStmtClass()) {
            case Stmt::DeclStmtClass:
                DEBUG_WITH_TYPE("Coverage-handleStmt", llvm::errs() << "meeting a Decl statement.\n");
                handleDeclStmt(dyn_cast<DeclStmt>(tmp));
                break;
            case Stmt::BinaryOperatorClass:
                handleBinOpStmt(dyn_cast<BinaryOperator>(tmp));
                break;
            case Stmt::IfStmtClass:
                // it could have CompoundStmt, which in turn has ForStmt
                DEBUG_WITH_TYPE("Coverage-handleStmt", llvm::errs() << "meeting a If statement.\n");
                //ToDo: adding code to handle if statement if necessary, currently just processing its body
                handleIfStmt(dyn_cast<IfStmt>(tmp));
                break;
            case Stmt::ForStmtClass:
                // Handle For loop statements, it has high probability to have array operations;
                handleForStmt(dyn_cast<ForStmt>(tmp));
                DEBUG_WITH_TYPE("Coverage-handleStmt", llvm::errs() << "meeting a For statement.\n");
                break;
            case Stmt::WhileStmtClass:
                handleWhileStmt(dyn_cast<WhileStmt>(tmp));
                break;
            case Stmt::CompoundAssignOperatorClass:
                handleAssignBinOpStmt(dyn_cast<BinaryOperator>(tmp));
                break;
            default:
                llvm::errs() << "statement not handled:" << tmp->getStmtClassName() << "\n";
                //tmp->dump();
                llvm::errs() << "\n";
        }

        //llvm::errs() << "\n";
    }
}

// 'int i, j, k' is a DeclStmt, which contains 3 VarDecls (Clang type)
// 'int i' is a DeclStmt, which contains just 1 VarDecl.
// In addition to VarDecl, Clang has tens of other Decls e.g. ParamDecl, FunctionDecl, TypeDecl,
// they share same Decl base class, we only care about VarDecl and ParamVarDecl now
void CoverageVisitor::handleDeclStmt(DeclStmt *st) {
    assert(st != NULL);
    // iterate over each variable declaration
    for (DeclStmt::decl_iterator it = st->decl_begin(), end = st->decl_end();
         it != end; it++) {
        Decl * declaration = *it;

        VarDecl *var;
        Expr *expr;
        DeclRefExpr *declref;

        switch (declaration->getKind()) {
            // Function Parameter Variable declaration
            case Decl::ParmVar:
                break;
            // Local/Global Variable declaration.
            case Decl::Var:
                var = dyn_cast<VarDecl>(declaration);
                // ToDo: append to hash table later
                llvm::errs() << "new var:" << var->getName();

                // Var declaration has initial value;
                if (var->hasInit()) {
                    expr = var->getInit();
                    switch (expr->getStmtClass()) {
                        // initial value is int/float/string/char constant
                        case Stmt::IntegerLiteralClass:
                            llvm::errs() << " = "
                                         << dyn_cast<IntegerLiteral>(expr)->getValue().getLimitedValue() << "\n";
                            break;
                        case Stmt::FloatingLiteralClass:
                            llvm::errs() << " = "
                                         << dyn_cast<FloatingLiteral>(expr)->getValue().convertToFloat() << "\n";
                            break;
                        case Stmt::CharacterLiteralClass:
                            llvm::errs() << " = "
                                         << dyn_cast<CharacterLiteral>(expr)->getValue() << "\n";
                            break;
                        case Stmt::StringLiteralClass:
                            llvm::errs() << " = "
                                         << dyn_cast<StringLiteral>(expr)->getBytes() << "\n";
                            break;

                            // initializing with single variable. e.g. int i = j;
                        case Stmt::ImplicitCastExprClass:
                            declref = dyn_cast<DeclRefExpr>(dyn_cast<ImplicitCastExpr>(expr)->getSubExpr());
                            llvm::errs() << " = " << declref->getDecl()->getName() << "\n";
                            break;

                            // initializing with expression. e.g. int i = j+k;
                        case Stmt::BinaryOperatorClass:
                            llvm::errs() << " = ";
                            printBinaryOp(dyn_cast<BinaryOperator>(expr));
                            llvm::errs() << "\n";
                            handleBinOpStmt(dyn_cast<BinaryOperator>(expr));
                            break;
                        default:
                            llvm::errs() << "Unprocessed: \n";
                            expr->dump();
                    }
                }
                break;
                // Skip other Declaration type, e.g. FunctionDel
            default:
                continue;
        }
    }
}

// handle for loop, currently focus on loop body
void CoverageVisitor::handleForStmt(ForStmt *st) {
    // body of a for loop is also a CompoundStmt;
    //llvm::errs() << "for body: " << st->getBody()->getStmtClass() << ", " << st->getBody()->getStmtClassName() << "\n\n";
    CompoundStmt *body = dyn_cast<CompoundStmt>(st->getBody());
    DEBUG_WITH_TYPE("Coverage", llvm::errs() << "For body:\n");
    DEBUG_WITH_TYPE("Coverage", body->dump());
    handleCompoundStmt(body);
}

// handle if statement, currently focus on then/else body
void CoverageVisitor::handleIfStmt(IfStmt *st) {
    // body of a for loop is also a CompoundStmt;
    CompoundStmt *body;
    if (st->getThen()) {
        /*
        llvm::errs() << "if-then: " << st->getThen()->getStmtClass() << ", "
                     << st->getThen()->getStmtClassName() << "\n\n";
        */
        body = dyn_cast<CompoundStmt>(st->getThen());
        DEBUG_WITH_TYPE("Coverage", llvm::errs() << "If then:\n");
        DEBUG_WITH_TYPE("Coverage", body->dump());
        handleCompoundStmt(body);
    }

    if (st->getElse()) {
        /*
        llvm::errs() << "if-else: " << st->getElse()->getStmtClass() << ", "
                     << st->getElse()->getStmtClassName() << "\n\n";
        */
        body = dyn_cast<CompoundStmt>(st->getElse());
        DEBUG_WITH_TYPE("Coverage", llvm::errs() << "If else:\n");
        DEBUG_WITH_TYPE("Coverage", body->dump());
        handleCompoundStmt(body);
    }

}

// handle while loop, currently focus on while body
void CoverageVisitor::handleWhileStmt(WhileStmt *st) {
    //llvm::errs() << "while body: " << st->getBody()->getStmtClass() << ", " << st->getBody()->getStmtClassName() << "\n\n";
    CompoundStmt *body = dyn_cast<CompoundStmt>(st->getBody());
    DEBUG_WITH_TYPE("Coverage", llvm::errs() << "while body:\n");
    DEBUG_WITH_TYPE("Coverage", body->dump());
    handleCompoundStmt(body);

}

// handle assignment statement, it is called by handleBinOpStmt
void CoverageVisitor::handleAssignBinOpStmt(BinaryOperator *st) {
    VarList LHS, RHS;
    LHS = handleHSExpr(st->getLHS());
    RHS = handleHSExpr(st->getRHS());

    printVarList(LHS);
    llvm::errs() << " used:";
    printVarList(RHS);
    llvm::errs() << "\n";
}

// handle non-assignment statement, it is called by handleBinOpStmt and handleHSStmt
VarList CoverageVisitor::handleNonAssignBinOpStmt(BinaryOperator *st) {
    VarList LHS, RHS, ALL;
    LHS = handleHSExpr(st->getLHS());
    RHS = handleHSExpr(st->getRHS());
    ALL.insert(ALL.end(), LHS.begin(), LHS.end());
    ALL.insert(ALL.end(), RHS.begin(), RHS.end());
    return ALL;
}

// handleBinOpStmt extracts operands in an stmt;
void CoverageVisitor::handleBinOpStmt(BinaryOperator *st) {

    switch (st->getOpcode()) {
            // [C++ 5.5] Pointer-to-member operators.
        case BO_PtrMemD:
        case BO_PtrMemI:
            break;

            // [C99 6.5.5] Multiplicative operators.
        case BO_Mul:
        case BO_Div:
        case BO_Rem:
            // [C99 6.5.6] Additive operators.
        case BO_Add:
        case BO_Sub:
            // [C99 6.5.7] Bitwise shift operators.
        case BO_Shl:
        case BO_Shr:
            // [C99 6.5.8] Relational operators.
        case BO_LT:
        case BO_GT:
        case BO_LE:
        case BO_GE:
            // [C99 6.5.9] Equality operators.
        case BO_EQ:
        case BO_NE:
            // [C99 6.5.10] Bitwise AND operator.
        case BO_And:
            // [C99 6.5.11] Bitwise XOR operator.
        case BO_Xor:
            // [C99 6.5.12] Bitwise OR operator.
        case BO_Or:
            // [C99 6.5.13] Logical AND operator.
        case BO_LAnd:
            // [C99 6.5.14] Logical OR operator.
        case BO_LOr:
            handleNonAssignBinOpStmt(st);
            break;

            // [C99 6.5.16] Assignment operators.
        case BO_Assign:
        case BO_MulAssign:
        case BO_DivAssign:
        case BO_RemAssign:
        case BO_AddAssign:
        case BO_SubAssign:
        case BO_ShlAssign:
        case BO_ShrAssign:
        case BO_AndAssign:
        case BO_XorAssign:
        case BO_OrAssign:
            handleAssignBinOpStmt(st);
            break;
        case BO_Comma:
            break;
    }
}

// handleHSExpr: This function is to handle LHS and RHS of a binary
// statement in the AST. This is not to handle array index, which will be
// handled separately.
VarList CoverageVisitor::handleHSExpr(Expr *HS) {
    VarList variables;
    ImplicitCastExpr *ImplicitExprTmp;
    IntegerLiteral *IntTmp;
    DeclRefExpr *DeclRefTmp;
    VarDecl * HSVarTmp;
    Expr * ExprTmp;
    ElemExpr HSVar;

    switch (HS->getStmtClass()) {
        // normal mathematical calculation
        // Simple Scala variable
        case Stmt::DeclRefExprClass:
            HSVarTmp = dyn_cast<VarDecl>(dyn_cast<DeclRefExpr>(HS)->getDecl());
            HSVar = std::make_tuple(HSVarTmp->getName(), "", SCALA);
            variables.push_back(HSVar);
            break;
        // array element: e.g. a[2*i+1]
        case Stmt::ArraySubscriptExprClass:
            HSVar = handleArraySubscriptExpr(dyn_cast<ArraySubscriptExpr>(HS));
            variables.push_back(HSVar);
            break;
        // array or scala variable is wrapped inside implicit cast
        case Stmt::ImplicitCastExprClass:
            ImplicitExprTmp = dyn_cast<ImplicitCastExpr>(HS);
            ExprTmp = ImplicitExprTmp->getSubExpr();
            if (isa<ArraySubscriptExpr>(ExprTmp)) {
                HSVar = handleArraySubscriptExpr(dyn_cast<ArraySubscriptExpr>(ExprTmp));
            } else if (isa<DeclRefExpr>(ImplicitExprTmp->getSubExpr())) {
                DeclRefTmp = dyn_cast<DeclRefExpr>(ExprTmp);
                HSVarTmp = dyn_cast<VarDecl>(DeclRefTmp->getDecl());
                HSVar = std::make_tuple(HSVarTmp->getName(), "", SCALA);
            } else {
                llvm::errs() << "Unprocessed Expression: \n";
                ExprTmp->dump();
                llvm::errs() << "\n";
            }
            variables.push_back(HSVar);
            break;
        // constant integer
        case Stmt::IntegerLiteralClass:
            IntTmp = dyn_cast<IntegerLiteral>(HS);
            HSVar = std::make_tuple(IntTmp->getValue().toString(10, true), "", SCALA);
            variables.push_back(HSVar);
            break;
        // further binary operators;
        case Stmt::BinaryOperatorClass:
            if (dyn_cast<BinaryOperator>(HS)->isAssignmentOp()) {
                llvm::errs() << "case such as a=b=c=4.0 has not been handled well\n";
                handleAssignBinOpStmt(dyn_cast<BinaryOperator>(HS));
            } else {
                variables = handleNonAssignBinOpStmt(dyn_cast<BinaryOperator>(HS));
            }
            break;
        default:
            llvm::errs() << "Other operand in Assign: \n";
            HS->dump();
            llvm::errs() << "\n";
    }

    return variables;

}

ElemExpr CoverageVisitor::handleArraySubscriptExpr(ArraySubscriptExpr *st) {
    StringRef name;
    std::string index;
    ImplicitCastExpr *ImplicitExprTmp;
    DeclRefExpr *DeclRefTmp;
    Expr * ExprTmp;
    ElemExpr element;

    // get LHS, normally it is array name
    ImplicitExprTmp = dyn_cast<ImplicitCastExpr>(st->getLHS());
    DeclRefTmp= dyn_cast<DeclRefExpr>(ImplicitExprTmp->getSubExpr());
    name = dyn_cast<VarDecl>(DeclRefTmp->getDecl())->getName();

    // get RHS, normally it is array index, it could be simple variable, e.g. i,
    // or expression, e.g. 2*i+1;
    ExprTmp = st->getRHS();
    if (isa<ImplicitCastExpr>(ExprTmp)) {
        ImplicitExprTmp = dyn_cast<ImplicitCastExpr>(ExprTmp);
        DeclRefTmp= dyn_cast<DeclRefExpr>(ImplicitExprTmp->getSubExpr());
        index = dyn_cast<VarDecl>(DeclRefTmp->getDecl())->getName().str();
    } else if (isa<BinaryOperator>(ExprTmp)) {
        index = getArrayIndexAsString(dyn_cast<BinaryOperator>(ExprTmp));
    }
    element = std::make_tuple(name, index, ARRAY);
    return element;
}






void CoverageVisitor::printVarList(VarList result) {
    int i, size = result.size();
    llvm::errs() << "(";
    for ( i = 0; i < size; i++) {
        //llvm::errs() << result[i].get(0);
        llvm::errs() << std::get<0>(result[i]);
        if (std::get<2>(result[i]) == ARRAY) {
            //llvm::errs() << "[" << result[i].get(1) << "]";
            llvm::errs() << "[" << std::get<1>(result[i]) << "]";
        }
        if ( i < size - 1) {
            llvm::errs() << ",";
        }
    }
    llvm::errs() <<  ")";
}

std::string CoverageVisitor::getArrayIndexAsString(BinaryOperator *expr) {
    Expr *LHS, *RHS, *tmp;
    DeclRefExpr * DeclRefTmp;
    VarDecl *var;
    IntegerLiteral * IntTmp;
    std::string index;

    LHS = expr->getLHS();

    switch(LHS->getStmtClass()) {
        case Expr::ImplicitCastExprClass:
            tmp = dyn_cast<ImplicitCastExpr>(LHS)->getSubExpr();
            DeclRefTmp = dyn_cast<DeclRefExpr>(tmp);
            var = dyn_cast<VarDecl>(DeclRefTmp->getDecl());
            //llvm::errs() << var->getName();
            index += var->getName().str();
            break;
        case Expr::IntegerLiteralClass:
            IntTmp = dyn_cast<IntegerLiteral>(LHS);
            //llvm::errs() << IntTmp->getValue().getLimitedValue();
            index += IntTmp->getValue().toString(10, true);
            break;
        case Expr::BinaryOperatorClass:
            index += getArrayIndexAsString(dyn_cast<BinaryOperator>(LHS));
            break;
        default:
            llvm::errs() << "PrintBinOp:\n";
            LHS->dump();
            llvm::errs() << "\n";
    }
    //llvm::errs() << expr->getOpcodeStr();
    index += expr->getOpcodeStr().str();

    RHS = expr->getRHS();
    switch(RHS->getStmtClass()) {
        case Expr::ImplicitCastExprClass:
            tmp = dyn_cast<ImplicitCastExpr>(RHS)->getSubExpr();
            DeclRefTmp = dyn_cast<DeclRefExpr>(tmp);
            var = dyn_cast<VarDecl>(DeclRefTmp->getDecl());
            //llvm::errs() << var->getName();
            index += var->getName().str();
            break;
        case Expr::IntegerLiteralClass:
            IntTmp = dyn_cast<IntegerLiteral>(RHS);
            index += IntTmp->getValue().toString(10, true);
            break;
        case Expr::BinaryOperatorClass:
            index += getArrayIndexAsString(dyn_cast<BinaryOperator>(RHS));
            break;
        default:
            llvm::errs() << "PrintBinOp:\n";
            LHS->dump();
            llvm::errs() << "\n";
    }
    return index;
}

// printBinaryOp is to print mathematical expressions e.g. 2*i+1
void CoverageVisitor::printBinaryOp(BinaryOperator *expr) {
    Expr *LHS, *RHS, *tmp;
    DeclRefExpr * DeclRefTmp;
    VarDecl *var;
    IntegerLiteral * IntTmp;
    LHS = expr->getLHS();

    switch(LHS->getStmtClass()) {
        case Expr::ImplicitCastExprClass:
            tmp = dyn_cast<ImplicitCastExpr>(LHS)->getSubExpr();
            DeclRefTmp = dyn_cast<DeclRefExpr>(tmp);
            var = dyn_cast<VarDecl>(DeclRefTmp->getDecl());
            llvm::errs() << var->getName();
            break;
        case Expr::IntegerLiteralClass:
            IntTmp = dyn_cast<IntegerLiteral>(LHS);
            llvm::errs() << IntTmp->getValue().getLimitedValue();
            break;
        case Expr::BinaryOperatorClass:
            printBinaryOp(dyn_cast<BinaryOperator>(LHS));
            break;
        default:
            llvm::errs() << "PrintBinOp:\n";
            LHS->dump();
            llvm::errs() << "\n";
    }
    llvm::errs() << expr->getOpcodeStr();

    RHS = expr->getRHS();
    switch(RHS->getStmtClass()) {
        case Expr::ImplicitCastExprClass:
            tmp = dyn_cast<ImplicitCastExpr>(RHS)->getSubExpr();
            DeclRefTmp = dyn_cast<DeclRefExpr>(tmp);
            var = dyn_cast<VarDecl>(DeclRefTmp->getDecl());
            llvm::errs() << var->getName();
            break;
        case Expr::IntegerLiteralClass:
            IntTmp = dyn_cast<IntegerLiteral>(RHS);
            llvm::errs() << IntTmp->getValue().getLimitedValue();
            break;
        case Expr::BinaryOperatorClass:
            printBinaryOp(dyn_cast<BinaryOperator>(RHS));
            break;
        default:
            llvm::errs() << "PrintBinOp:\n";
            LHS->dump();
            llvm::errs() << "\n";
    }
}
