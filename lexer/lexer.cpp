#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ==================================
// Lexer
// ==================================

enum Token
{
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,
};

static std::string IdentifierStr; // Filled in tok_identifier
static double NumVal;             // Filled in tok_number

/// gettok - Return the next token from standard input.
static int gettok()
{
    static int LastChar = '';

    // Skip any whitespace.
    while (isspace(LastChar)) // memo: isspace ... stdio, 空白文字判定関数
        LastChar = getchar(); // memo: getchar ... stdio, 標準入力から1文字受け取る

    // identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(LastChar))
    { // memo: isalpha ... ctype, 引数が英字か判定, [a-zA-Z]
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar()))) // memo: isalnum ... ctype, 引数が英数字か判定, [a-zA-Z0-9]
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }

    // Number: [0-9.]+
    if (isdigit(LastChar) || LastChar == '.')
    { // memo: isdigit ... ctype, 数字判定
        std::string NumStr;
        do
        {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr); // memo: strtod ... stdlib, 文字列をdoubleに変換, endp = nullの場合エラー処理skip
        return tok_number;
    }

    // Comment until end of line.
    if (LastChar == '#')
    {
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file. Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
