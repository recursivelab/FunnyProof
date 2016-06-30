/*******************************************************************************
*                                                                              *
*     FunnyProof - Easy for use proof assistant.                               *
*     Copyright (C) 2015  Nedeljko Stefanovic                                  *
*                                                                              *
*     This program is free software: you can redistribute it and/or modify     *
*     it under the terms of version 3 of the GNU General Public License as     *
*     published by the Free Software Foundation.                               *
*                                                                              *
*     This program is distributed in the hope that it will be useful,          *
*     but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*     GNU General Public License for more details.                             *
*                                                                              *
*     You should have received a copy of the GNU General Public License        *
*     along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
*                                                                              *
*******************************************************************************/


#ifndef ERROR_H
#define ERROR_H

#include <string>

enum ExceptionType
{
    UNEXPECTED_END,
    ALPHA_EXPECTED,
    ONE_OF_EXPECTED,
    NAME_EXPECTED,
    CONSTANT_EXPECTED,
    VARIABLE_EXPECTED,
    OPERATION_EXPECTED,
    RELATION_EXPECTED,
    WRONG_ARITY,
    SYMBOL_NOT_FOUND,
    WRONG_TOKEN,
    TERM_EXPECTED,
    EQUALITY_EXPECTED,
    NONEQUALITY_UNEXPECTED,
    EQUALITY_OR_NONEQUALITY_EXPECTED,
    FORMULA_EXPECTED
};

struct Exception
{
    const ExceptionType type;
    std::wstring description;

    Exception(ExceptionType type, const std::wstring &description) :
        type(type),
        description(description)
    {
    }
};

struct UnexpectedEndException : Exception
{
    UnexpectedEndException() :
        Exception(UNEXPECTED_END, L"Unexpected end.")
    {
    }
};

struct AlphaExpectedException : public Exception
{
    AlphaExpectedException() :
        Exception(ALPHA_EXPECTED, L"Letter is expected.")
    {
    }
};

struct OneOfExpectedException : public Exception
{
    const std::wstring chars;

    OneOfExpectedException(const std::wstring &chars) :
        Exception(ONE_OF_EXPECTED, [](const std::wstring &chars) -> std::wstring
    {
        std::wstring result = L"One of ";

        for (size_t i = 0; i<chars.size(); ++i) {
            result += L"\'";
            result += chars[i];
            result += L"\'";

            if (i<chars.size()-1) {
                result += L", ";
            }
        }

        result += L" is expected.";

        return result;
    } (chars)),
        chars(chars)
    {
    }
};

struct NameExpectedException : public Exception
{
    NameExpectedException() :
        Exception(NAME_EXPECTED, L"Name is expected.")
    {
    }
};

struct ConstantExpectedException : public Exception
{
    ConstantExpectedException() :
        Exception(CONSTANT_EXPECTED, L"Constant is expected.")
    {
    }
};

struct VariableExpectedException : public Exception
{
    VariableExpectedException() :
        Exception(VARIABLE_EXPECTED, L"Variable is expected.")
    {
    }
};

struct OperationExpectedException : public Exception
{
    OperationExpectedException() :
        Exception(OPERATION_EXPECTED, L"Operation is expected.")
    {
    }
};

struct RelationExpectedException : public Exception
{
    RelationExpectedException() :
        Exception(OPERATION_EXPECTED, L"Operation is expected.")
    {
    }
};

struct WrongArityException : public Exception
{
    const size_t expectedArity;
    const size_t foundArity;

    WrongArityException(size_t expectedArity, size_t foundArity) :
        Exception(WRONG_ARITY, [](size_t expectedArity, size_t foundArity) -> std::wstring
    {
        std::wstring result = L"Wrong arity. Expected ";

        result += std::to_wstring(expectedArity);
        result += L" but ";
        result += std::to_wstring(foundArity);
        result += L" found.";

        return result;
    }(expectedArity, foundArity)),
    expectedArity(expectedArity),
    foundArity(foundArity)
    {
    }
};

struct SymbolNotFoundException : public Exception
{
    SymbolNotFoundException() :
        Exception(SYMBOL_NOT_FOUND, L"No symbol found.")
    {
    }
};

struct WrongTokenException : public Exception
{
    const std::wstring expectedToken;
    const std::wstring foundToken;

    WrongTokenException(const std::wstring &expectedToken, const std::wstring &foundToken) :
        Exception(WRONG_TOKEN, [](const std::wstring &expectedToken, const std::wstring &foundToken) -> std::wstring
    {
        std::wstring result = L"Wrong token. \'";

        result += expectedToken;
        result += L"\' expected but \'";
        result += foundToken;
        result += L"\' found.";

        return result;
    }(expectedToken, foundToken)),
        expectedToken(expectedToken),
        foundToken(foundToken)
    {
    }
};

struct TermExpectedException : public Exception
{
    TermExpectedException() :
        Exception(TERM_EXPECTED, L"Term is expected.")
    {
    }
};

struct EqualityExpectedException : public Exception
{
    EqualityExpectedException() :
        Exception(EQUALITY_EXPECTED, L"Equality is expected.")
    {
    }
};

struct NonexpectedNonequalityException : public Exception
{
    NonexpectedNonequalityException() :
        Exception(NONEQUALITY_UNEXPECTED, L"Nonequality is unexpected")
    {
    }
};

struct ExpectedEqualityOrNonequalityException : public Exception
{
    ExpectedEqualityOrNonequalityException() :
        Exception(EQUALITY_OR_NONEQUALITY_EXPECTED, L"Equality or nonequality is expected")
    {
    }
};

struct FormulaExpectedException : public Exception
{
    FormulaExpectedException() :
        Exception(FORMULA_EXPECTED, L"Formula is expected")
    {
    }
};

#endif // ERROR_H
