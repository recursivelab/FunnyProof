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
    if (freeVariables == false) {
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

            std::map<Variable, Term> update;

            update.insert(std::pair<Variable, Term>(*x, *t));

            for (std::map<Variable, Term>::iterator i = result.begin(); i!=result.end(); ++i) {
                update.insert(std::pair<Variable, Term>(i->first, i->second[Substitution(*x, *t)]));
            }

            result = update;

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
    if (freeVariables == false) {
        std::shared_ptr<std::set<Variable>> freeVariables(new std::set<Variable>);

        switch (symbol.type) {
        case NONE_SYMBOL:
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
