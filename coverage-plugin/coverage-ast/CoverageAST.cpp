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

            //Handling function body, which is a CompoundStmt
            handleCompoundStmt(dyn_cast<CompoundStmt>(st));
            llvm::errs() << "\n\n";
            func->dump();
            st->viewAST();

            return true;
        }

    private:
        ASTContext *Context;
        void handleStmt(Stmt *st);
        void handleCompoundStmt(CompoundStmt *st);
        void handleDeclStmt(DeclStmt *st);
        void handleBinOpStmt(BinaryOperator *st);
        void handleForStmt(ForStmt *st);

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



void CoverageVisitor::handleStmt(Stmt *st) {
    switch (st->getStmtClass()) {
        // ToDo: recheck what kinds of statements need to handle, currently focusing on array related binary operators
        // ToDo: DeclStmt may define new variables, need to save them to the final graph table;
        case Stmt::DeclStmtClass:
            DEBUG_WITH_TYPE("Coverage-handleStmt", llvm::errs() << "meeting a Decl statement.\n");
            handleDeclStmt(dyn_cast<DeclStmt>(st));
            break;

            // it could have CompoundStmt, which in turn has ForStmt
        case Stmt::IfStmtClass:
            DEBUG_WITH_TYPE("Coverage-handleStmt", llvm::errs() << "meeting a If statement.\n");
            break;

            // Most probability has array operations;
        case Stmt::ForStmtClass:
            handleForStmt(dyn_cast<ForStmt>(st));
            DEBUG_WITH_TYPE("Coverage-handleStmt", llvm::errs() << "meeting a For statement.\n");
            break;

        case Stmt::CompoundStmtClass:
            handleCompoundStmt(dyn_cast<CompoundStmt>(st));
            break;

            // This is for function call
        case Stmt::CallExprClass:
            break;

            // Array Operations normally happens here
        case Stmt::BinaryOperatorClass:
            handleBinOpStmt(dyn_cast<BinaryOperator>(st));
            break;

        case Stmt::ArraySubscriptExprClass:
            llvm::errs() << "\n\n Array Subscript Expr: \n";
            st->dump();
            llvm::errs() << "\n";
            break;

        default:
            DEBUG_WITH_TYPE("Coverage-handleStmt",llvm::errs() << "meeting a statement:" << st->getStmtClassName() << "\n");
            break;
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
                llvm::errs() << "new var (appending to table later):" << var->getName() << "\n";

                // Var declaration has initial value;
                if (var->hasInit()) {
                    expr = var->getInit();
                    switch (expr->getStmtClass()) {
                        // initial value is int/float/string/char constant
                        case Stmt::IntegerLiteralClass:
                            llvm::errs() << "const (integer): ";
                            dyn_cast<IntegerLiteral>(expr)->getValue().dump();
                            llvm::errs() << "\n";
                            break;
                        case Stmt::FloatingLiteralClass:
                            llvm::errs() << "const (float): "
                                         << dyn_cast<FloatingLiteral>(expr)->getValue().convertToFloat() << "\n";
                            break;
                        case Stmt::CharacterLiteralClass:
                            llvm::errs() << "const (char): "
                                         << dyn_cast<CharacterLiteral>(expr)->getValue() << "\n";
                            break;
                        case Stmt::StringLiteralClass:
                            llvm::errs() << "const (String): "
                                         << dyn_cast<StringLiteral>(expr)->getBytes() << "\n";
                            break;

                            // initializing with single variable. e.g. int i = j;
                        case Stmt::ImplicitCastExprClass:
                            declref = dyn_cast<DeclRefExpr>(dyn_cast<ImplicitCastExpr>(expr)->getSubExpr());
                            llvm::errs() << "Expression: " << declref->getDecl()->getName() << "\n";
                            break;

                            // initializing with expression. e.g. int i = j+k;
                        case Stmt::BinaryOperatorClass:
                            handleBinOpStmt(dyn_cast<BinaryOperator>(expr));
                            break;
                        default:
                            llvm::errs() << "Unprocessed: \n";
                            expr->dump();
                            llvm::errs() << "\n\n";
                    }
                }

                break;
                // Skip other Declaration type, e.g. FunctionDel
            default:
                continue;
        }
    }
}

void CoverageVisitor::handleBinOpStmt(BinaryOperator *st) {
    Expr *LHS, *RHS;
    VarDecl * LHSVar, * RHSVar;
    DeclRefExpr *DeclLHS, *DeclRHS;

    st->dump();
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
            llvm::errs() << "Operator: " << st->getOpcodeStr();
            LHS = st->getLHS();
            RHS = st->getRHS();
            DeclLHS = dyn_cast<DeclRefExpr>(dyn_cast<ImplicitCastExpr>(LHS)->getSubExpr());
            DeclRHS = dyn_cast<DeclRefExpr>(dyn_cast<ImplicitCastExpr>(RHS)->getSubExpr());
            LHSVar = dyn_cast<VarDecl>(DeclLHS->getDecl());
            RHSVar = dyn_cast<VarDecl>(DeclRHS->getDecl());

            llvm::errs() << "(LHS: " << LHSVar->getName() << "), " << "(RHS: " << RHSVar->getName() << ")\n\n";

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
            LHS = st->getLHS();
            LHSVar = dyn_cast<VarDecl>(dyn_cast<DeclRefExpr>(LHS)->getDecl());
            RHS = st->getRHS();

            llvm::errs() << "Operator: " << st->getOpcodeStr() << "\n";
            llvm::errs() << "(LHS:" << LHSVar->getName() << "), (RHS:";

            switch (RHS->getStmtClass()) {
                case Stmt::BinaryOperatorClass:
                    handleBinOpStmt(dyn_cast<BinaryOperator>(RHS));
                    break;
                default:
                    RHS->dump();
                    llvm::errs() << "\n\n";
            }
            llvm::errs() << ")\n\n";
            break;

        case BO_Comma:
            break;
    }
    llvm::errs() << "-------------------\n\n";
}
void CoverageVisitor::handleForStmt(ForStmt *st) {
    // body of a for loop is also a CompoundStmt;
    CompoundStmt *body = dyn_cast<CompoundStmt>(st->getBody());
    DEBUG_WITH_TYPE("Coverage", llvm::errs() << "For body:\n");
    DEBUG_WITH_TYPE("Coverage", body->dump());
    handleCompoundStmt(body);
}

void CoverageVisitor::handleCompoundStmt(CompoundStmt *st) {
    assert(st != NULL);
    for (CompoundStmt::body_iterator it = st->body_begin(), end = st->body_end();
         it != end; it++) {
        Stmt * tmp = *it;
        handleStmt(tmp);
    }
}
