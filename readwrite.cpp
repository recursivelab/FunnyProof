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
#include "error.h"
#include "readwrite.h"
#include "utility.h"

class DictionaryExtender {
    Dictionary &dictionary;
    bool merged;

public:
    DictionaryExtender(Dictionary &dictionary) :
        dictionary(dictionary),
        merged(false)
    {
        dictionary.push();
    }

    ~DictionaryExtender()
    {
        if (merged==false) {
            dictionary.pop();
        }
    }

    void merge()
    {
        if (merged==false) {
            merged = true;
            dictionary.mergeTop2Environments();
        }
    }
};

Symbolic::Symbolic() :
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

Writer::Writer()
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
        str = symbolic.leftFormulaBracket+str+symbolic.rightFormulaBracket;
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
            return symbolic.noneSymbol;

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

    if (term.type()!=OPERATION) {
        return result;
    }

    result += symbolic.leftTermBracket;

    if (term.arity()) {
        result += operator ()(term.args()[0], dictionary);

        for (size_t i = 1; i < term.arity(); ++i) {
            result += symbolic.operationSeparatorSymbol;
            result += operator ()(term.args()[i], dictionary);
        }
    }

    result += symbolic.rightTermBracket;

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
        return symbolic.noneSymbol;

        break;

    case FALSE_SYMBOL:
        return symbolic.falseSymbol;

        break;

    case TRUE_SYMBOL:
        return symbolic.trueSymbol;

        break;

    case EQUALITY:
        if (formula.terms().size()<=1) {
            return symbolic.trueSymbol;
        }

        result += operator ()(formula.terms()[0], dictionary);

        for (size_t i = 1; i < formula.terms().size(); ++i) {
            result += symbolic.equalitySymbol;
            result += operator ()(formula.terms()[i], dictionary);
        }

        break;

    case RELATION:
        result = dictionary(formula.symbol());

        if (result.empty()) {
            result = relationName(formula.symbol().id);
        }

        result += symbolic.leftRelationBracket;

        if (formula.terms().empty() == false) {
            result += operator ()(formula.terms()[0], dictionary);

            for (size_t i = 1; i < formula.terms().size(); ++i) {
                result += symbolic.relationSeparatorSymbol;
                result += operator ()(formula.terms()[i], dictionary);
            }
        }

        result += symbolic.rightRelationBracket;

        break;

    case NEGATION:
        result = operator ()(formula.formulas()[0], dictionary, &type);
        insertBracketsIfNeeded(formula.type(), type, result);
        type = NEGATION;
        result = symbolic.negationSymbol+result;

        break;

    case CONJUNCTION: case DISJUNCTION:
        switch (formula.formulas().size()) {
        case 0:
            if (type == CONJUNCTION) {
                result = symbolic.trueSymbol;
                type = TRUE_SYMBOL;
            } else {
                result = symbolic.falseSymbol;
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
                result += formula.type() == CONJUNCTION ? symbolic.conjunctionSymbol : symbolic.disjunctionSymbol;

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
            result = symbolic.trueSymbol;
        } else {
            result = operator ()(formula.formulas()[0], dictionary, &type);

            insertBracketsIfNeeded(formula.type(), type, result);

            for (size_t i = 1; i < formula.formulas().size(); ++i) {
                result += formula.type() == IMPLICATION ? symbolic.implicationSymbol : symbolic.equivalenceSymbol;

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
            result = symbolic.leftQuantifierBracket;
            result += type == UNIVERSAL ? symbolic.universalQuantifier : symbolic.existentialQuantifier;
            result += operator ()(formula.variables()[0], dictionary);

            for (size_t i = 1; i < formula.variables().size(); ++i) {
                result += symbolic.variableSeparatorSymbol;
                result += operator ()(formula.variables()[i], dictionary);
            }

            result += symbolic.rightQuantifierBracket;

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

bool Reader::isName(const std::wstring &str) const
{
    if (str.empty()) {
        return false;
    }

    if (iswalpha(str[0])==false) {
        return false;
    }

    for (size_t i = 1; i<str.size(); ++i) {
        if (iswalnum(str[i])==false) {
            return false;
        }
    }

    return true;
}

void Reader::getToken(const std::wstring &str, size_t &pos, std::wstring &token) const
{
    token.clear();

    while (pos<str.size() && iswspace(str[pos])) {
        ++pos;
    }

    if (pos>=str.size()) {
        throw(UnexpectedEndException());
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

        return;

        break;
    }

    if (iswalpha(ch)==false) {
        throw(AlphaExpectedException());
    }

    token += ch;
    ++pos;

    if (pos<str.size() && str[pos]==L'_') {
        switch (str[pos-1]) {
        case L'v':
        case L'c':
        case L'f':
        case L'R':

            break;

        default:
            throw(OneOfExpectedException(L"vcfR"));

            break;
        }

        token += str[pos++];

        if (pos>=str.size() || iswalpha(str[pos]) == false) {
            throw(AlphaExpectedException());
        }

        token += str[pos++];

        while (pos<str.size() && iswalnum(str[pos])) {
            token += str[pos++];
        }

        if (token.size()==2) {
            token.clear();

            throw(UnexpectedEndException());
        }
    }

    while (pos<str.size() && iswalnum(str[pos])) {
        token += str[pos++];
    }
}

Symbol Reader::getSymbol(const std::wstring &str, size_t &pos, Dictionary &dictionary, std::wstring &name, size_t *arity) const
{
    std::wstring token;

    name.clear();
    getToken(str, pos, token);

    if (token.size()>2 && token[1]==L'_') {
        name = token.substr(2);

        if (isName(name)==false) {
            throw(NameExpectedException());
        }

        switch (token[0]) {
        case L'c':
        {
            Symbol s = dictionary(name);

            if (s.type==CONSTANT) {
                return s;
            }

            if (s.type!=NONE_SYMBOL) {
                name.clear();

                throw(ConstantExpectedException());
            }

            ConstantSymbol c;
            DictionaryExtender extender(dictionary);

            if (dictionary.insert(name, c)) {
                extender.merge();

                return c;
            }

            name.clear();

            throw(ConstantExpectedException());
        }
            break;

        case L'v':
        {
            Symbol s = dictionary(name);

            if (s.type==VARIABLE) {
                return s;
            }

            if (s.type!=NONE_SYMBOL) {
                name.clear();

                throw(VariableExpectedException());
            }

            Variable v;
            DictionaryExtender extender(dictionary);

            if (dictionary.insert(name, v)) {
                extender.merge();

                return v;
            }

            name.clear();

            throw(VariableExpectedException());
        }
            break;

        case L'f':
        {
            Symbol s = dictionary(name);

            if (s.type==OPERATION) {
                if (arity!=nullptr && s.arity!=*arity) {
                    name.clear();

                    throw(WrongArityException(*arity, s.arity));
                }

                return s;
            }

            if (s.type!=NONE_SYMBOL || arity==nullptr) {
                name.clear();

                throw(OperationExpectedException());
            }

            OperationSymbol f(*arity);
            DictionaryExtender extender(dictionary);

            if (dictionary.insert(name, f)) {
                extender.merge();

                return f;
            }

            name.clear();

            throw(OperationExpectedException());
        }

            break;

        case L'R':
        {
            Symbol s = dictionary(name);

            if (s.type==RELATION) {
                if (arity!=nullptr && s.arity!=*arity) {
                    name.clear();

                    throw(WrongArityException(*arity, s.arity));
                }

                return s;
            }

            if (s.type!=NONE_SYMBOL || arity==nullptr) {
                name.clear();

                throw(RelationExpectedException());
            }

            RelationSymbol f(*arity);
            DictionaryExtender extender(dictionary);

            if (dictionary.insert(name, f)) {
                extender.merge();

                return f;
            }

            name.clear();

            throw(RelationExpectedException());
        }

            break;
        }

        throw(OneOfExpectedException(L"cvfR"));
    }

    Symbol s = dictionary(token);

    if (s.type==NONE_SYMBOL) {
        throw(SymbolNotFoundException());
    }

    if (arity!=nullptr && (s.type==OPERATION || s.type==RELATION) && (s.arity!=*arity)) {
        throw(WrongArityException(*arity, s.arity));
    }

    name = token;

    return s;
}

void Reader::expectToken(const std::wstring &str, size_t &pos, const std::wstring &token) const
{
    std::wstring readedToken;

    getToken(str, pos, readedToken);

    if (readedToken!=token) {
        throw(WrongTokenException(token, readedToken));
    }
}

std::vector<Term> Reader::parseTermList(const std::wstring &str, Dictionary &dictionary, size_t &pos) const
{
    std::wstring token;
    std::vector<Term> result;

    expectToken(str, pos, L"(");

    try {
        expectToken(str, pos, L")");
    } catch(WrongTokenException &e) {
        DictionaryExtender extender(dictionary);

        try {
            while (true) {
                Term t = parseTerm(str, dictionary, pos);

                result.push_back(t);
                getToken(str, pos, token);

                if (token==L")") {
                    extender.merge();

                    return result;
                }

                expectToken(str, pos, L",");
            }
        } catch(Exception &e) {
            throw(e);
        }
    }

    return result;
}

Term Reader::parseTerm(const std::wstring &str, Dictionary &dictionary, size_t &pos) const
{
    std::wstring token;
    DictionaryExtender extender(dictionary);

    try {
        Symbol s = getSymbol(str, pos, dictionary, token);

        switch (s.type) {
        case CONSTANT:
            extender.merge();

            return TermEnvironment::Term(ConstantSymbol(s));

            break;

        case VARIABLE:
            extender.merge();

            return TermEnvironment::Term(Variable(s));

            break;

        case OPERATION:
        {
            std::vector<Term> terms = parseTermList(str, dictionary, pos);

            if (terms.size()==s.arity) {
                extender.merge();

                return TermEnvironment::Term(OperationSymbol(s), std::move(terms));
            }
        }

            break;

        default:

            break;
        }
    } catch(const SymbolNotFoundException&) {
    }

    throw(TermExpectedException());
}

Term Reader::parseTerm(const std::wstring &str, Dictionary &dictionary) const
{
    size_t pos = 0;

    return parseTerm(str, dictionary, pos);
}

Formula Reader::parseFirstSubformula(const std::wstring &str, Dictionary &dictionary, size_t &pos) const
{
    size_t p = pos;
    std::vector<Term> terms;
    DictionaryExtender extender(dictionary);

    try {
        Term t = parseTerm(str, dictionary, pos);

        terms.push_back(t);

        std::wstring token;

        while (true) {
            bool end = false;

            p = pos;

            try {
                getToken(str, pos, token);
            } catch(const UnexpectedEndException&) {
                end = true;
            }

            if (end) {
                break;
            }

            toLower(token);

            if (token==L"=") {
                Term t2 = parseTerm(str, dictionary, pos);

                terms.push_back(t2);
            } else if (token==L"neq") {
                if (terms.size()==1) {
                    Term t2 = parseTerm(str, dictionary, pos);

                    terms.push_back(t2);
                    extender.merge();

                    return FormulaEnvironment::NonequalityFormula(std::move(terms));
                } else {
                    throw(NonexpectedNonequalityException());
                }
            } else {
                pos = p;

                break;
            }
        }

        if (terms.size()==1) {
            throw(ExpectedEqualityOrNonequalityException());
        }

        extender.merge();

        return FormulaEnvironment::EqualityFormula(std::move(terms));
    } catch(const TermExpectedException&) {
        try {
            std::wstring token;

            pos = p;
            getToken(str, pos, token);

            std::wstring lowerToken = token;

            toLower(lowerToken);

            if (lowerToken==L"false") {
                extender.merge();

                return FormulaEnvironment::FalseFormula();
            }

            if (lowerToken==L"true") {
                extender.merge();

                return FormulaEnvironment::TrueFormula();
            }

            if (lowerToken==L"not") {
                p = pos;

                Formula f = parseFirstSubformula(str, dictionary, pos);

                extender.merge();

                return FormulaEnvironment::NegationFormula(f);
            }

            if (lowerToken==L"(") {
                p = pos;
                getToken(str, pos, token);
                toLower(token);

                bool forall = token==L"forall";
                bool exists = forall==false && token==L"exists";

                if (forall || exists) {
                    std::vector<Variable> variables;

                    while (true) {
                        getToken(str, pos, token);

                        if (token==L",") {
                            getToken(str, pos, token);
                        }

                        if (token==L")") {
                            break;
                        }

                        if (isName(token)==false) {
                            throw(VariableExpectedException());
                        }

                        Symbol s = dictionary(token);

                        if (s.type==NONE_SYMBOL) {
                            Variable x;

                            dictionary.insert(token, x);
                            variables.push_back(x);
                        } else if (s.type==VARIABLE) {
                            Variable x(s);

                            variables.push_back(x);
                        } else {
                            throw(VariableExpectedException());
                        }
                    }

                    if (variables.empty()) {
                        throw(VariableExpectedException());
                    }

                    Formula f = parseFirstSubformula(str, dictionary, pos);

                    extender.merge();

                    if (forall) {
                        return FormulaEnvironment::UniversalFormula(f, std::move(variables));
                    }

                    return FormulaEnvironment::ExistentialFormula(f, std::move(variables));
                }

                Formula f = parseImpEqu(str, dictionary, pos);

                expectToken(str, pos, L")");
                extender.merge();

                return f;
            }

            Symbol s = dictionary(token);

            if (s.type==NONE_SYMBOL) {
                std::vector<Term> terms = parseTermList(str, dictionary, pos);
                RelationSymbol r(terms.size());

                extender.merge();

                return FormulaEnvironment::RelationFormula(r, std::move(terms));
            }

            if (s.type==RELATION) {
                std::vector<Term> terms = parseTermList(str, dictionary, pos);

                if (terms.size()!=s.arity) {
                    throw(WrongArityException(s.arity, terms.size()));
                }

                RelationSymbol r(s);

                extender.merge();

                return FormulaEnvironment::RelationFormula(r, std::move(terms));
            }

            throw(FormulaExpectedException());
        } catch(const Exception &e) {
            throw(e);
        }
    } catch (const Exception &e) {
        throw(e);
    }
}

Formula Reader::parseConDis(const std::wstring &str, Dictionary &dictionary, size_t &pos) const
{
    std::vector<Formula> formulas;
    bool conjunction;
    DictionaryExtender extender(dictionary);

    while (true) {
        bool conj;
        bool disj;

        try {
            Formula f = parseFirstSubformula(str, dictionary, pos);

            formulas.push_back(f);

            std::wstring token;
            size_t p = pos;
            bool end = false;

            try {
                getToken(str, pos, token);
            } catch (const UnexpectedEndException&) {
                end = true;
            }

            if (end) {
                break;
            }

            toLower(token);
            conj = token==L"and";
            disj = conj==false && token==L"or";

            if (conj==false && disj==false) {
                pos = p;

                break;
            }
        } catch(const Exception &e) {
            throw(e);
        }

        if (formulas.size()==1) {
            conjunction = conj;
        } else {
            if (conjunction!=conj) {

                return Formula();
            }
        }
    }

    if (formulas.empty()) {
        throw(FormulaExpectedException());
    }

    extender.merge();

    if (formulas.size()==1) {
        return formulas[0];
    }

    if (conjunction) {
        return FormulaEnvironment::ConjunctionFormula(std::move(formulas));
    }

    return FormulaEnvironment::DisjunctionFormula(std::move(formulas));
}

Formula Reader::parseImpEqu(const std::wstring &str, Dictionary &dictionary, size_t &pos) const
{
    std::vector<bool> connectives;
    std::vector<Formula> formulas;
    DictionaryExtender extender(dictionary);

    while (true) {
        bool impl;
        bool equiv;

        try {
            Formula f = parseConDis(str, dictionary, pos);

            formulas.push_back(f);

            std::wstring token;
            size_t p = pos;
            bool end = false;

            try {
                getToken(str, pos, token);
            } catch(UnexpectedEndException&) {
                end = true;
            }

            if (end) {
                break;
            }

            toLower(token);
            impl = token==L"imp";
            equiv = impl==false && token==L"equ";

            if (impl==false && equiv==false) {
                pos = p;

                break;
            }

            connectives.push_back(impl);
        } catch(const Exception &e) {
            throw(e);
        }
    }

    if (formulas.empty()) {
        throw(FormulaExpectedException());
    }

    extender.merge();

    if (formulas.size()==1) {
        return formulas[0];
    }

    std::vector<Formula> result;
    size_t i = 0;
    size_t j = 1;

    while (j<connectives.size()) {
        while (j<connectives.size() && connectives[i]==connectives[j]) {
            ++j;
        }

        std::vector<Formula> tmp;

        for (size_t k = i; k<=j; ++k) {
            tmp.push_back(formulas[k]);
        }

        if (connectives[i]) {
            result.push_back(FormulaEnvironment::ImplicationFormula(std::move(tmp)));
        } else {
            result.push_back(FormulaEnvironment::EquivalenceFormula(std::move(tmp)));
        }

        i = j;
        ++j;
    }

    if (result.size()==1) {
        return result[0];
    }

    return FormulaEnvironment::ConjunctionFormula(std::move(result));
}

Formula Reader::parseFormula(const std::wstring &str, Dictionary &dictionary, size_t &pos) const
{
    return parseImpEqu(str, dictionary, pos);
}

Formula Reader::parseFormula(const std::wstring &str, Dictionary &dictionary) const
{
    size_t pos = 0;

    Formula result = parseFormula(str, dictionary, pos);

    if (pos<str.size()) {
        throw(FormulaExpectedException());
    }

    return result;
}
