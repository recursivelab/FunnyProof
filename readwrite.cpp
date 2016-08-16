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

#define EXTEND_DICTIONARY DictionaryExtender extender(dictionary)
#define CASE_BEGIN \
{\
    size_t oldPos = pos;\
    \
    try {\
        DictionaryExtender dictionaryExtender(dictionary);

#define CASE_END \
    } catch(const Exception &e) {\
        if (messageText.empty() || pos>=messagePos) {\
            messageText = e.description;\
            messagePos = pos;\
        }\
        \
        pos = oldPos;\
    }\
}

Symbolic::Symbolic() :
    noneSymbol(L"↑"),
    falseSymbol(L"⊥"),
    trueSymbol(L"⊤"),
    equalitySymbol(L"="),
    nonequalitySymbol(L"≠"),
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
    case NONEQUALITY:
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

    case NONEQUALITY:
        if (formula.terms().size()<=1) {
            return symbolic.trueSymbol;
        }

        if (formula.terms().size()==2) {
            result += (*this)(formula.terms()[0], dictionary);
            result += symbolic.nonequalitySymbol;
            result += (*this)(formula.terms()[1], dictionary);
        } else {
            result += symbolic.nonequalitySymbol+symbolic.leftRelationBracket;

            for (size_t i = 0; i<formula.terms().size(); ++i) {
                if (i!=0) {
                    result += symbolic.relationSeparatorSymbol;
                    result += (*this)(formula.terms()[i], dictionary);
                }
            }

            result += symbolic.rightRelationBracket;
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
        type = formula.type();
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
        }

        type = formula.type();

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

        type = formula.type();

        break;

    default:
        throw(1);

        break;
    }

    return result;
}

void Reader::updateMessage(const Exception &e)
{
    if (messageText.empty() || pos>messagePos) {
        messagePos = pos;
        messageText = e.description;
    }
}

Reader::Reader(const std::wstring &str, Dictionary &dictionary) :
    pos(0),
    str(str),
    dictionary(dictionary)
{
}

Reader::Reader(std::wstring &&str, Dictionary &dictionary) :
    pos(0),
    str(str),
    dictionary(dictionary)
{
}

Reader::~Reader()
{
}

void Reader::getToken(std::wstring &token)
{
    CASE_BEGIN
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
    case L'.':
        token.push_back(ch);
        ++pos;

        return;

        break;
    }

    if (iswalpha(ch)==false) {
        throw(AlphaExpectedException());
    }

    ++pos;

    if (pos<str.size() && str[pos]==L'_') {
        ++pos;

        if (pos>=str.size() || iswalpha(str[pos])==false) {
            throw(AlphaExpectedException());
        }

        while (pos<str.size() && iswalnum(str[pos])) {
            token += str[pos++];
        }

        Symbol s = dictionary(token);

        switch (ch) {
        case L'v':
            if (s.type==NONE_SYMBOL) {
                Variable v;

                dictionary.insert(token, v);
                dictionaryExtender.merge();
            } else if (s.type!=VARIABLE) {
                throw(VariableExpectedException());
            }

            return;

            break;
        case L'c':
            if (s.type==NONE_SYMBOL) {
                ConstantSymbol c;

                dictionary.insert(token, c);
                dictionaryExtender.merge();
            } else if (s.type!=CONSTANT) {
                throw(ConstantExpectedException());
            }

            return;

        case L'f':
            if (s.type==NONE_SYMBOL) {
                token = std::wstring(L"f_")+token;
            } else if (s.type!=OPERATION) {
                throw(OperationExpectedException());
            }

            return;

        case L'R':
            if (s.type==NONE_SYMBOL) {
                token = std::wstring(L"R_")+token;
            } else if (s.type!=RELATION) {
                throw(RelationExpectedException());
            }

            return;

            break;

        default:
            throw(OneOfExpectedException(L"vcfR"));

            break;
        }
    }

    --pos;

    while (pos<str.size() && iswalnum(str[pos])) {
        token += str[pos++];
    }

    return;

    CASE_END

    throw(UnexpectedEndException());
}

void Reader::expectToken(const std::wstring &token)
{
    std::wstring readedToken;

    CASE_BEGIN
    getToken(readedToken);

    if (readedToken!=token) {
        throw(WrongTokenException(token, readedToken));
    }

    return;

    CASE_END

    throw(UnexpectedEndException());
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

Variable Reader::getVariable(const std::wstring &token, std::wstring &name)
{
    CASE_BEGIN

    if (token.size()>2 && token[1]==L'_') {
        if (token[0]!=L'v') {
            name.clear();

            throw(VariableExpectedException());
        }

        name = token.substr(2);
    } else {
        name = token;
    }

    if (isName(name)==false) {
        name.clear();

        throw(VariableExpectedException());
    }

    Symbol s = dictionary(name);

    if (s.type==VARIABLE) {
        return s;
    }

    if (s.type!=NONE_SYMBOL) {
        name.clear();

        throw(VariableExpectedException());
    }

    Variable v;

    dictionary.insert(name, v);
    dictionaryExtender.merge();

    return v;

    CASE_END

    throw(VariableExpectedException());
}

ConstantSymbol Reader::getConstant(const std::wstring &token, std::wstring &name)
{
    CASE_BEGIN

    if (token.size()>2 && token[1]==L'_') {
        if (token[0]!=L'c') {
            name.clear();

            throw(ConstantExpectedException());
        }

        name = token.substr(2);
    } else {
        name = token;
    }

    if (isName(name)==false) {
        name.clear();

        throw(ConstantExpectedException());
    }

    Symbol s = dictionary(name);

    if (s.type==CONSTANT) {
        return s;
    }

    if (s.type!=NONE_SYMBOL) {
        name.clear();

        throw(ConstantExpectedException());
    }

    ConstantSymbol c;

    dictionary.insert(name, c);
    dictionaryExtender.merge();

    return c;

    CASE_END

    throw(ConstantExpectedException());
}

OperationSymbol Reader::getOperation(const std::wstring &token, std::wstring &name, size_t arity)
{
    CASE_BEGIN

    if (token.size()>2 && token[1]==L'_') {
        if (token[0]!=L'f') {
            name.clear();

            throw(OperationExpectedException());
        }

        name = token.substr(2);
    } else {
        name = token;
    }

    if (isName(name)==false) {
        name.clear();

        throw(OperationExpectedException());
    }

    Symbol s = dictionary(name);

    if (s.type==OPERATION) {
        if (s.arity!=arity) {
            throw(WrongArityException(arity, s.arity));
        }

        dictionaryExtender.merge();

        return s;
    }

    if (s.type!=NONE_SYMBOL) {
        name.clear();

        throw(OperationExpectedException());
    }

    OperationSymbol op(arity);

    dictionary.insert(name, op);
    dictionaryExtender.merge();

    return op;

    CASE_END

    throw(OperationExpectedException());
}

RelationSymbol Reader::getRelation(const std::wstring &token, std::wstring &name, size_t arity)
{
    CASE_BEGIN

    if (token.size()>2 && token[1]==L'_') {
        if (token[0]!=L'R') {
            name.clear();

            throw(RelationExpectedException());
        }

        name = token.substr(2);
    } else {
        name = token;
    }

    if (isName(name)==false) {
        name.clear();

        throw(RelationExpectedException());
    }

    Symbol s = dictionary(name);

    if (s.type==RELATION) {
        if (s.arity!=arity) {
            throw(WrongArityException(arity, s.arity));
        }

        dictionaryExtender.merge();

        return s;
    }

    if (s.type!=NONE_SYMBOL) {
        name.clear();

        throw(RelationExpectedException());
    }

    RelationSymbol r(arity);

    dictionary.insert(name, r);
    dictionaryExtender.merge();

    return r;

    CASE_END

    throw(RelationExpectedException());
}

Term Reader::parseTerm()
{
    std::wstring token;
    std::wstring name;

    CASE_BEGIN
    getToken(token);

    if (token.size()>2 && token[1]==L'_') {
        if (token[0]!=L'f') {
            throw(OperationExpectedException());
        }

        token = token.substr(2);
    }

    if (isName(token)==false) {
        throw(NameExpectedException());
    }

    Symbol s = dictionary(token);

    if (s.type!=NONE_SYMBOL && s.type!=OPERATION) {
        throw(OperationExpectedException());
    }

    std::vector<Term> terms = parseTermList();
    OperationSymbol op = getOperation(token, name, terms.size());

    dictionaryExtender.merge();

    return Term(op, terms);

    CASE_END
    CASE_BEGIN
    getToken(token);

    ConstantSymbol c = getConstant(token, name);

    dictionaryExtender.merge();

    return Term(c);

    CASE_END
    CASE_BEGIN
    getToken(token);

    if (isName(token)==false) {
        throw(NameExpectedException());
    }

    Variable v = getVariable(token, name);

    dictionaryExtender.merge();

    return Term(v);

    CASE_END

    throw(TermExpectedException());
}

std::vector<Term> Reader::parseTermList()
{
    std::wstring token;
    std::vector<Term> result;

    CASE_BEGIN
    expectToken(L"(");

    while (true) {
        CASE_BEGIN
        Term t = parseTerm();

        result.push_back(t);
        dictionaryExtender.merge();
        getToken(token);

        if (token==L")") {
            break;
        }

        if (token!=L",") {
            throw(OneOfExpectedException(L",)"));
        }

        continue;

        CASE_END
        CASE_BEGIN

        getToken(token);

        if (result.empty() && token==L")") {
            break;
        }

        CASE_END

        throw(TermExpectedException());
    }

    dictionaryExtender.merge();

    return result;

    CASE_END

    throw(TermExpectedException());
}

Formula Reader::parseFirstSubformula()
{
    std::wstring token;
    std::wstring name;

    CASE_BEGIN

    Term t = parseTerm();
    std::vector<Term> terms;
    bool nonequality = false;

    terms.push_back(t);

    while (true) {
        CASE_BEGIN
        getToken(token);

        if (token==L"=") {
            terms.push_back(parseTerm());
            dictionaryExtender.merge();
        } else if (token==L"neq") {
            if (terms.size()==1) {
                terms.push_back(parseTerm());
                nonequality = true;
                dictionaryExtender.merge();

                break;
            }

            throw(UnexpectedNonequalityException());
        } else {
            throw(EqualityOrNonequalityExpectedException());
        }

        continue;

        CASE_END

        break;
    }

    if (nonequality) {
        dictionaryExtender.merge();

        return FormulaEnvironment::NonequalityFormula(terms);
    }

    if (terms.size()==1) {
        throw(EqualityOrNonequalityExpectedException());
    }

    dictionaryExtender.merge();

    return FormulaEnvironment::EqualityFormula(terms);

    CASE_END
    CASE_BEGIN
    getToken(token);

    if (token==L"false") {
        dictionaryExtender.merge();

        return FormulaEnvironment::FalseFormula();
    }

    if (token==L"true") {
        dictionaryExtender.merge();

        return FormulaEnvironment::TrueFormula();
    }

    if (token==L"not") {
        Formula f = parseFirstSubformula();

        dictionaryExtender.merge();

        return FormulaEnvironment::NegationFormula(f);
    }

    throw(FormulaExpectedException());

    CASE_END
    CASE_BEGIN

    expectToken(L"(");
    getToken(token);
    toLower(token);

    bool universal = token==L"forall";
    bool existential = token==L"exists";

    if (universal==false && existential==false) {
        throw(QuantifierExpectedException());
    }

    std::vector<Variable> variables;

    while (true) {
        getToken(token);

        if (token==L")") {
            break;
        }

        if (token==L",") {
            if (variables.empty()) {
                throw(VariableExpectedException());
            }

            getToken(token);
        }

        std::wstring name;

        variables.push_back(getVariable(token, name));
    }

    Formula f = parseFirstSubformula();

    dictionaryExtender.merge();

    if (universal) {
        return FormulaEnvironment::UniversalFormula(f, variables);
    }

    return FormulaEnvironment::ExistentialFormula(f, variables);

    CASE_END
    CASE_BEGIN

    expectToken(L"(");
    Formula f = parseImpEqu();
    expectToken(L")");
    dictionaryExtender.merge();

    return f;

    CASE_END
    CASE_BEGIN
    getToken(token);

    if (token.size()>2 && token[1]==L'_') {
        if (token[0]!=L'R') {
            throw(RelationExpectedException());
        }

        token = token.substr(2);
    }

    Symbol s = dictionary(token);

    if (s.type!= NONE_SYMBOL && s.type!=RELATION) {
        throw(RelationExpectedException());
    }

    std::vector<Term> terms = parseTermList();
    RelationSymbol r = getRelation(token, name, terms.size());

    dictionaryExtender.merge();

    return FormulaEnvironment::RelationFormula(r, terms);

    CASE_END

    throw(FormulaExpectedException());
}

Formula Reader::parseConDis()
{
    CASE_BEGIN

    std::vector<Formula> formulas;
    bool conjunction = false;
    std::wstring token;

    while (true) {
        Formula f = parseFirstSubformula();

        formulas.push_back(f);
        CASE_BEGIN
        getToken(token);
        toLower(token);

        if (token==L"and") {
            if (conjunction==false && formulas.size()>1) {
                throw(ConjunctionExpectedException());
            }

            conjunction = true;
        } else if (token==L"or") {
            if (conjunction && formulas.size()>1) {
                throw(DisjunctionExpectedException());
            }

            conjunction = false;
        } else {
            throw(ConjunctionOrDisjunctionExpectedException());
        }

        continue;

        CASE_END

        break;
    }

    if (formulas.empty()) {
        throw(FormulaExpectedException());
    }

    dictionaryExtender.merge();

    if (formulas.size()==1) {
        return formulas[0];
    }

    if (conjunction) {
        return FormulaEnvironment::ConjunctionFormula(formulas);
    }

    return FormulaEnvironment::DisjunctionFormula(formulas);

    CASE_END

    throw(ConjunctionOrDisjunctionExpectedException());
}

Formula Reader::parseImpEqu()
{
    std::vector<bool> connectives;
    std::vector<Formula> formulas;
    std::wstring token;

    CASE_BEGIN

    while (true) {
        bool impl;
        bool equiv;
        Formula f = parseConDis();

        formulas.push_back(f);
        CASE_BEGIN
        getToken(token);
        toLower(token);
        impl = token==L"imp";
        equiv = impl==false && token==L"equ";

        if (impl==false && equiv==false) {
            throw(ImplicationOrEquivalenceExpectedException());
        }

        connectives.push_back(impl);

        continue;

        CASE_END

        break;
    }

    if (formulas.empty()) {
        throw(FormulaExpectedException());
    }

    dictionaryExtender.merge();

    if (formulas.size()==1) {
        return formulas[0];
    }

    std::vector<Formula> result;
    size_t i = 0;
    size_t j = 0;

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
    }

    if (result.size()==1) {
        return result[0];
    }

    return FormulaEnvironment::ConjunctionFormula(std::move(result));

    CASE_END

    throw(ImplicationOrEquivalenceExpectedException());
}

Formula Reader::parseFormula()
{
    CASE_BEGIN
    Formula result = parseImpEqu();

    expectToken(L".");
    dictionaryExtender.merge();

    return result;
    CASE_END

    throw(FormulaExpectedException());
}
