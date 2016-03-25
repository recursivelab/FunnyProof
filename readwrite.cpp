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

#include "readwrite.h"


Writer::Writer() :
    noneSymbol(L"↑"),
    falseSymbol(L"⊥"),
    trueSymbol(L"⊤"),
    equalitySymbol(L"="),
    negationSymbol(L"¬"),
    conjunctionSymbol(L"∧"),
    disjunctionSymbol(L"∨"),
    implicationSymbol(L"⇒"),
    equivalenceSymbol(L"⇔"),
    universalQuantifier(L"∀"),
    existentialQuantifier(L"∃"),
    leftTermBracket(L"("),
    rightTermBracket(L")"),
    leftRelationBracket(L"("),
    rightRelationBracket(L")"),
    leftFormulaBracket(L"("),
    rightFormulaBracket(L")"),
    leftQuantifierBracket(L"("),
    rightQuantifierBracket(L")"),
    operationSeparatorSymbol(L","),
    relationSeparatorSymbol(L","),
    variableSeparatorSymbol(L",")
{
}

Writer::~Writer()
{
}

std::wstring Writer::variableName(uint64_t id) const
{
    return std::wstring(L"v_")+std::to_wstring(id);
}

std::wstring Writer::constantName(uint64_t id) const
{
    return std::wstring(L"c_")+std::to_wstring(id);
}

std::wstring Writer::operationName(uint64_t id) const
{
    return std::wstring(L"O_")+std::to_wstring(id);
}

std::wstring Writer::relationName(uint64_t id) const
{
    return std::wstring(L"R_")+std::to_wstring(id);
}

size_t Writer::arity(SymbolType type) const
{
    switch (type) {
    case NONE_SYMBOL:
    case FALSE_SYMBOL:
    case TRUE_SYMBOL:
    case EQUALITY:
    case RELATION:
        return 0;

        break;

    case NEGATION:
    case UNIVERSAL:
    case EXISTENTIAL:
        return 1;

        break;

    case CONJUNCTION:
    case DISJUNCTION:
    case IMPLICATION:
    case EQUIVALENCE:
        return 2;

        break;

    default:
        throw(1);

        break;
    }
}

int Writer::precendence(SymbolType type) const
{
    switch (type) {
    case NONE_SYMBOL:
    case FALSE_SYMBOL:
    case TRUE_SYMBOL:
    case EQUALITY:
    case RELATION:
    case NEGATION:
    case UNIVERSAL:
    case EXISTENTIAL:
        return 0;

        break;

    case CONJUNCTION:
    case DISJUNCTION:
        return 1;

        break;

    case IMPLICATION:
    case EQUIVALENCE:
        return 2;

        break;

    default:
        throw(1);

        break;
    }
}

bool Writer::bracketsNeeded(SymbolType type, SymbolType nestedType) const
{
    switch (arity(type)) {
    case 0:
        return false;

        break;

    case 1:
        return arity(nestedType) == 2;

        break;

    case 2:
        if (arity(nestedType) < 2) {
            return false;
        }

        return precendence(type) <= precendence(nestedType);

        break;

    default:
        throw(1);

        break;
    }
}

void Writer::insertBracketsIfNeeded(SymbolType type, SymbolType nestedType, std::wstring &str) const
{
    if (bracketsNeeded(type, nestedType)) {
        str = leftFormulaBracket+str+rightFormulaBracket;
    }
}

std::wstring Writer::operator ()(const Term &term, const Dictionary &dictionary, SymbolType *symbolType) const
{
    if (symbolType) {
        *symbolType = term.type();
    }

    std::wstring result = dictionary(term.symbol());

    if (result.empty()) {
        switch (term.type()) {
        case NONE_SYMBOL:
            return noneSymbol;

            break;

        case VARIABLE:
            return variableName(term.symbol().id);

            break;
        case CONSTANT:
            return constantName(term.symbol().id);

            break;
        case OPERATION:
            result = operationName(term.symbol().id);

            break;

        default:
            throw(1);

            break;
        }
    }

    if (term.type() != OPERATION) {
        return result;
    }

    result += leftTermBracket;

    if (term.arity()) {
        result += operator ()(term.args()[0], dictionary);

        for (size_t i = 1; i < term.arity(); ++i) {
            result += operationSeparatorSymbol;
            result += operator ()(term.args()[i], dictionary);
        }
    }

    result += rightTermBracket;

    return result;
}

std::wstring Writer::operator ()(const Formula &formula, const Dictionary &dictionary, SymbolType *symbolType, size_t *counter) const
{
    size_t localCounter;
    SymbolType localSymbolType;
    SymbolType &type = symbolType ? *symbolType : localSymbolType;
    size_t &cnt = counter ? *counter : localCounter;

    type = formula.type();
    cnt = formula.terms().size() + formula.formulas().size();

    std::wstring result;

    switch (type) {
    case NONE_SYMBOL:
        return noneSymbol;

        break;

    case FALSE_SYMBOL:
        return falseSymbol;

        break;

    case TRUE_SYMBOL:
        return trueSymbol;

        break;

    case EQUALITY:
        if (formula.terms().size() <= 1) {
            return trueSymbol;
        }

        result += operator ()(formula.terms()[0], dictionary);

        for (size_t i = 1; i < formula.terms().size(); ++i) {
            result += equalitySymbol;
            result += operator ()(formula.terms()[i], dictionary);
        }

        break;

    case RELATION:
        result = dictionary(formula.symbol());

        if (result.empty()) {
            result = relationName(formula.symbol().id);
        }

        result += leftRelationBracket;

        if (formula.terms().empty() == false) {
            result += operator ()(formula.terms()[0], dictionary);

            for (size_t i = 1; i < formula.terms().size(); ++i) {
                result += relationSeparatorSymbol;
                result += operator ()(formula.terms()[i], dictionary);
            }
        }

        result += rightRelationBracket;

        break;

    case NEGATION:
        result = operator ()(formula.formulas()[0], dictionary, &type, &cnt);
        insertBracketsIfNeeded(formula.type(), type, result);
        type = NEGATION;
        cnt = 1;
        result = negationSymbol+result;

        break;

    case CONJUNCTION: case DISJUNCTION:
        if (cnt == 0) {
            result = type == CONJUNCTION ? trueSymbol : falseSymbol;
        } else if (cnt == 1) {
            result = operator ()(formula.formulas()[0], dictionary);
        } else {
            result = operator ()(formula.formulas()[0], dictionary, &type, &cnt);
            insertBracketsIfNeeded(formula.type(), type, result);

            for (size_t i = 1; i < formula.formulas().size(); ++i) {
                result += formula.type() == CONJUNCTION ? conjunctionSymbol : disjunctionSymbol;

                std::wstring tmp = operator ()(formula.formulas()[i], dictionary, &type, &cnt);

                insertBracketsIfNeeded(formula.type(), type, tmp);
                result += tmp;
            }

            type = formula.type();
            cnt = formula.formulas().size();
        }

        break;

    case IMPLICATION: case EQUIVALENCE:
        if (cnt <= 1) {
            result = trueSymbol;
        } else {
            result = operator ()(formula.formulas()[0], dictionary, &type, &cnt);

            insertBracketsIfNeeded(formula.type(), type, result);

            for (size_t i = 1; i < formula.formulas().size(); ++i) {
                result += formula.type() == IMPLICATION ? implicationSymbol : equivalenceSymbol;

                std::wstring tmp = operator ()(formula.formulas()[i], dictionary, &type, &cnt);

                insertBracketsIfNeeded(formula.type(), type, tmp);
                result += tmp;
            }

            type = formula.type();
            cnt = formula.formulas().size();
        }

        break;

    case UNIVERSAL: case EXISTENTIAL:
        if (formula.variables().empty()) {
            result = operator ()(formula.formulas()[0], dictionary, &type, &cnt);
        } else {
            result = leftQuantifierBracket;
            result += type == UNIVERSAL ? universalQuantifier : existentialQuantifier;
            result += operator ()(formula.variables()[0], dictionary);

            for (size_t i = 1; i < formula.variables().size(); ++i) {
                result += variableSeparatorSymbol;
                result += operator ()(formula.variables()[i], dictionary);
            }

            result += rightQuantifierBracket;

            std::wstring tmp = operator ()(formula.formulas()[0], dictionary, &type);

            insertBracketsIfNeeded(formula.type(), type, tmp);
            result += tmp;
        }

        break;

    default:
        throw(1);

        break;
    }

    return result;
}
