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

/*!
    \file "readwrite.h"

    \author Nedeljko Stefanovic

    \brief Implementation of classes for parsing and converting to string terms and formulas.

    These logical objects are implemented as immutable objects.
    Copy construction is enabled, but assignment is disabled.

    All these classes have comparision operators "=", "!=" amd "<",
    comparision function "cmp" and hash function.

    All program objects representing terms and formulas shares internal
    representation of this type of logical object. A object constructed as copy
    of another object uses the same shared internal object.
    Difference of addreses of internal objects does not imply difference of
    represented logical objects.

    Any symbol object have it's own id. Symbol object constructed without copy
    constructor representing a new symbol with new id.
*/

#ifndef READWRITE_H
#define READWRITE_H

#include "dictionary.h"

class Symbolic
{
public:
    std::wstring noneSymbol;
    std::wstring falseSymbol;
    std::wstring trueSymbol;
    std::wstring equalitySymbol;
    std::wstring negationSymbol;
    std::wstring conjunctionSymbol;
    std::wstring disjunctionSymbol;
    std::wstring implicationSymbol;
    std::wstring equivalenceSymbol;
    std::wstring universalQuantifier;
    std::wstring existentialQuantifier;
    std::wstring leftTermBracket;
    std::wstring rightTermBracket;
    std::wstring leftRelationBracket;
    std::wstring rightRelationBracket;
    std::wstring leftFormulaBracket;
    std::wstring rightFormulaBracket;
    std::wstring leftQuantifierBracket;
    std::wstring rightQuantifierBracket;
    std::wstring operationSeparatorSymbol;
    std::wstring relationSeparatorSymbol;
    std::wstring variableSeparatorSymbol;

    Symbolic();
};

class Writer
{
public:
    Symbolic symbolic;

    Writer();
    virtual ~Writer();
    virtual std::wstring variableName(uint64_t id) const;
    virtual std::wstring constantName(uint64_t id) const;
    virtual std::wstring operationName(uint64_t id) const;
    virtual std::wstring relationName(uint64_t id) const;
    virtual size_t arity(SymbolType type) const;
    virtual int precendence(SymbolType type) const;
    virtual bool bracketsNeeded(SymbolType type, SymbolType nestedType) const;
    virtual void insertBracketsIfNeeded(SymbolType type, SymbolType nestedType, std::wstring &str) const;
    virtual std::wstring operator ()(const Term &term, const Dictionary &dictionary, SymbolType *symbolType = nullptr) const;
    virtual std::wstring operator ()(const Formula &formula, const Dictionary &dictionary, SymbolType *symbolType = nullptr) const;
};

// Class for parsing terms and formulas.
class Reader
{
public:
    Symbolic symbolic;

    Reader();
    virtual ~Reader();
    virtual bool isName(const std::wstring &str) const;
    // Parses token from string str from given position pos and stores result into token.
    // If succeds, returns true and pos is after readed token.
    // If fails, returns false and pos is at error position.
    virtual void getToken(const std::wstring &str, size_t &pos, std::wstring &token) const;
    Symbol getSymbol(const std::wstring &str, size_t &pos, Dictionary &dictionary, std::wstring &name, size_t *arity = nullptr) const;
    // Parses single textual token from string str from position pos and expects given token given as token.
    // Parses single textual token from string str from position pos and expects given token given as token.
    // If given token is found, pos is updated to position after readed token and true is returned.
    // Otherwise, pos is untouched and false is returned.
    void expectToken(const std::wstring &str, size_t &pos, const std::wstring &token) const;
    // Parses from string str and position pos a list of terms bounded by parentheses '(' and ')' and separated by comma.
    // If succeds, returns vector of readed terms, sets ok to true, and updates pos after ter.
    // If fails, returns empty vector of terms and ok sets to false.
    virtual std::vector<Term> parseTermList(const std::wstring &str, Dictionary &dictionary, size_t &pos) const;
    virtual Term parseTerm(const std::wstring &str, Dictionary &dictionary, size_t &pos) const;
    virtual Term parseTerm(const std::wstring &str, Dictionary &dictionary) const;
    virtual Formula parseFirstSubformula(const std::wstring &str, Dictionary &dictionary, size_t &pos) const;
    virtual Formula parseConDis(const std::wstring &str, Dictionary &dictionary, size_t &pos) const;
    virtual Formula parseImpEqu(const std::wstring &str, Dictionary &dictionary, size_t &pos) const;
    virtual Formula parseFormula(const std::wstring &str, Dictionary &dictionary, size_t &pos) const;
    virtual Formula parseFormula(const std::wstring &str, Dictionary &dictionary) const;
};

#endif // READWRITE_H
