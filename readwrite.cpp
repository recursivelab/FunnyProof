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

#include <cwctype>
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

std::wstring Writer::operator ()(const Formula &formula, const Dictionary &dictionary, SymbolType *symbolType) const
{
    SymbolType localSymbolType;
    SymbolType &type = symbolType ? *symbolType : localSymbolType;
    std::wstring result;

    type = formula.type();

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
        result = operator ()(formula.formulas()[0], dictionary, &type);
        insertBracketsIfNeeded(formula.type(), type, result);
        type = NEGATION;
        result = negationSymbol+result;

        break;

    case CONJUNCTION: case DISJUNCTION:
        switch (formula.formulas().size()) {
        case 0:
            if (type == CONJUNCTION) {
                result = trueSymbol;
                type = TRUE_SYMBOL;
            } else {
                result = falseSymbol;
                type = FALSE_SYMBOL;
            }

            break;

        case 1:
            result = operator ()(formula.formulas()[0], dictionary);

            break;
        default:
            result = operator ()(formula.formulas()[0], dictionary, &type);
            insertBracketsIfNeeded(formula.type(), type, result);

            for (size_t i = 1; i < formula.formulas().size(); ++i) {
                result += formula.type() == CONJUNCTION ? conjunctionSymbol : disjunctionSymbol;

                std::wstring tmp = operator ()(formula.formulas()[i], dictionary, &type);

                insertBracketsIfNeeded(formula.type(), type, tmp);
                result += tmp;
            }

            break;
        }

        type = formula.type();

        break;

    case IMPLICATION: case EQUIVALENCE:
        if (formula.formulas().size() <= 1) {
            result = trueSymbol;
        } else {
            result = operator ()(formula.formulas()[0], dictionary, &type);

            insertBracketsIfNeeded(formula.type(), type, result);

            for (size_t i = 1; i < formula.formulas().size(); ++i) {
                result += formula.type() == IMPLICATION ? implicationSymbol : equivalenceSymbol;

                std::wstring tmp = operator ()(formula.formulas()[i], dictionary, &type);

                insertBracketsIfNeeded(formula.type(), type, tmp);
                result += tmp;
            }

            type = formula.type();
        }

        break;

    case UNIVERSAL: case EXISTENTIAL:
        if (formula.variables().empty()) {
            result = operator ()(formula.formulas()[0], dictionary, &type);
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

Reader::Reader()
{
}

Reader::~Reader()
{
}

bool Reader::getToken(const std::wstring &str, size_t &pos, std::wstring &token) const
{

    token.clear();

    while (pos < str.size() && iswspace(str[pos])) {
        ++pos;
    }

    if (pos >= str.size()) {
        return false;
    }

    wchar_t ch = str[pos];

    switch (ch) {
    case L'(':
    case L')':
    case L',':
    case L'↑':
    case L'¬':
    case L'∧':
    case L'∨':
    case L'⇒':
    case L'⇔':
    case L'⊤':
    case L'⊥':
    case L'∀':
    case L'∃':
    case L'=':
        token.push_back(ch);
        ++pos;

        return true;

        break;
    default:
        break;
    }

    if (iswalpha(ch) == false) {
        return false;
    }

    token += ch;
    ++pos;

    if (pos < str.size() && str[pos] == L'_') {
        switch (str[pos-1]) {
        case L'v':
        case L'c':
        case L'f':
        case L'R':

            break;

        default:
            return false;

            break;
        }

        token += str[pos++];

        if (pos >= str.size() || iswalnum(str[pos]) == false) {
            return false;
        }

        token += str[pos++];

        while (pos < str.size() && iswalnum(str[pos])) {
            token += str[pos++];
        }

        return true;
    }

    while (pos < str.size() && isalnum(str[pos])) {
        token += str[pos++];
    }

    return true;
}

std::vector<Term> Reader::getTermList(const std::wstring &str, Dictionary &dictionary, size_t &pos, bool &ok) const
{
    std::wstring token;
    std::vector<Term> result;

    if (getToken(str, pos, token) == false || token != L"(") {
        ok = false;

        return result;
    }

    size_t currentPos = pos;

    ok = getToken(str, pos, token);

    if (ok == false) {
        return std::vector<Term>();
    }

    if (token == L")") {
        return result;
    }

    pos = currentPos;

    while (true) {
        Term t = getTerm(str, dictionary, pos, ok);

        if (ok == false) {
            return std::vector<Term>();
        }

        result.push_back(t);
        ok = getToken(str, pos, token);

        if (ok == false) {
            return std::vector<Term>();
        }

        if (token == L")") {
            return result;
        }

        if (token != L",") {
            ok = false;

            return std::vector<Term>();
        }
    }
}

Term Reader::getTerm(const std::wstring &str, Dictionary &dictionary, size_t &pos, bool &ok) const
{
    std::vector<Term> terms;
    std::wstring token;

    if (getToken(str, pos, token) == false) {
        ok = false;

        return Term();
    }

    if (token.size() > 2 && token[1] == L'_') {
        wchar_t c = token[0];
        std::wstring name = token.substr(2);

        Symbol s = dictionary(name);

        if (s.type == NONE_SYMBOL) {
            if (iswalpha(name[0]) == false) {
                ok = false;

                return Term();
            }

            switch (c) {
            case 'v':
            {
                Variable v;

                ok = dictionary.insert(name, v);

                if (ok) {
                    return Term(v);
                }

                return Term();
            }

                break;

            case 'c':
            {
                ConstantSymbol c;

                ok = dictionary.insert(name, c);

                if (ok) {
                    return Term(c);
                }

                return Term();
            }

                break;

            case 'f':
            {
                terms = getTermList(str, dictionary, pos, ok);

                if (ok == false) {
                    return Term();
                }


                Symbol s = dictionary(name);

                switch (s.type) {
                case NONE_SYMBOL:
                {
                    OperationSymbol f(terms.size());

                    dictionary.insert(name, f);

                    return Term(f, std::move(terms));
                }

                    break;

                case OPERATION:
                    if (s.arity != terms.size()) {
                        ok = false;

                        return Term();
                    }

                    return Term(OperationSymbol(s), std::move(terms));

                    break;

                default:
                    ok = false;

                    return Term();

                    break;
                }
            }

                break;

            default:
                throw(1);

                break;
            }
        } else {
            switch (c) {
            case L'v':
                if (s.type != VARIABLE) {
                    ok = false;

                    return Term();
                }

                return TermEnvironment::Term(Variable(s));

                break;

            case L'c':
                if (s.type != CONSTANT) {
                    ok = false;

                    return Term();
                }

                return TermEnvironment::Term(ConstantSymbol(s));

                break;

            case L'f':
                if (s.type != OPERATION) {
                    ok = false;

                    return Term();
                }

                terms = getTermList(str, dictionary, pos, ok);

                if (ok == false || terms.size() != s.arity) {
                    ok = false;

                    return Term();
                }

                return TermEnvironment::Term(OperationSymbol(s), std::move(terms));

                break;

            default:
                throw (1);

                break;
            }
        }
    } else {
        Symbol s = dictionary(token);

        switch (s.type) {
        case VARIABLE:
            return Term(Variable(s));

            break;

        case CONSTANT:
            return Term(ConstantSymbol(s));

            break;

        case OPERATION:
            terms = getTermList(str, dictionary, pos, ok);

            if (ok == false || terms.size() != s.arity) {
                ok = false;

                return Term();
            }

            return Term(OperationSymbol(s), std::move(terms));

            break;

        default:
            break;
        }
    }

    ok = false;

    return Term();
}

Term Reader::getTerm(const std::wstring &str, Dictionary &dictionary, bool *ok) const
{
    static bool localBool;
    size_t pos = 0;

    if (ok == nullptr) {
        ok = &localBool;
    }

    Term result = getTerm(str, dictionary, pos, *ok);

    return result;
}
