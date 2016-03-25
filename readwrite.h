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

#ifndef READWRITE_H
#define READWRITE_H

#include "dictionary.h"

class Writer
{
public:
    Writer();
    virtual ~Writer();
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
    virtual std::wstring variableName(uint64_t id) const;
    virtual std::wstring constantName(uint64_t id) const;
    virtual std::wstring operationName(uint64_t id) const;
    virtual std::wstring relationName(uint64_t id) const;
    virtual size_t arity(SymbolType type) const;
    virtual int precendence(SymbolType type) const;
    virtual bool bracketsNeeded(SymbolType type, SymbolType nestedType) const;
    virtual void insertBracketsIfNeeded(SymbolType type, SymbolType nestedType, std::wstring &str) const;
    virtual std::wstring operator ()(const Term &term, const Dictionary &dictionary, SymbolType *symbolType = nullptr) const;
    virtual std::wstring operator ()(const Formula &formula, const Dictionary &dictionary, SymbolType *symbolType = nullptr, size_t *counter = nullptr) const;
};

#endif // READWRITE_H
