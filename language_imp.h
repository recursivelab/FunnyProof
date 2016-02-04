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

#ifndef LANGUAGE_IMP_H
#define LANGUAGE_IMP_H

#include "language.h"

Symbol::Symbol() :
    type(NONE_SYMBOL),
    id(0),
    arity(0)
{
}

Symbol::Symbol(SymbolType type, std::size_t arity) :
    type(type),
    id([]() -> uint64_t { static std::atomic<uint64_t> counter; return ++counter; }()),
    arity(arity)
{
}

const Symbol& Symbol::dummy()
{
    static Symbol object;

    return object;
}

Symbol::Symbol(const Symbol &other) :
    type(other.type),
    id(other.id),
    arity(other.arity)
{
}

bool Symbol::operator ==(const Symbol &other) const
{
    return other.id == id;
}

bool Symbol::operator !=(const Symbol &other) const
{
    return other.id != id;
}

bool Symbol::operator <(const Symbol &other) const
{
    return id < other.id;
}

int Symbol::compare(const Symbol &other) const
{
    if (id < other.id) {
        return -1;
    }

    return id > other.id;
}

NegationSymbol::NegationSymbol() :
    Symbol(NEGATION)
{
}

const NegationSymbol& NegationSymbol::instance()
{
    static NegationSymbol result;

    return result;
}

const NegationSymbol& negation()
{
    return NegationSymbol::instance();
}

ConjunctionSymbol::ConjunctionSymbol() :
    Symbol(CONJUNCTION)
{
}

const ConjunctionSymbol& ConjunctionSymbol::instance()
{
    static ConjunctionSymbol result;

    return result;
}

const ConjunctionSymbol& conjunction()
{
    return ConjunctionSymbol::instance();
}

DisjunctionSymbol::DisjunctionSymbol() :
    Symbol(DISJUNCTION)
{
}

const DisjunctionSymbol& DisjunctionSymbol::instance()
{
    static DisjunctionSymbol result;

    return result;
}

const DisjunctionSymbol& disjunction()
{
    return DisjunctionSymbol::instance();
}

ImplicationSymbol::ImplicationSymbol() :
    Symbol(IMPLICATION)
{
}

const ImplicationSymbol& ImplicationSymbol::instance()
{
    static ImplicationSymbol result;

    return result;
}

const ImplicationSymbol& implication()
{
    return ImplicationSymbol::instance();
}

EquivalenceSymbol::EquivalenceSymbol() :
    Symbol(EQUIVALENCE)
{
}

const EquivalenceSymbol& EquivalenceSymbol::instance()
{
    static EquivalenceSymbol result;

    return result;
}

const EquivalenceSymbol& equivalence()
{
    return EquivalenceSymbol::instance();
}

UniversalSymbol::UniversalSymbol() :
    Symbol(UNIVERSAL)
{
}

const UniversalSymbol& UniversalSymbol::instance()
{
    static UniversalSymbol result;

    return result;
}

const UniversalSymbol& universal()
{
    return UniversalSymbol::instance();
}

ExistentialSymbol::ExistentialSymbol() :
    Symbol(EXISTENTIAL)
{
}

const ExistentialSymbol& ExistentialSymbol::instance()
{
    static ExistentialSymbol result;

    return result;
}

const ExistentialSymbol& existential()
{
    return ExistentialSymbol::instance();
}

EqualitySymbol::EqualitySymbol() :
    Symbol(EQUALITY)
{
}

const EqualitySymbol& EqualitySymbol::instance()
{
    static EqualitySymbol result;

    return result;
}

const EqualitySymbol& equality()
{
    return EqualitySymbol::instance();
}

OperationSymbol::OperationSymbol(std::size_t arity) :
    Symbol(OPERATION, arity)
{
}

OperationSymbol::OperationSymbol(const OperationSymbol &other) :
    Symbol(other)
{
}

OperationSymbol::OperationSymbol(const Symbol &symbol) :
    Symbol(symbol)
{
    if (symbol.type != OPERATION) {
        throw(0);
    }
}

ConstantSymbol::ConstantSymbol() :
    Symbol(CONSTANT, 0)
{
}

ConstantSymbol::ConstantSymbol(const ConstantSymbol &other) :
    Symbol(other)
{
}

ConstantSymbol::ConstantSymbol(const Symbol &symbol) :
    Symbol(symbol)
{
    if (symbol.type != CONSTANT) {
        throw(0);
    }
}

RelationSymbol::RelationSymbol(std::size_t arity) :
    Symbol(RELATION, arity)
{
}

RelationSymbol::RelationSymbol(const RelationSymbol &other) :
    Symbol(other)
{
}

Variable::Variable(const Symbol &other) :
    Symbol(other)
{
    if (other.type != VARIABLE) {
        throw(0);
    }
}

Variable::Variable() :
    Symbol(VARIABLE)
{
}

Variable::Variable(const Variable &other) :
    Symbol(other)
{
}

TermEnvironment::TermPrivate::TermPrivate(Symbol symbol, const std::vector<Term> &args) :
    symbol(symbol),
    args(args),
    freeVariables(),
    freeVariablesComputed(symbol.type == CONSTANT),
    refs(1)
{
    if (args.size() != symbol.arity) {
        throw(0);
    }
}

TermEnvironment::TermPrivate::TermPrivate(Symbol symbol) :
    symbol(symbol),
    args(),
    freeVariables(),
    freeVariablesComputed(symbol.type == CONSTANT),
    refs(1)
{
}

TermEnvironment::TermPrivate::TermPrivate(Symbol symbol, std::vector<Term> &&args) :
    symbol(symbol),
    args(std::move(args)),
    freeVariables(),
    freeVariablesComputed(symbol.type == CONSTANT),
    refs(1)
{
    if (args.size() != symbol.arity) {
        throw(0);
    }
}

TermEnvironment::TermPrivate::TermPrivate(const TermPrivate &other) :
    symbol(other.symbol),
    args(other.args),
    freeVariables(other.freeVariables),
    freeVariablesComputed(other.freeVariablesComputed),
    refs(other.refs)
{
}

TermEnvironment::TermPrivate::TermPrivate(TermPrivate &&other) :
    symbol(other.symbol),
    args(std::move(other.args)),
    freeVariables(std::move(other.freeVariables)),
    freeVariablesComputed(other.freeVariablesComputed),
    refs(other.refs)
{
}

bool TermEnvironment::TermPrivate::operator ==(const TermPrivate &other) const
{
    if (symbol != other.symbol) {
        return false;
    }

    if (symbol.arity == 0) {
        return true;
    }

    if (args.size() != other.args.size()) {
        return false;
    }

    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] != other.args[i]) {
            return false;
        }
    }

    return true;
}

bool TermEnvironment::TermPrivate::operator !=(const TermPrivate &other) const
{
    return operator ==(other) == false;
}

int TermEnvironment::TermPrivate::compare(const TermPrivate &other) const
{
    int result = symbol.compare(other.symbol);

    if (result != 0) {
        return result;
    }

    for (std::size_t i = 0; i < args.size(); ++i) {
        result = args[i].compare(other.args[i]);

        if (result != 0) {
            return result;
        }
    }

    return 0;
}

bool TermEnvironment::TermPrivate::operator <(const TermPrivate &other) const
{
   return compare(other) < 0;
}

bool TermEnvironment::TermPrivate::isFreeVariable(const Variable &variable) const
{
    switch (symbol.id) {
    case VARIABLE:
        return symbol == variable;

        break;

    case CONSTANT:
        return false;

        break;

    case OPERATION:
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (args[i].isFreeVariable(variable)) {
                return true;
            }
        }

        return false;

        break;

    default:
        throw(0);

        break;
    }
}

const std::set<Variable>& TermEnvironment::TermPrivate::getFreeVariables() const
{
    if (freeVariablesComputed == false) {
        switch (symbol.type) {
        case VARIABLE:
            freeVariables.insert(symbol);

            break;

        case OPERATION:
            for (std::size_t i = 0; i < args.size(); ++i) {
                const std::set<Variable> &variables = args[i].getFreeVariables();

                freeVariables.insert(variables.cbegin(), variables.cend());
            }

        default:
            throw(0);

            break;
        }

        freeVariablesComputed = true;
    }

    return freeVariables;
}

TermEnvironment::VariableTermPrivate::VariableTermPrivate(const Variable &variable) :
    TermPrivate(variable)
{
}

TermEnvironment::ConstantTermPrivate::ConstantTermPrivate(const ConstantSymbol &constant) :
    TermPrivate(constant)
{
}

TermEnvironment::OperationTermPrivate::OperationTermPrivate(const OperationSymbol &operation, const std::vector<Term> &args) :
    TermPrivate(operation, args)
{
}

TermEnvironment::OperationTermPrivate::OperationTermPrivate(const OperationSymbol &operation, std::vector<Term> &&args) :
    TermPrivate(operation, std::move(args))
{
}

TermEnvironment::Substitution::Substitution(const std::map<Variable, Term> &valuation) :
    data(valuation)
{
}

TermEnvironment::Substitution::Substitution(std::map<Variable, Term> &&valuation) :
    data(valuation)
{
}

TermEnvironment::Term TermEnvironment::Substitution::operator ()(const Variable &variable) const
{
    std::map<Variable, Term>::const_iterator i = data.find(variable);

    if (i != data.cend()) {
        return i->second;
    }

    return Term(variable);
}

std::set<TermEnvironment::TermPrivate>& TermEnvironment::Term::variableTerms()
{
    thread_local static std::set<TermPrivate> object;

    return object;
}

std::set<TermEnvironment::TermPrivate>& TermEnvironment::Term::constantTerms()
{
    thread_local static std::set<TermPrivate> object;

    return object;
}

std::set<TermEnvironment::TermPrivate>& TermEnvironment::Term::operationTerms()
{
    thread_local static std::set<TermPrivate> object;

    return object;
}

const std::set<TermEnvironment::TermPrivate>::const_iterator TermEnvironment::Term::insert(const TermPrivate &term)
{
    thread_local static std::pair<std::set<TermPrivate>::const_iterator, bool> pair;
    std::set<TermPrivate> *termsSet;

    switch (term.symbol.type) {
    case VARIABLE:
        termsSet = &variableTerms();

        break;

    case CONSTANT:
        termsSet = &constantTerms();

        break;

    case OPERATION:
        termsSet = &operationTerms();

        break;

    default:
        throw(0);

        break;
    }

    pair = termsSet->insert(term);

    if (pair.second == false) {
        ++(pair.first->refs);
    }

    return pair.first;
}

TermEnvironment::Term::Term() :
    position()
{
}

TermEnvironment::Term::Term(const Term &other) :
    position(other.position)
{
}

TermEnvironment::Term::Term(const Variable &variable) :
    position(insert(VariableTermPrivate(variable)))
{
}

TermEnvironment::Term::Term(const ConstantSymbol &constantSymbol) :
    position(insert(ConstantTermPrivate(constantSymbol)))
{
}

TermEnvironment::Term::Term(const OperationSymbol &operationSymbol, const std::vector<Term> &args) :
    position(insert(OperationTermPrivate(operationSymbol, args)))
{
}

TermEnvironment::Term::Term(const OperationSymbol &operationSymbol, std::vector<Term> &&args) :
    position(insert(OperationTermPrivate(operationSymbol, args)))
{
}

TermEnvironment::Term::~Term()
{
    const TermPrivate *data = &*position;

    if (data != nullptr && --(data->refs) == 0) {
        switch (data->symbol.id) {
        case VARIABLE:
            variableTerms().erase(position);

            break;

        case CONSTANT:
            constantTerms().erase(position);

            break;

        case OPERATION:
            operationTerms().erase(position);

            break;

        default:
            break;
        }
    }
}

bool TermEnvironment::Term::operator ==(const Term &other) const
{
    if (isEmpty()) {
        return other.isEmpty();
    }

    if (other.isEmpty()) {
        return false;
    }

    return position == other.position;
}

bool TermEnvironment::Term::operator !=(const Term &other) const
{
    return operator ==(other) == false;
}

int TermEnvironment::Term::compare(const Term &other) const
{
    if (isEmpty()) {
        return other.isEmpty() ? 0 : -1;
    }

    if (other.isEmpty()) {
        return 1;
    }

    return position->compare(*other.position);
}

bool TermEnvironment::Term::operator <(const Term &other) const
{
    if (isEmpty()) {
        return other.isEmpty() == false;
    }

    if (other.isEmpty()) {
        return false;
    }

    return position->symbol < other.position->symbol;
}

SymbolType TermEnvironment::Term::type() const
{
    return position->symbol.type;
}

uint64_t TermEnvironment::Term::id() const
{
    return position->symbol.id;
}

const Symbol& TermEnvironment::Term::symbol() const
{
    return position->symbol;
}

size_t TermEnvironment::Term::arity() const
{
    return position->symbol.arity;
}

bool TermEnvironment::Term::isFreeVariable(const Variable &variable) const
{
    return position->isFreeVariable(variable);
}

const std::set<Variable>& TermEnvironment::Term::getFreeVariables() const
{
    return position->getFreeVariables();
}

bool TermEnvironment::Term::isEmpty() const
{
    return &*position == nullptr;
}

TermEnvironment::Term TermEnvironment::Term::operator [](const Substitution &valuation) const
{
    if (isEmpty()) {
        return Term();
    }

    const TermPrivate &term = *position;

    switch (term.symbol.type) {
    case VARIABLE:
        return valuation(term.symbol);

        break;

    case CONSTANT:
        return *this;

        break;

    case OPERATION:
        {
            std::vector<Term> args;
            for (size_t i = 0; i < term.args.size(); ++i) {
                args.push_back(term.args[i][valuation]);
            }

            return Term(term.symbol, args);

            break;
        }
    default:
        throw(0);

        break;
    }
}

const TermEnvironment::Term& TermEnvironment::Term::dummy()
{
    static Term result;

    return result;
}

std::vector<Variable> TermEnvironment::oneVariable(const Variable &variable)
{
    std::vector<Variable> result;

    result.push_back(variable);

    return result;
}

std::vector<Variable> TermEnvironment::oneVariable(Variable &&variable)
{
    std::vector<Variable> result;

    result.push_back(variable);

    return result;
}

std::vector<TermEnvironment::Term> TermEnvironment::oneTerm(const TermEnvironment::Term &term)
{
    std::vector<Term> result;

    result.push_back(term);

    return result;
}

std::vector<TermEnvironment::Term> TermEnvironment::oneTerm(TermEnvironment::Term &&term)
{
    std::vector<Term> result;

    result.push_back(term);

    return result;
}

std::vector<TermEnvironment::Term> TermEnvironment::twoTerms(const TermEnvironment::Term &term1, const TermEnvironment::Term &term2)
{
    std::vector<Term> result;

    result.push_back(term1);
    result.push_back(term2);

    return result;
}

std::vector<TermEnvironment::Term> TermEnvironment::twoTerms(const TermEnvironment::Term &term1, TermEnvironment::Term &&term2)
{
    std::vector<Term> result;

    result.push_back(term1);
    result.push_back(term2);

    return result;
}

std::vector<TermEnvironment::Term> TermEnvironment::twoTerms(TermEnvironment::Term &&term1, const TermEnvironment::Term &term2)
{
    std::vector<Term> result;

    result.push_back(term1);
    result.push_back(term2);

    return result;
}

std::vector<TermEnvironment::Term> TermEnvironment::twoTerms(TermEnvironment::Term &&term1, TermEnvironment::Term &&term2)
{
    std::vector<Term> result;

    result.push_back(term1);
    result.push_back(term2);

    return result;
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate() :
    symbol(Symbol::dummy())
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const std::vector<Term> &terms) :
    symbol(symbol),
    terms(terms),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, std::vector<Term> &&terms) :
    symbol(symbol),
    terms(terms),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const std::vector<Formula> &formulas) :
    symbol(symbol),
    formulas(formulas),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, std::vector<Formula> &&formulas) :
    symbol(symbol),
    formulas(formulas),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const Formula &formula, const std::vector<Variable> &variables) :
    symbol(symbol),
    formulas(oneFormula(formula)),
    variables(variables),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const Formula &formula, std::vector<Variable> &&variables) :
    symbol(symbol),
    formulas(oneFormula(formula)),
    variables(variables),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const std::vector<Term> &terms, std::vector<Formula> &&formulas) :
    symbol(symbol),
    terms(terms),
    formulas(formulas),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, std::vector<Term> &&terms, const std::vector<Formula> &formulas) :
    symbol(symbol),
    terms(terms),
    formulas(formulas),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, std::vector<Term> &&terms, std::vector<Formula> &&formulas) :
    symbol(symbol),
    terms(terms),
    formulas(formulas),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const FormulaPrivate &other) :
    symbol(other.symbol),
    terms(other.terms),
    formulas(other.formulas),
    freeVariables(),
    freeVariablesComputed(false),
    refs(1)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(FormulaPrivate &&other) :
    symbol(other.symbol),
    terms(std::move(other.terms)),
    formulas(std::move(other.formulas)),
    freeVariables(std::move(other.freeVariables)),
    freeVariablesComputed(other.freeVariablesComputed),
    refs(1)
{
}

bool FormulaEnvironment::FormulaPrivate::operator ==(const FormulaPrivate &other) const
{
    if (symbol != other.symbol || terms.size() != other.terms.size() || formulas.size() != other.formulas.size()) {
        return false;
    }

    for (std::size_t i = 0; i < terms.size(); ++i) {
        if (terms[i] != other.terms[i]) {
            return false;
        }
    }

    for (std::size_t i = 0; i < formulas.size(); ++i) {
        if (formulas[i] != other.formulas[i]) {
            return false;
        }
    }

    return true;
}

bool FormulaEnvironment::FormulaPrivate::operator !=(const FormulaPrivate &other) const
{
    return operator ==(other) == false;
}

int FormulaEnvironment::FormulaPrivate::compare(const FormulaPrivate &other) const
{
    int result = symbol.compare(other.symbol);

    if (result) {
        return result;
    }

    if (terms.size() != other.terms.size()) {
        return (terms.size() < other.terms.size()) ? -1 : 1;
    }

    if (formulas.size() != other.formulas.size()) {
        return (formulas.size() < other.formulas.size()) ? -1 : 1;
    }

    for (std::size_t i = 0; i < terms.size(); ++i) {
        result = terms[i].compare(other.terms[i]);

        if (result != 0) {
            return result;
        }
    }

    for (std::size_t i = 0; i < formulas.size(); ++i) {
        result = formulas[i].compare(other.formulas[i]);

        if (result != 0) {
            return result;
        }
    }

    return 0;
}

bool FormulaEnvironment::FormulaPrivate::operator <(const FormulaPrivate &other) const
{
    return compare(other) < 0;
}

bool FormulaEnvironment::FormulaPrivate::isFreeVariable(const Variable &variable) const
{
    switch (symbol.type) {
    case EQUALITY:
    case RELATION:
        for (size_t i = 0; i < terms.size(); ++i) {
            if (terms[i].isFreeVariable(variable)) {
                return true;
            }
        }

        return false;

        break;

    case NEGATION:
    case CONJUNCTION:
    case DISJUNCTION:
    case IMPLICATION:
    case EQUIVALENCE:
        for (size_t i = 0; i < formulas.size(); ++i) {
            if (formulas[i].isFreeVariable(variable)) {
                return true;
            }
        }

        return false;

    case UNIVERSAL:
    case EXISTENTIAL:
        for (size_t i = 0; i < variables.size(); ++i)
        if (variables[i] == variable) {
            return false;
        }

        for (size_t i = 0; i < formulas.size(); ++i) {
            if (formulas[i].isFreeVariable(variable)) {
                return true;
            }
        }

        return false;

    default:
        throw(1);

        break;
    }
}

const std::set<Variable>& FormulaEnvironment::FormulaPrivate::getFreeVariables() const
{
    if (freeVariablesComputed == false) {
        switch (symbol.type) {
        case EQUALITY:
        case RELATION:
            freeVariables.clear();

            for (size_t i = 0; i < terms.size(); ++i) {
                const std::set<Variable> &variables = terms[i].getFreeVariables();

                freeVariables.insert(variables.begin(), variables.end());
            }

            break;

        case NEGATION:
        case CONJUNCTION:
        case DISJUNCTION:
        case IMPLICATION:
        case EQUIVALENCE:
            freeVariables.clear();

            for (size_t i = 0; i < formulas.size(); ++i) {
                const std::set<Variable> &variables = formulas[i].getFreeVariables();

                freeVariables.insert(variables.begin(), variables.end());
            }

            break;

        case UNIVERSAL:
        case EXISTENTIAL:
            freeVariables.clear();

            for (size_t i = 0; i < formulas.size(); ++i) {
                const std::set<Variable> &variables = formulas[i].getFreeVariables();

                freeVariables.insert(variables.begin(), variables.end());
            }

            for (size_t i = 0; i < variables.size(); ++i) {
                freeVariables.erase(variables[i]);
            }

            break;

        default:
            throw(1);

            break;
        }

        freeVariablesComputed = true;
    }

    return freeVariables;
}

std::vector<FormulaEnvironment::Formula> FormulaEnvironment::oneFormula(const Formula &formula)
{
    std::vector<Formula> result;

    result.push_back(formula);

    return result;
}

std::vector<FormulaEnvironment::Formula> FormulaEnvironment::twoFormulas(const Formula &formula1, const Formula &formula2)
{
    std::vector<Formula> result;

    result.push_back(formula1);
    result.push_back(formula2);

    return result;
}

FormulaEnvironment::EqualityFormulaPrivate::EqualityFormulaPrivate(const std::vector<Term> &terms) :
    FormulaPrivate(equality(), terms, std::vector<Formula>())
{
}

FormulaEnvironment::EqualityFormulaPrivate::EqualityFormulaPrivate(std::vector<Term> &&terms) :
    FormulaPrivate(equality(), terms, std::vector<Formula>())
{
}

FormulaEnvironment::EqualityFormulaPrivate::EqualityFormulaPrivate(const Term &term1, const Term &term2) :
    EqualityFormulaPrivate(TermEnvironment::twoTerms(term1, term2))
{
}

FormulaEnvironment::RelationFormulaPrivate::RelationFormulaPrivate(const RelationSymbol &symbol, const std::vector<Term> &terms) :
    FormulaPrivate(symbol, terms, std::vector<Formula>())
{
    if (terms.size() != symbol.arity) {
        throw(0);
    }
}

FormulaEnvironment::RelationFormulaPrivate::RelationFormulaPrivate(const RelationSymbol &symbol, std::vector<Term> &&terms) :
    FormulaPrivate(symbol, terms, std::vector<Formula>())
{
    if (terms.size() != symbol.arity) {
        throw(0);
    }
}

FormulaEnvironment::NegationFormulaPrivate::NegationFormulaPrivate(const Formula &formula) :
    FormulaPrivate(negation(), std::vector<Term>(), oneFormula(formula))
{
}

FormulaEnvironment::ConjunctionFormulaPrivate::ConjunctionFormulaPrivate(const std::vector<Formula> &formulas) :
    FormulaPrivate(conjunction(), std::vector<Term>(), formulas)
{
}

FormulaEnvironment::ConjunctionFormulaPrivate::ConjunctionFormulaPrivate(std::vector<Formula> &&formulas) :
    FormulaPrivate(conjunction(), std::vector<Term>(), formulas)
{
}

FormulaEnvironment::ConjunctionFormulaPrivate::ConjunctionFormulaPrivate(const Formula &formula1, const Formula &formula2) :
    FormulaPrivate(conjunction(), std::vector<Term>(), twoFormulas(formula1, formula2))
{
}

FormulaEnvironment::DisjunctionFormulaPrivate::DisjunctionFormulaPrivate(const std::vector<Formula> &formulas) :
    FormulaPrivate(disjunction(), std::vector<Term>(), formulas)
{
}

FormulaEnvironment::DisjunctionFormulaPrivate::DisjunctionFormulaPrivate(std::vector<Formula> &&formulas) :
    FormulaPrivate(disjunction(), std::vector<Term>(), formulas)
{
}

FormulaEnvironment::DisjunctionFormulaPrivate::DisjunctionFormulaPrivate(const Formula &formula1, const Formula &formula2) :
    FormulaPrivate(disjunction(), std::vector<Term>(), twoFormulas(formula1, formula2))
{
}

FormulaEnvironment::ImplicationFormulaPrivate::ImplicationFormulaPrivate(const std::vector<Formula> &formulas) :
    FormulaPrivate(implication(), std::vector<Term>(), formulas)
{
}

FormulaEnvironment::ImplicationFormulaPrivate::ImplicationFormulaPrivate(std::vector<Formula> &&formulas) :
    FormulaPrivate(implication(), std::vector<Term>(), formulas)
{
}

FormulaEnvironment::ImplicationFormulaPrivate::ImplicationFormulaPrivate(const Formula &formula1, const Formula &formula2) :
    FormulaPrivate(implication(), std::vector<Term>(), twoFormulas(formula1, formula2))
{
}

FormulaEnvironment::EquivalenceFormulaPrivate::EquivalenceFormulaPrivate(const std::vector<Formula> &formulas) :
    FormulaPrivate(equivalence(), std::vector<Term>(), formulas)
{
}

FormulaEnvironment::EquivalenceFormulaPrivate::EquivalenceFormulaPrivate(std::vector<Formula> &&formulas) :
    FormulaPrivate(equivalence(), std::vector<Term>(), formulas)
{
}

FormulaEnvironment::EquivalenceFormulaPrivate::EquivalenceFormulaPrivate(const Formula &formula1, const Formula &formula2) :
    FormulaPrivate(equivalence(), std::vector<Term>(), twoFormulas(formula1, formula2))
{
}

FormulaEnvironment::UniversalFormulaPrivate::UniversalFormulaPrivate(const Formula &formula, const Variable &variable) :
    FormulaPrivate(universal(), formula, TermEnvironment::oneVariable(variable))
{
}

FormulaEnvironment::UniversalFormulaPrivate::UniversalFormulaPrivate(const Formula &formula, const std::vector<Variable> &variables) :
    FormulaPrivate(universal(), formula, variables)
{
}

FormulaEnvironment::UniversalFormulaPrivate::UniversalFormulaPrivate(const Formula &formula, std::vector<Variable> &&variables) :
    FormulaPrivate(universal(), formula, variables)
{
}

FormulaEnvironment::ExistentialFormulaPrivate::ExistentialFormulaPrivate(const Formula &formula, const Variable &variable) :
    FormulaPrivate(existential(), formula, TermEnvironment::oneVariable(variable))
{
}

FormulaEnvironment::ExistentialFormulaPrivate::ExistentialFormulaPrivate(const Formula &formula, const std::vector<Variable> &variables) :
    FormulaPrivate(existential(), formula, variables)
{
}

FormulaEnvironment::ExistentialFormulaPrivate::ExistentialFormulaPrivate(const Formula &formula, std::vector<Variable> &&variables) :
    FormulaPrivate(existential(), formula, variables)
{
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::emptyFormulaSet()
{
    static std::set<FormulaPrivate> result;

    result.insert(FormulaPrivate());

    return result;
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::equalityFormulas()
{
    thread_local static std::set<FormulaPrivate> result;

    return result;
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::relationFormulas()
{
    thread_local static std::set<FormulaPrivate> result;

    return result;
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::negationFormulas()
{
    thread_local static std::set<FormulaPrivate> result;

    return result;
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::conjunctionFormulas()
{
    thread_local static std::set<FormulaPrivate> result;

    return result;
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::disjunctionFormulas()
{
    thread_local static std::set<FormulaPrivate> result;

    return result;
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::impicationFormulas()
{
    thread_local static std::set<FormulaPrivate> result;

    return result;
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::equivalenceFormulas()
{
    thread_local static std::set<FormulaPrivate> result;

    return result;
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::universalFormulas()
{
    thread_local static std::set<FormulaPrivate> result;

    return result;
}

std::set<FormulaEnvironment::FormulaPrivate>& FormulaEnvironment::Formula::existentialFormulas()
{
    thread_local static std::set<FormulaPrivate> result;

    return result;
}

const std::set<FormulaEnvironment::FormulaPrivate>::const_iterator FormulaEnvironment::Formula::insert(const FormulaPrivate &formula)
{
    thread_local static std::pair<std::set<FormulaPrivate>::const_iterator, bool> pair;
    std::set<FormulaPrivate> *formulasSet;

    switch (formula.symbol.type) {
    case RELATION:
        formulasSet = &relationFormulas();

    case EQUALITY:
        formulasSet = &equalityFormulas();

        break;
    case NEGATION:
        formulasSet = &negationFormulas();

        break;

    case CONJUNCTION:
        formulasSet = &conjunctionFormulas();

        break;

    case DISJUNCTION:
        formulasSet = &disjunctionFormulas();

        break;

    case IMPLICATION:
        formulasSet = &impicationFormulas();

        break;

    case EQUIVALENCE:
        formulasSet = &equivalenceFormulas();

        break;

    case UNIVERSAL:
        formulasSet = &universalFormulas();

        break;

    case EXISTENTIAL:
        formulasSet = &existentialFormulas();

        break;

    default:
        throw(0);

        break;
    }

    pair = formulasSet->insert(formula);

    if (pair.second == false) {
        ++(pair.first->refs);
    }

    return pair.first;
}

const std::set<FormulaEnvironment::FormulaPrivate>::const_iterator& FormulaEnvironment::Formula::emptyPosition()
{
    static std::set<FormulaPrivate>::const_iterator result(
                []() -> std::set<FormulaPrivate>::const_iterator {
                    std::set<FormulaPrivate> set;
                    set.insert(FormulaPrivate());

                    return set.cbegin();
                }());

    return result;
}

FormulaEnvironment::Formula::Formula() :
    position(emptyPosition())
{
}

FormulaEnvironment::Formula::Formula(const Formula &other)
{
    if (other.isEmpty()) {
        return;
    }

    ++(other.position->refs);
    const_cast<std::set<FormulaPrivate>::const_iterator&>(position) = other.position;
}

bool FormulaEnvironment::Formula::operator ==(const Formula &other) const
{
    return position == other.position;
}

bool FormulaEnvironment::Formula::operator !=(const Formula &other) const
{
    return position != other.position;
}

int FormulaEnvironment::Formula::compare(const Formula &other) const
{
    const FormulaPrivate &object = *position;

    return object.compare(*(other.position));
}

bool FormulaEnvironment::Formula::operator <(const Formula &other) const
{
    const FormulaPrivate &object = *position;

    return object < *(other.position);
}

const Symbol& FormulaEnvironment::Formula::symbol() const
{
    return position->symbol;
}

SymbolType FormulaEnvironment::Formula::type() const
{
    return position->symbol.type;
}

uint64_t FormulaEnvironment::Formula::id() const
{
    return position->symbol.id;
}

const std::vector<Term>& FormulaEnvironment::Formula::terms() const
{
    return position->terms;
}

const std::vector<Formula>& FormulaEnvironment::Formula::formulas() const
{
    return position->formulas;
}

const std::vector<Variable>& FormulaEnvironment::Formula::variables() const
{
    return position->variables;
}

bool FormulaEnvironment::Formula::isFreeVariable(const Variable &variable) const
{
    return position->isFreeVariable(variable);
}

const std::set<Variable>& FormulaEnvironment::Formula::getFreeVariables() const
{
    return position->getFreeVariables();
}

bool FormulaEnvironment::Formula::isEmpty() const
{
    return position == emptyPosition();
}

FormulaEnvironment::Formula FormulaEnvironment::Formula::operator [](const TermEnvironment::Substitution &substitution) const
{
    std::map<Variable, Term> data;

    switch (position->symbol.type) {
    case NONE_SYMBOL:
        return *this;

        break;

    case RELATION:
    {
        const std::vector<Term> &terms = position->terms;
        std::vector<Term> result;

        for (size_t i = 0; i < terms.size(); ++i) {
            result.push_back(terms[i][substitution]);
        }

        return RelationFormula(position->symbol, result);
    }

        break;

    case EQUALITY:
    {
        const std::vector<Term> &terms = position->terms;
        std::vector<Term> result;

        for (size_t i = 0; i < terms.size(); ++i) {
            result.push_back(terms[i][substitution]);
        }

        return EqualityFormula(result);
    }

        break;

    case NEGATION:
        return NegationFormula(position->formulas[0][substitution]);

        break;

    case CONJUNCTION:
    {
        const std::vector<Formula> &formulas = position->formulas;
        std::vector<Formula> result;

        for (size_t i = 0; i < formulas.size(); ++i) {
            result.push_back(formulas[i][substitution]);
        }

        return ConjunctionFormula(result);
    }

        break;

    case DISJUNCTION:
    {
        const std::vector<Formula> &formulas = position->formulas;
        std::vector<Formula> result;

        for (size_t i = 0; i < formulas.size(); ++i) {
            result.push_back(formulas[i][substitution]);
        }

        return DisjunctionFormula(result);
    }

        break;

    case IMPLICATION:
    {
        const std::vector<Formula> &formulas = position->formulas;
        std::vector<Formula> result;

        for (size_t i = 0; i < formulas.size(); ++i) {
            result.push_back(formulas[i][substitution]);
        }

        return ImplicationFormula(result);
    }

        break;

    case EQUIVALENCE:
    {
        const std::vector<Formula> &formulas = position->formulas;
        std::vector<Formula> result;

        for (size_t i = 0; i < formulas.size(); ++i) {
            result.push_back(formulas[i][substitution]);
        }

        return EquivalenceFormula(result);
    }

        break;

    case UNIVERSAL: case EXISTENTIAL:
    {
        const std::set<Variable> &fv = getFreeVariables();

        for (std::map<Variable, Term>::const_iterator i = substitution.data.cbegin(); i != substitution.data.cend(); ++i) {
            Variable x = i->first;

            if (fv.count(x)) {
                data.insert(*i);
            }
        }

        Substitution sub(std::move(data));
        std::map<Variable, Term> s;
        std::vector<Variable> qs;
        std::set<Variable> occured;
        std::vector<Variable> v = variables();

        for (size_t i = 0; i < v.size(); ++i) {
            if (occured.find(v[i]) == occured.cend()) {
                qs.push_back(v[i]);
                occured.insert(v[i]);
            }
        }

        for (std::map<Variable, Term>::const_iterator i = sub.data.cbegin(); i != sub.data.cend(); ++i) {
            Variable x = i->first;
            Term t = i->second;
            std::set<Variable> tv = t.getFreeVariables();

            if (x != t.symbol() && fv.find(x) != fv.cend() && tv.find(x) != tv.cend()) {
                Variable y;

                s.insert(std::pair<Variable, Term>(x, Term(y)));
            }
        }

        v.clear();

        for (size_t i = 0; i < qs.size(); ++i) {
            std::map<Variable, Term>::const_iterator pos = s.find(qs[i]);

            if (pos == s.cend()) {
                v.push_back(qs[i]);
            } else {
                v.push_back(pos->second.symbol());
            }
        }

        if (type() == UNIVERSAL) {
            return UniversalFormula(formulas()[0][s][sub], v);
        }

        return ExistentialFormula(formulas()[0][s][sub], v);
    }

        break;

    default:
        break;
    }

    throw(0);
}

const FormulaEnvironment::Formula& FormulaEnvironment::Formula::dummy()
{
    static Formula result;

    return result;
}

FormulaEnvironment::Formula::Formula(const std::set<FormulaPrivate>::const_iterator &position) :
    position(position)
{
}

FormulaEnvironment::RelationFormula::RelationFormula(const RelationSymbol &symbol, const std::vector<Term> &terms) :
    Formula(Formula::insert(RelationFormulaPrivate(symbol, terms)))
{
}

FormulaEnvironment::RelationFormula::RelationFormula(const RelationSymbol &symbol, std::vector<Term> &&terms) :
    Formula(Formula::insert(RelationFormulaPrivate(symbol, terms)))
{
}

FormulaEnvironment::EqualityFormula::EqualityFormula(const std::vector<Term> &terms) :
    Formula(Formula::insert(EqualityFormulaPrivate(terms)))
{
}

FormulaEnvironment::EqualityFormula::EqualityFormula(std::vector<Term> &&terms) :
    Formula(Formula::insert(EqualityFormulaPrivate(terms)))
{
}

FormulaEnvironment::EqualityFormula::EqualityFormula(const Term &term1, const Term &term2) :
    Formula(Formula::insert(EqualityFormulaPrivate(term1, term2)))
{
}

FormulaEnvironment::NegationFormula::NegationFormula(const Formula &formula) :
    Formula(Formula::insert(NegationFormulaPrivate(formula)))
{
}

FormulaEnvironment::ConjunctionFormula::ConjunctionFormula(const std::vector<Formula> &formulas) :
    Formula(Formula::insert(ConjunctionFormulaPrivate(formulas)))
{
}

FormulaEnvironment::ConjunctionFormula::ConjunctionFormula(std::vector<Formula> &&formulas) :
    Formula(Formula::insert(ConjunctionFormulaPrivate(formulas)))
{
}

FormulaEnvironment::ConjunctionFormula::ConjunctionFormula(const Formula &formula1, const Formula &formula2) :
    Formula(Formula::insert(ConjunctionFormulaPrivate(formula1, formula2)))
{
}

FormulaEnvironment::DisjunctionFormula::DisjunctionFormula(const std::vector<Formula> &formulas) :
    Formula(Formula::insert(DisjunctionFormulaPrivate(formulas)))
{
}

FormulaEnvironment::DisjunctionFormula::DisjunctionFormula(std::vector<Formula> &&formulas) :
    Formula(Formula::insert(DisjunctionFormulaPrivate(formulas)))
{
}

FormulaEnvironment::DisjunctionFormula::DisjunctionFormula(const Formula &formula1, const Formula &formula2) :
    Formula(Formula::insert(DisjunctionFormulaPrivate(formula1, formula2)))
{
}

FormulaEnvironment::ImplicationFormula::ImplicationFormula(const std::vector<Formula> &formulas) :
    Formula(Formula::insert(ImplicationFormulaPrivate(formulas)))
{
}

FormulaEnvironment::ImplicationFormula::ImplicationFormula(std::vector<Formula> &&formulas) :
    Formula(Formula::insert(ImplicationFormulaPrivate(formulas)))
{
}

FormulaEnvironment::ImplicationFormula::ImplicationFormula(const Formula &formula1, const Formula &formula2) :
    Formula(Formula::insert(ImplicationFormulaPrivate(formula1, formula2)))
{
}

FormulaEnvironment::EquivalenceFormula::EquivalenceFormula(const std::vector<Formula> &formulas) :
    Formula(Formula::insert(EquivalenceFormulaPrivate(formulas)))
{
}

FormulaEnvironment::EquivalenceFormula::EquivalenceFormula(std::vector<Formula> &&formulas) :
    Formula(Formula::insert(EquivalenceFormulaPrivate(formulas)))
{
}

FormulaEnvironment::EquivalenceFormula::EquivalenceFormula(const Formula &formula1, const Formula &formula2) :
    Formula(Formula::insert(EquivalenceFormulaPrivate(formula1, formula2)))
{
}

FormulaEnvironment::UniversalFormula::UniversalFormula(const Formula &formula, const Variable &variable) :
    Formula(Formula::insert(UniversalFormulaPrivate(formula, variable)))
{
}

FormulaEnvironment::UniversalFormula::UniversalFormula(const Formula &formula, const std::vector<Variable> &variables) :
    Formula(Formula::insert(UniversalFormulaPrivate(formula, variables)))
{
}

FormulaEnvironment::UniversalFormula::UniversalFormula(const Formula &formula, std::vector<Variable> &&variables) :
    Formula(Formula::insert(UniversalFormulaPrivate(formula, variables)))
{
}

FormulaEnvironment::ExistentialFormula::ExistentialFormula(const Formula &formula, const Variable &variable) :
    Formula(Formula::insert(ExistentialFormulaPrivate(formula, variable)))
{
}

FormulaEnvironment::ExistentialFormula::ExistentialFormula(const Formula &formula, const std::vector<Variable> &variables) :
    Formula(Formula::insert(ExistentialFormulaPrivate(formula, variables)))
{
}

FormulaEnvironment::ExistentialFormula::ExistentialFormula(const Formula &formula, std::vector<Variable> &&variables) :
    Formula(Formula::insert(ExistentialFormulaPrivate(formula, variables)))
{
}

#endif // LANGUAGE_IMP_H
