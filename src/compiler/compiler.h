#pragma once

#include "scanner.h"
#include "streamer.h"
#include "vm.h"

namespace sciche {
class compiler {
private:
    virtual_machine* m_vm;
    scanner m_scanner;
    bool m_eof;

    void parseExpr(void);
    void parsePrimaryExpr(void);
    void parseAssignExpr(void);
    void parseFuncArgsExpr(void);
    void parseFuncParamsExpr(void);
    void parseFuncCallExpr(void);
    void parseLogicalOrExpr(void);
    void parseLogicalAndExpr(void);
    void parseEqualityExpr(void);
    void parseComparisonExpr(void);
    void parseTermExpr(void);
    void parseFactorExpr(void);
    void parseUnaryExpr(void);

    void parseClassDeclStmt(void);
    void parseFuncDeclStmt(void);
    void parseBlockStmt(void);
    void parseIfStmt(void);
    void parseForStmt(void);
    void parseWhileStmt(void);
    void parseReturnStmt(void);
    void parseDecl(void);

public:
    compiler(virtual_machine* vm);
    ~compiler();
    void parseStmt(void);
    void compile(streamer* streamer);
};
}; // namespace sciche
