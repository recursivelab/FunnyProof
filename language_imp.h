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

#include <algorithm>
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

FalseSymbol::FalseSymbol() :
    Symbol(FALSE_SYMBOL)
{
}

const FalseSymbol& FalseSymbol::instance()
{
    static FalseSymbol result;

    return result;
}

const FalseSymbol& falseSymbol()
{
    return FalseSymbol::instance();
}

TrueSymbol::TrueSymbol() :
    Symbol(TRUE_SYMBOL)
{
}

const TrueSymbol& TrueSymbol::instance()
{
    static TrueSymbol result;

    return result;
}

const TrueSymbol& trueSymbol()
{
    return TrueSymbol::instance();
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

NonequalitySymbol::NonequalitySymbol() :
    Symbol(NONEQUALITY)
{
}

const NonequalitySymbol& NonequalitySymbol::instance()
{
    static NonequalitySymbol result;

    return result;
}

const NonequalitySymbol& nonequality()
{
    return NonequalitySymbol::instance();
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

RelationSymbol::RelationSymbol(const Symbol &symbol) :
    Symbol(symbol)
{
    if (symbol.type != RELATION) {
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

Variable::Variable(const Symbol &other) :
    Symbol(other)
{
    if (other.type != VARIABLE) {
        throw(0);
    }
}

TermEnvironment::TermPrivate::TermPrivate(Symbol symbol) :
    symbol(symbol),
    args(),
    freeVariables()
{
}

TermEnvironment::TermPrivate::TermPrivate(Symbol symbol, const std::vector<Term> &args) :
    symbol(symbol),
    args(args),
    freeVariables()
{
    if (args.size() != symbol.arity) {
        throw(0);
    }
}

TermEnvironment::TermPrivate::TermPrivate(Symbol symbol, std::vector<Term> &&args) :
    symbol(symbol),
    args(std::move(args)),
    freeVariables()
{
    if (args.size() != symbol.arity) {
        throw(0);
    }
}

TermEnvironment::TermPrivate::TermPrivate(const TermPrivate &other) :
    symbol(other.symbol),
    args(other.args),
    freeVariables(other.freeVariables)
{
}

TermEnvironment::TermPrivate::TermPrivate(TermPrivate &&other) :
    symbol(other.symbol),
    args(std::move(other.args)),
    freeVariables(std::move(other.freeVariables))
{
}

const TermEnvironment::TermPrivate& TermEnvironment::TermPrivate::dummy()
{
    thread_local static EmptyTermPrivate result;

    return result;
}

bool TermEnvironment::TermPrivate::operator ==(const TermPrivate &other) const
{
    if (this == &other) {
        return true;
    }

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
    if (this == &other) {
        return 0;
    }

    int result = symbol.compare(other.symbol);

    if (result != 0) {
        return result;
    }

    if (args.size() < other.args.size()) {
        return -1;
    }

    if (args.size() > other.args.size()) {
        return 1;
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

size_t TermEnvironment::TermPrivate::hash() const
{
    size_t result = symbol.id;

    for (size_t i = 0; i < args.size(); ++i) {
        result ^= args[i].hash();
    }

    return result;
}

bool TermEnvironment::TermPrivate::isFreeVariable(const Variable &variable) const
{
    switch (symbol.id) {
    case NONE_SYMBOL: case FALSE_SYMBOL: case TRUE_SYMBOL:
        return false;

        break;

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
    if ((bool)freeVariables == false) {
        std::shared_ptr<std::set<Variable>> freeVariables(new std::set<Variable>);

        switch (symbol.type) {
        case NONE_SYMBOL: case FALSE_SYMBOL: case TRUE_SYMBOL: case CONSTANT:
            break;

        case VARIABLE:
            freeVariables->insert(symbol);

            break;

        case OPERATION:
            for (size_t i = 0; i < args.size(); ++i) {
                const std::set<Variable> variables = args[i].getFreeVariables();

                freeVariables->insert(variables.cbegin(), variables.cend());
            }

            break;
        default:
            throw(1);

            break;
        }

        this->freeVariables = freeVariables;
    }

    return *freeVariables;
}

TermEnvironment::EmptyTermPrivate::EmptyTermPrivate() :
    TermPrivate(Symbol::dummy())
{
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

TermEnvironment::Substitution::Substitution()
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

TermEnvironment::Substitution::Substitution(const Variable &variable, const Term &term) :
    data(([](const Variable &variable, const Term &term) -> std::map<Variable, Term> {
        std::map<Variable, Term> result;

        result.insert(std::pair<Variable, Term>(variable, term));

        return result;
    })(variable, term))
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

Substitution Substitution::operator [](const Substitution &other) const
{
    std::map<Variable, Term> result;

    for (std::map<Variable, Term>::const_iterator i = other.data.cbegin(); i!=other.data.cend(); ++i) {
        Variable x = i->first;
        Term t = i->second;
        Term r = t[*this];

        result.insert(std::pair<Variable, Term>(x, r));
    }

    for (std::map<Variable, Term>::const_iterator i = data.cbegin(); i!=data.cend(); ++i) {
        if (other.data.find(i->first)==data.end()) {
            result.insert(*i);
        }
    }

    return result;
}

TermEnvironment::Term::Term() :
    term(TermPrivate::dummy())
{
}

TermEnvironment::Term::Term(const Term &other) :
    termPtr(other.termPtr),
    term(other.term)
{
}

TermEnvironment::Term::Term(const Variable &variable) :
    termPtr(new VariableTermPrivate(variable)),
    term(*termPtr)
{
}

TermEnvironment::Term::Term(const ConstantSymbol &constantSymbol) :
    termPtr(new ConstantTermPrivate(constantSymbol)),
    term(*termPtr)
{
}

TermEnvironment::Term::Term(const OperationSymbol &operationSymbol, const std::vector<Term> &args) :
    termPtr(new OperationTermPrivate(operationSymbol, args)),
    term(*termPtr)
{
}

TermEnvironment::Term::Term(const OperationSymbol &operationSymbol, std::vector<Term> &&args) :
    termPtr(new OperationTermPrivate(operationSymbol, args)),
    term(*termPtr)
{
}

bool TermEnvironment::Term::operator ==(const Term &other) const
{
    return term == other.term;
}

bool TermEnvironment::Term::operator !=(const Term &other) const
{
    return operator ==(other) == false;
}

int TermEnvironment::Term::compare(const Term &other) const
{
    return term.compare(other.term);
}

bool TermEnvironment::Term::operator <(const Term &other) const
{
    return term.compare(other.term) < 0;
}

size_t TermEnvironment::Term::hash() const
{
    return term.hash();
}

SymbolType TermEnvironment::Term::type() const
{
    return term.symbol.type;
}

uint64_t TermEnvironment::Term::id() const
{
    return term.symbol.id;
}

const Symbol& TermEnvironment::Term::symbol() const
{
    return term.symbol;
}

size_t TermEnvironment::Term::arity() const
{
    return term.symbol.arity;
}

const std::vector<TermEnvironment::Term>& TermEnvironment::Term::args() const
{
    return term.args;
}

bool TermEnvironment::Term::isFreeVariable(const Variable &variable) const
{
    return term.isFreeVariable(variable);
}

const std::set<Variable>& TermEnvironment::Term::getFreeVariables() const
{
    return term.getFreeVariables();
}

bool TermEnvironment::Term::isEmpty() const
{
    return false;
}

TermEnvironment::Term TermEnvironment::Term::operator [](const Substitution &valuation) const
{
    if (isEmpty()) {
        return *this;
    }

    switch (term.symbol.type) {
    case NONE_SYMBOL: case FALSE_SYMBOL: case TRUE_SYMBOL: case CONSTANT:
        return *this;

        break;

    case VARIABLE:
        return valuation(term.symbol);

        break;

    case OPERATION:
        {
            std::vector<Term> args;

            for (size_t i = 0; i < term.args.size(); ++i) {
                args.push_back(term.args[i][valuation]);
            }

            return Term(term.symbol, std::move(args));

            break;
        }
    default:
        throw(0);

        break;
    }
}

const TermEnvironment::Term& TermEnvironment::Term::dummy()
{
    thread_local static Term result;

    return result;
}

std::vector<Variable> TermEnvironment::oneVariable(const Variable &variable)
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

std::vector<TermEnvironment::Term> TermEnvironment::twoTerms(const TermEnvironment::Term &term1, const TermEnvironment::Term &term2)
{
    std::vector<Term> result;

    result.push_back(term1);
    result.push_back(term2);

    return result;
}

Substitution TermEnvironment::unificator(std::vector<std::pair<Term, Term>> &conditions, bool &ok)
{
    std::map<Variable, Term> result;

    while (conditions.empty()==false) {
        const std::pair<Term, Term> &p = conditions[conditions.size()-1];
        const Term u = p.first;
        const Term v = p.second;

        if (u==v) {
            conditions.pop_back();

            continue;
        }

        if (u.type()==VARIABLE || v.type()==VARIABLE) {
            const Variable *x;
            const Term *t;

            if (u.type()==VARIABLE) {
                const Variable var(u.symbol());

                x = &var;
                t = &v;
            } else {
                const Variable var(v.symbol());

                x = &var;
                t = &u;
            }

            if (t->isFreeVariable(*x)) {
                ok = false;
                result.clear();

                return result;
            }

//            std::map<Variable, Term> update;

//            update.insert(std::pair<Variable, Term>(*x, *t));

//            for (std::map<Variable, Term>::iterator i = result.begin(); i!=result.end(); ++i) {
//                update.insert(std::pair<Variable, Term>(i->first, i->second[Substitution(*x, *t)]));
//            }

//            result = update;
            result = Substitution(*x, *t)[result].data;

            continue;
        }

        if ((u.type()==CONSTANT || v.type()==CONSTANT) || (u.symbol()!=v.symbol())) {
            ok = false;
            result.clear();

            return result;
        }

        conditions.pop_back();

        size_t size = u.arity();

        if (size!=v.arity() || size!=u.args().size() || size!=v.args().size()) {
            throw(1);
        }

        for (size_t i = 0; i<size; ++i) {
            conditions.push_back(std::pair<Term, Term>(u.args()[i], v.args()[i]));
        }
    }

    ok = true;

    return result;
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate() :
    symbol(Symbol::dummy())
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol) :
    symbol(symbol)
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const std::vector<Term> &terms) :
    symbol(symbol),
    terms(terms),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, std::vector<Term> &&terms) :
    symbol(symbol),
    terms(terms),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const std::vector<Formula> &formulas) :
    symbol(symbol),
    formulas(formulas),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, std::vector<Formula> &&formulas) :
    symbol(symbol),
    formulas(formulas),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const Formula &formula, const std::vector<Variable> &variables) :
    symbol(symbol),
    formulas(oneFormula(formula)),
    variables(variables),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const Formula &formula, std::vector<Variable> &&variables) :
    symbol(symbol),
    formulas(oneFormula(formula)),
    variables(variables),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, const std::vector<Term> &terms, std::vector<Formula> &&formulas) :
    symbol(symbol),
    terms(terms),
    formulas(formulas),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, std::vector<Term> &&terms, const std::vector<Formula> &formulas) :
    symbol(symbol),
    terms(terms),
    formulas(formulas),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const Symbol &symbol, std::vector<Term> &&terms, std::vector<Formula> &&formulas) :
    symbol(symbol),
    terms(terms),
    formulas(formulas),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(const FormulaPrivate &other) :
    symbol(other.symbol),
    terms(other.terms),
    formulas(other.formulas),
    freeVariables()
{
}

FormulaEnvironment::FormulaPrivate::FormulaPrivate(FormulaPrivate &&other) :
    symbol(other.symbol),
    terms(std::move(other.terms)),
    formulas(std::move(other.formulas)),
    freeVariables(std::move(other.freeVariables))
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
    case NONE_SYMBOL:
        return false;

        break;

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
    if ((bool)freeVariables == false) {
        std::shared_ptr<std::set<Variable>> freeVariables(new std::set<Variable>);

        switch (symbol.type) {
        case NONE_SYMBOL:
        case FALSE_SYMBOL:
        case TRUE_SYMBOL:
            break;

        case EQUALITY:
        case RELATION:
            for (size_t i = 0; i < terms.size(); ++i) {
                const std::set<Variable> &variables = terms[i].getFreeVariables();

                freeVariables->insert(variables.begin(), variables.end());
            }

            break;

        case NEGATION:
        case CONJUNCTION:
        case DISJUNCTION:
        case IMPLICATION:
        case EQUIVALENCE:
            for (size_t i = 0; i < formulas.size(); ++i) {
                const std::set<Variable> &variables = formulas[i].getFreeVariables();

                freeVariables->insert(variables.begin(), variables.end());
            }

            break;

        case UNIVERSAL:
        case EXISTENTIAL:
            for (size_t i = 0; i < formulas.size(); ++i) {
                const std::set<Variable> &variables = formulas[i].getFreeVariables();

                freeVariables->insert(variables.begin(), variables.end());
            }

            for (size_t i = 0; i < variables.size(); ++i) {
                freeVariables->erase(variables[i]);
            }

            break;

        default:
            throw(1);

            break;
        }

        this->freeVariables = freeVariables;
    }

    return *freeVariables;
}

const FormulaEnvironment::FormulaPrivate& FormulaEnvironment::FormulaPrivate::dummy()
{
    static const EmptyFormulaPrivate result;

    return result;
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

FormulaEnvironment::EmptyFormulaPrivate::EmptyFormulaPrivate()
{
}

FormulaEnvironment::FalseFormulaPrivate::FalseFormulaPrivate() :
    FormulaPrivate(falseSymbol())
{
}

FormulaEnvironment::TrueFormulaPrivate::TrueFormulaPrivate() :
    FormulaPrivate(trueSymbol())
{
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

FormulaEnvironment::NonequalityFormulaPrivate::NonequalityFormulaPrivate(const std::vector<Term> &terms) :
    FormulaPrivate(nonequality(), terms, std::vector<Formula>())
{
}

FormulaEnvironment::NonequalityFormulaPrivate::NonequalityFormulaPrivate(std::vector<Term> &&terms) :
    FormulaPrivate(nonequality(), terms, std::vector<Formula>())
{
}

FormulaEnvironment::NonequalityFormulaPrivate::NonequalityFormulaPrivate(const Term &term1, const Term &term2) :
    NonequalityFormulaPrivate(TermEnvironment::twoTerms(term1, term2))
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

FormulaEnvironment::Formula::Formula(FormulaPrivate *formulaPtr) :
    formulaPtr(formulaPtr),
    formula(*formulaPtr)
{
}

FormulaEnvironment::Formula::Formula() :
    formula(FormulaPrivate::dummy())
{
}

FormulaEnvironment::Formula::Formula(const Formula &other) :
    formulaPtr(other.formulaPtr),
    formula(other.formula)
{
}

bool FormulaEnvironment::Formula::operator ==(const Formula &other) const
{
    return formula == other.formula;
}

bool FormulaEnvironment::Formula::operator !=(const Formula &other) const
{
    return operator ==(other) == false;
}

int FormulaEnvironment::Formula::compare(const Formula &other) const
{
    return formula.compare(other.formula);
}

bool FormulaEnvironment::Formula::operator <(const Formula &other) const
{
    return compare(other) < 0;
}

const Symbol& FormulaEnvironment::Formula::symbol() const
{
    return formula.symbol;
}

SymbolType FormulaEnvironment::Formula::type() const
{
    return formula.symbol.type;
}

uint64_t FormulaEnvironment::Formula::id() const
{
    return formula.symbol.id;
}

const std::vector<Term>& FormulaEnvironment::Formula::terms() const
{
    return formula.terms;
}

const std::vector<Formula>& FormulaEnvironment::Formula::formulas() const
{
    return formula.formulas;
}

const std::vector<Variable>& FormulaEnvironment::Formula::variables() const
{
    return formula.variables;
}

bool FormulaEnvironment::Formula::isFreeVariable(const Variable &variable) const
{
    return formula.isFreeVariable(variable);
}

const std::set<Variable>& FormulaEnvironment::Formula::getFreeVariables() const
{
    return formula.getFreeVariables();
}

bool FormulaEnvironment::Formula::isEmpty() const
{
    return formula.symbol.type == NONE_SYMBOL;
}

FormulaEnvironment::Formula FormulaEnvironment::Formula::operator [](const TermEnvironment::Substitution &substitution) const
{
    std::map<Variable, Term> data;

    switch (formula.symbol.type) {
    case NONE_SYMBOL:
        return *this;

        break;

    case RELATION:
    {
        const std::vector<Term> &terms = formula.terms;
        std::vector<Term> result;

        for (size_t i = 0; i < terms.size(); ++i) {
            result.push_back(terms[i][substitution]);
        }

        return RelationFormula(formula.symbol, std::move(result));
    }

        break;

    case EQUALITY:
    {
        const std::vector<Term> &terms = formula.terms;
        std::vector<Term> result;

        for (size_t i = 0; i < terms.size(); ++i) {
            result.push_back(terms[i][substitution]);
        }

        return EqualityFormula(std::move(result));
    }

        break;

    case NEGATION:
        return NegationFormula(formula.formulas[0][substitution]);

        break;

    case CONJUNCTION:
    {
        const std::vector<Formula> &formulas = formula.formulas;
        std::vector<Formula> result;

        for (size_t i = 0; i < formulas.size(); ++i) {
            result.push_back(formulas[i][substitution]);
        }

        return ConjunctionFormula(std::move(result));
    }

        break;

    case DISJUNCTION:
    {
        const std::vector<Formula> &formulas = formula.formulas;
        std::vector<Formula> result;

        for (size_t i = 0; i < formulas.size(); ++i) {
            result.push_back(formulas[i][substitution]);
        }

        return DisjunctionFormula(std::move(result));
    }

        break;

    case IMPLICATION:
    {
        const std::vector<Formula> &formulas = formula.formulas;
        std::vector<Formula> result;

        for (size_t i = 0; i < formulas.size(); ++i) {
            result.push_back(formulas[i][substitution]);
        }

        return ImplicationFormula(std::move(result));
    }

        break;

    case EQUIVALENCE:
    {
        const std::vector<Formula> &formulas = formula.formulas;
        std::vector<Formula> result;

        for (size_t i = 0; i < formulas.size(); ++i) {
            result.push_back(formulas[i][substitution]);
        }

        return EquivalenceFormula(std::move(result));
    }

        break;

    case UNIVERSAL: case EXISTENTIAL:
    {
        const std::set<Variable> &fv = getFreeVariables();
        const std::vector<Variable> &v = variables();
        std::vector<Variable> qs;
        std::set<Variable> tfv;
        std::set<Variable> bounded;

        for (size_t i = 0; i < v.size(); ++i) {
            bounded.insert(v[i]);
        }

        for (std::map<Variable, Term>::const_iterator i = substitution.data.cbegin(); i != substitution.data.cend(); ++i) {
            Variable x = i->first;

            if (fv.count(x)) {
                const std::set<Variable> &tv = (i->second).getFreeVariables();

                tfv.insert(tv.cbegin(), tv.cend());
            }
        }

        for (std::set<Variable>::const_iterator i = bounded.cbegin(); i != bounded.cend(); ++i) {
            if (tfv.count(*i)) {
                Variable y;
                Term t(y);

                data.insert(std::pair<Variable, Term>(*i, t));
                qs.push_back(y);
            } else {
                data.insert(std::pair<Variable, Term>(*i, substitution(*i)));
                qs.push_back(*i);
            }
        }

        if (type() == UNIVERSAL) {
            return UniversalFormula(formulas()[0][data], std::move(qs));
        }

        return ExistentialFormula(formulas()[0][data], std::move(qs));
    }

        break;

    default:
        throw(1);

        break;
    }

    throw(0);
}

const FormulaEnvironment::Formula& FormulaEnvironment::Formula::dummy()
{
    static Formula result;

    return result;
}

FormulaEnvironment::Formula FormulaEnvironment::Formula::simplify() const
{
    switch (type()) {
    case EQUALITY:
    case NONEQUALITY:
    {
        std::set<Term> args;
        std::vector<Term> result;

        for (size_t i = 0; i<terms().size(); ++i) {
            Term t = terms()[i];

            if (t.type()==NONE_SYMBOL) {
                return EmptyFormula();
            }

            args.insert(t);
            result.push_back(t);
        }

        if (result.size()<=1U) {
            return TrueFormula();
        }

        if (result.size()!=args.size()) {
            if (type()==EQUALITY) {
                result.clear();

                for (auto i = args.cbegin(); i!=args.cend(); ++i) {
                    result.push_back(*i);
                }

                return EqualityFormula(result).simplify();
            }

            return FalseFormula();
        }
    }

        break;

    case NEGATION:
    {
        Formula arg = formulas()[0].simplify();

        switch (arg.type()) {
        case FALSE_SYMBOL:
            return TrueFormula();

            break;

        case TRUE_SYMBOL:
            return FalseFormula();

            break;

        case EQUALITY:
            if (arg.variables().size()==2) {
                return NonequalityFormula(arg.terms());
            }

            break;

        case NONEQUALITY:
            if (arg.variables().size()==2) {
                return EqualityFormula(arg.terms());
            }

            break;

        case NEGATION:
            return arg.formulas()[0];

            break;

        case CONJUNCTION:
        case DISJUNCTION:
        {
            const std::vector<Formula> &formulas = arg.formulas();
            std::vector<Formula> args;

            for (auto i = formulas.cbegin(); i!=formulas.cend(); ++i) {
                args.push_back(NegationFormula(*i).simplify());
            }

            if (arg.type()==CONJUNCTION) {
                return FormulaEnvironment::DisjunctionFormula(args);
            }

            return FormulaEnvironment::ConjunctionFormula(args);
        }

            break;

        case IMPLICATION:
            if (arg.formulas().size()==2) {
                Formula f1 = arg.formulas()[0].simplify();
                Formula f2 = arg.formulas()[1].simplify();

                return ConjunctionFormula(f1, NegationFormula(f2).simplify());
            }

            break;

            if (arg.formulas().size()==2) {
                Formula f1 = arg.formulas()[0].simplify();
                Formula f2 = arg.formulas()[1].simplify();

                if (f1.type()==NEGATION) {
                    return ConjunctionFormula(NegationFormula(f1).simplify(), f2);
                }

                if (f2.type()==NEGATION) {
                    return ConjunctionFormula(f1, NegationFormula(f2).simplify());
                }
            }

            break;

        case UNIVERSAL:
        case EXISTENTIAL:
        {
            Formula f = FormulaEnvironment::NegationFormula(arg.formulas()[0]).simplify();

            if (arg.type()==UNIVERSAL) {
                return FormulaEnvironment::ExistentialFormula(f, arg.variables());
            }

            return FormulaEnvironment::UniversalFormula(f, arg.variables());
        }

        default:
            break;
        }

        return NegationFormula(arg);

        break;
    }

    case CONJUNCTION:
    case DISJUNCTION:
    {
        std::set<Formula> args;
        std::vector<Formula> result;

        for (auto i = formulas().cbegin(); i!=formulas().cend(); ++i) {
            Formula f = i->simplify();

            if (f.type()==type()) {
                for (auto j = f.formulas().cbegin(); j!=f.formulas().cend(); ++j) {
                    if (args.count(*j)==0) {
                        args.insert(*j);
                    }
                }
            } else if (f.type()==TRUE_SYMBOL || f.type()==FALSE_SYMBOL) {
                if ((type()==CONJUNCTION)!=(f.type()==TRUE_SYMBOL)) {
                    return f;
                }
            } else if (args.count(f)==0) {
                args.insert(f);
            }
        }

        if (args.empty()) {
            if (type()==CONJUNCTION) {
                return TrueFormula();
            }

            return FalseFormula();
        }

        for (auto i = args.cbegin(); i!=args.cend(); ++i) {
            result.push_back(*i);
        }

        if (result.size()==1) {
            return result[0];
        }

        if (type()==CONJUNCTION) {
// TO DO: (A imp B) and (B imp C) -> A imp B imp C.
            return ConjunctionFormula(result);
        }

        return DisjunctionFormula(result);
    }

        break;

    case IMPLICATION:
    {
        std::vector<Formula> fs;
        std::map<Formula, size_t> indexes;
        std::vector<size_t> representers;
        bool hasFalse = false;
        bool hasTrue = false;
        size_t falsePosition;
        size_t truePosition;

        for (size_t i = 0; i<formulas().size(); ++i) {
            Formula f = formulas()[i].simplify();

            fs.push_back(f);

            if (f.type()==NONE_SYMBOL) {
                return EmptyFormula();
            } else if (f.type()==FALSE_SYMBOL) {
                hasFalse = true;
                falsePosition = i;
            } else if (hasTrue==false && f.type()==TRUE_SYMBOL) {
                hasTrue = true;
                truePosition = i;
            }
        }

        if (hasFalse && hasTrue && truePosition<falsePosition) {
            return FalseFormula();
        }

        std::vector<Formula> result;
        size_t begin = hasFalse ? falsePosition+1 : 0;
        size_t end = hasTrue ? truePosition : fs.size();

        if (hasTrue) {
            for (size_t i = truePosition+1; i<fs.size(); ++i) {
                if (fs[i].type()!=TRUE_SYMBOL) {
                    result.push_back(fs[i]);
                }
            }
        }

        if (hasFalse) {
            for (size_t i = 0; i<falsePosition; ++i) {
                if (fs[i].type()!=FALSE_SYMBOL) {
                    result.push_back(NegationFormula(fs[i]).simplify());
                }
            }
        }

        for (size_t i = begin; i<end; ++i) {
            Formula f = fs[i];
            auto j = indexes.find(f);

            if (j==indexes.cend()) {
                indexes.insert(std::pair<Formula, size_t>(f, i));
                representers.push_back(i);
            } else {
                size_t k = j->second;
                size_t value = representers[k-begin];

                for (size_t l = value+1-begin; l<representers.size(); ++l) {
                    representers[l] = value;
                }

                representers.push_back(value);
            }
        }

        std::map<size_t, std::set<Formula>> groups;

        for (size_t i = begin; i<end; ++i) {
            if (representers[i-begin]==i) {
                std::set<Formula> s;

                s.insert(fs[i]);
                groups.insert(std::pair<size_t, std::set<Formula>>(i, s));
            } else {
                groups[representers[i-begin]].insert(fs[i]);
            }
        }

        std::vector<Formula> equivalences;

        for (auto i = groups.cbegin(); i!=groups.cend(); ++i) {
            const std::set<Formula> &g = i->second;
            std::vector<Formula> args;

            for (auto j = g.cbegin(); j!=g.cend(); ++j) {
                args.push_back(*j);
            }

            if (args.size()==1U) {
                equivalences.push_back(args[0]);
            } else {
                equivalences.push_back(EquivalenceFormula(args));
            }
        }

        if (equivalences.size()==formulas().size()) {
            return ImplicationFormula(equivalences);
        }

        if (equivalences.size()==1) {
            result.push_back(TrueFormula());
        } else if (equivalences.size()>1) {
            result.push_back(ImplicationFormula(equivalences));
        }

        return ConjunctionFormula(result);
    }

        break;

    case EQUIVALENCE:
    {
        std::set<Formula> args;
        std::vector<Formula> result;
        bool hasFalse = false;
        bool hasTrue = false;

        for (size_t i = 0; i<formulas().size(); ++i) {
            Formula f = formulas()[i].simplify();

            if (f.type()==NONE_SYMBOL) {
                return EmptyFormula();
            } else if (f.type()==FALSE_SYMBOL) {
                hasFalse = true;

                continue;
            } else if (f.type()==TRUE_SYMBOL) {
                hasTrue = true;

                continue;
            }

            args.insert(f);
        }

        if (hasFalse && hasTrue) {
            return FalseFormula();
        }

        if (args.empty()) {
            return TrueFormula();
        }

        for (auto i = args.cbegin(); i!=args.end(); ++i) {
            result.push_back(*i);
        }

        if (result.size()==1) {
            return TrueFormula();
        }

        if (hasFalse) {
            std::vector<Formula> negatedFormulas;

            for (size_t i = 0; i<result.size(); ++i) {
                negatedFormulas.push_back(NegationFormula(result[i]));
            }

            return ConjunctionFormula(negatedFormulas).simplify();
        }

        if (hasTrue) {
            return ConjunctionFormula(result).simplify();
        }

        return EquivalenceFormula(result);
    }

        break;

    case UNIVERSAL:
    case EXISTENTIAL:
    {
        Formula f = formulas()[0].simplify();
        std::set<Variable> freeVars = f.getFreeVariables();
        std::set<Variable> vars;
        std::vector<Variable> result;

        for (auto i = variables().cbegin(); i!=variables().cend(); ++i) {
            auto j = freeVars.find(*i);

            if (j!=freeVars.cend()) {
                vars.insert(*i);
                freeVars.erase(j);
            }
        }

        const Formula *subformula = &f;

        if (f.type()==type()) {
            for (auto i = f.variables().cbegin(); i!=f.variables().cend(); ++i) {
                vars.insert(*i);
            }

            subformula = &(f.formulas()[0]);
        }

        if (vars.empty()) {
            return *subformula;
        }

        for (auto i = vars.cbegin(); i!=vars.cend(); ++i) {
            result.push_back(*i);
        }

        if (type()==UNIVERSAL) {
            return UniversalFormula(*subformula, result);
        }

        return ExistentialFormula(*subformula, result);
    }

        break;

    default:
        break;
    }

    return *this;
}

FormulaEnvironment::EmptyFormula::EmptyFormula()
{
}

FormulaEnvironment::FalseFormula::FalseFormula() :
    Formula(new FalseFormulaPrivate)
{
}

FormulaEnvironment::TrueFormula::TrueFormula() :
    Formula(new TrueFormulaPrivate)
{
}

FormulaEnvironment::RelationFormula::RelationFormula(const RelationSymbol &symbol, const std::vector<Term> &terms) :
    Formula(new RelationFormulaPrivate(symbol, terms))
{
}

FormulaEnvironment::RelationFormula::RelationFormula(const RelationSymbol &symbol, std::vector<Term> &&terms) :
    Formula(new RelationFormulaPrivate(symbol, terms))
{
}

FormulaEnvironment::EqualityFormula::EqualityFormula(const std::vector<Term> &terms) :
    Formula(new EqualityFormulaPrivate(terms))
{
}

FormulaEnvironment::EqualityFormula::EqualityFormula(std::vector<Term> &&terms) :
    Formula(new EqualityFormulaPrivate(terms))
{
}

FormulaEnvironment::EqualityFormula::EqualityFormula(const Term &term1, const Term &term2) :
    Formula(new EqualityFormulaPrivate(term1, term2))
{
}

FormulaEnvironment::NonequalityFormula::NonequalityFormula(const std::vector<Term> &terms) :
    Formula(new NonequalityFormulaPrivate(terms))
{
}

FormulaEnvironment::NonequalityFormula::NonequalityFormula(std::vector<Term> &&terms) :
    Formula(new NonequalityFormulaPrivate(terms))
{
}

FormulaEnvironment::NonequalityFormula::NonequalityFormula(const Term &term1, const Term &term2) :
    Formula(new NonequalityFormulaPrivate(term1, term2))
{
}

FormulaEnvironment::NegationFormula::NegationFormula(const Formula &formula) :
    Formula(new NegationFormulaPrivate(formula))
{
}

FormulaEnvironment::ConjunctionFormula::ConjunctionFormula(const std::vector<Formula> &formulas) :
    Formula(new ConjunctionFormulaPrivate(formulas))
{
}

FormulaEnvironment::ConjunctionFormula::ConjunctionFormula(std::vector<Formula> &&formulas) :
    Formula(new ConjunctionFormulaPrivate(formulas))
{
}

FormulaEnvironment::ConjunctionFormula::ConjunctionFormula(const Formula &formula1, const Formula &formula2) :
    Formula(new ConjunctionFormulaPrivate(formula1, formula2))
{
}

FormulaEnvironment::DisjunctionFormula::DisjunctionFormula(const std::vector<Formula> &formulas) :
    Formula(new DisjunctionFormulaPrivate(formulas))
{
}

FormulaEnvironment::DisjunctionFormula::DisjunctionFormula(std::vector<Formula> &&formulas) :
    Formula(new DisjunctionFormulaPrivate(formulas))
{
}

FormulaEnvironment::DisjunctionFormula::DisjunctionFormula(const Formula &formula1, const Formula &formula2) :
    Formula(new DisjunctionFormulaPrivate(formula1, formula2))
{
}

FormulaEnvironment::ImplicationFormula::ImplicationFormula(const std::vector<Formula> &formulas) :
    Formula(new ImplicationFormulaPrivate(formulas))
{
}

FormulaEnvironment::ImplicationFormula::ImplicationFormula(std::vector<Formula> &&formulas) :
    Formula(new ImplicationFormulaPrivate(formulas))
{
}

FormulaEnvironment::ImplicationFormula::ImplicationFormula(const Formula &formula1, const Formula &formula2) :
    Formula(new ImplicationFormulaPrivate(formula1, formula2))
{
}

FormulaEnvironment::EquivalenceFormula::EquivalenceFormula(const std::vector<Formula> &formulas) :
    Formula(new EquivalenceFormulaPrivate(formulas))
{
}

FormulaEnvironment::EquivalenceFormula::EquivalenceFormula(std::vector<Formula> &&formulas) :
    Formula(new EquivalenceFormulaPrivate(formulas))
{
}

FormulaEnvironment::EquivalenceFormula::EquivalenceFormula(const Formula &formula1, const Formula &formula2) :
    Formula(new EquivalenceFormulaPrivate(formula1, formula2))
{
}

FormulaEnvironment::UniversalFormula::UniversalFormula(const Formula &formula, const Variable &variable) :
    Formula(new UniversalFormulaPrivate(formula, variable))
{
}

FormulaEnvironment::UniversalFormula::UniversalFormula(const Formula &formula, const std::vector<Variable> &variables) :
    Formula(new UniversalFormulaPrivate(formula, variables))
{
}

FormulaEnvironment::UniversalFormula::UniversalFormula(const Formula &formula, std::vector<Variable> &&variables) :
    Formula(new UniversalFormulaPrivate(formula, variables))
{
}

FormulaEnvironment::ExistentialFormula::ExistentialFormula(const Formula &formula, const Variable &variable) :
    Formula(new ExistentialFormulaPrivate(formula, variable))
{
}

FormulaEnvironment::ExistentialFormula::ExistentialFormula(const Formula &formula, const std::vector<Variable> &variables) :
    Formula(new ExistentialFormulaPrivate(formula, variables))
{
}

FormulaEnvironment::ExistentialFormula::ExistentialFormula(const Formula &formula, std::vector<Variable> &&variables) :
    Formula(new ExistentialFormulaPrivate(formula, variables))
{
}

#endif // LANGUAGE_IMP_H
