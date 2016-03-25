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

// Author : Nedeljko Stefanovic

// Implementation of constant, operation and relation symbols, variables,
// terms and formulas.
//
// These logical objects are implemented as immutable
// objects.
//
// All program objects representing the same logical object pointing the same
// internal representation even if these program objects are created independent
// (without copy constructor). An pointed object is automaticaly deleted when
// no objets pointing it.
//
// Any symbol object have it's own id. Symbol object constructed without copy
// constructor representing a new symbol with new id.


#ifndef LANGUAGE_H
#define LANGUAGE_H


#include <atomic>
#include <inttypes.h>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include "config.h"


enum SymbolType
{
    NONE_SYMBOL,
    FALSE_SYMBOL,
    TRUE_SYMBOL,
    VARIABLE,
    CONSTANT,
    OPERATION,
    RELATION,
    EQUALITY,
    NEGATION,
    CONJUNCTION,
    DISJUNCTION,
    IMPLICATION,
    EQUIVALENCE,
    UNIVERSAL,
    EXISTENTIAL
};

class Symbol
{
    DECLARE Symbol();

public:
    const SymbolType type;
    const uint64_t id;
    const std::size_t arity;

    DECLARE static const Symbol& dummy();

protected:
    DECLARE Symbol(SymbolType type, std::size_t arity = 0);

public:
    DECLARE Symbol(const Symbol &other);
    DECLARE bool operator ==(const Symbol &other) const;
    DECLARE bool operator !=(const Symbol &other) const;
    DECLARE bool operator <(const Symbol &other) const;
    DECLARE int compare(const Symbol &other) const;
};

class FalseSymbol : public Symbol
{
    DECLARE FalseSymbol();

public:
    DECLARE static const FalseSymbol& instance();
};

DECLARE const FalseSymbol& falseSymbol();

class TrueSymbol : public Symbol
{
    DECLARE TrueSymbol();

public:
    DECLARE static const TrueSymbol& instance();
};

DECLARE const TrueSymbol& trueSymbol();

class NegationSymbol : public Symbol
{
    DECLARE NegationSymbol();

public:
    DECLARE static const NegationSymbol& instance();
};

DECLARE const NegationSymbol& negation();

class ConjunctionSymbol : public Symbol
{
    DECLARE ConjunctionSymbol();

public:
    DECLARE static const ConjunctionSymbol& instance();
};

DECLARE const ConjunctionSymbol& conjunction();

class DisjunctionSymbol : public Symbol
{
    DECLARE DisjunctionSymbol();

public:
    DECLARE static const DisjunctionSymbol& instance();
};

DECLARE const DisjunctionSymbol& disjunction();

class ImplicationSymbol : public Symbol
{
    DECLARE ImplicationSymbol();

public:
    DECLARE static const ImplicationSymbol& instance();
};

DECLARE const ImplicationSymbol& implication();

class EquivalenceSymbol : public Symbol
{
    DECLARE EquivalenceSymbol();

public:
    DECLARE static const EquivalenceSymbol& instance();
};

DECLARE const EquivalenceSymbol& equivalence();

class UniversalSymbol : public Symbol
{
    DECLARE UniversalSymbol();

public:
    DECLARE static const UniversalSymbol& instance();
};

DECLARE const UniversalSymbol& universal();

class ExistentialSymbol : public Symbol
{
    DECLARE ExistentialSymbol();

public:
    DECLARE static const ExistentialSymbol& instance();
};

DECLARE const ExistentialSymbol& existential();

class EqualitySymbol : public Symbol
{
    DECLARE EqualitySymbol();

public:
    DECLARE static const EqualitySymbol& instance();
};

DECLARE const EqualitySymbol& equality();

class OperationSymbol : public Symbol
{
public:
    DECLARE OperationSymbol(std::size_t arity);
    DECLARE OperationSymbol(const OperationSymbol &other);
    DECLARE OperationSymbol(const Symbol &symbol);
};

class ConstantSymbol : public Symbol
{
public:
    DECLARE ConstantSymbol();
    DECLARE ConstantSymbol(const ConstantSymbol &other);
    DECLARE ConstantSymbol(const Symbol &symbol);
};

class RelationSymbol : public Symbol
{
public:
    DECLARE RelationSymbol(std::size_t arity);
    DECLARE RelationSymbol(const RelationSymbol &other);
    DECLARE RelationSymbol(const Symbol &symbol);
};

class Variable : public Symbol
{
public:
    DECLARE Variable();
    DECLARE Variable(const Variable &other);
    DECLARE Variable(const Symbol &other);
};

class TermEnvironment
{
public:
    class Term;

private:
    class TermPrivate
    {
    public:
        const Symbol symbol;
        const std::vector<Term> args;
        mutable std::set<Variable> freeVariables;
        mutable bool freeVariablesComputed;

    protected:
        DECLARE TermPrivate(Symbol symbol);
        DECLARE TermPrivate(Symbol symbol, const std::vector<Term> &args);
        DECLARE TermPrivate(Symbol symbol, std::vector<Term> &&args);

    public:
        DECLARE TermPrivate(const TermPrivate &other);
        DECLARE TermPrivate(TermPrivate &&other);
        DECLARE static const TermPrivate& dummy();
        DECLARE bool operator ==(const TermPrivate &other) const;
        DECLARE bool operator !=(const TermPrivate &other) const;
        DECLARE int compare(const TermPrivate &other) const;
        DECLARE bool operator <(const TermPrivate &other) const;
        DECLARE size_t hash() const;
        DECLARE bool isFreeVariable(const Variable &variable) const;
        DECLARE const std::set<Variable>& getFreeVariables() const;
    };

    struct EmptyTermPrivate : public TermPrivate
    {
        DECLARE EmptyTermPrivate();
    };

    struct VariableTermPrivate : public TermPrivate
    {
        DECLARE VariableTermPrivate(const Variable &variable);
    };

    struct ConstantTermPrivate : public TermPrivate
    {
        DECLARE ConstantTermPrivate(const ConstantSymbol &constant);
    };

    struct OperationTermPrivate : public TermPrivate
    {
        DECLARE OperationTermPrivate(const OperationSymbol &operation, const std::vector<Term> &args);
        DECLARE OperationTermPrivate(const OperationSymbol &operation, std::vector<Term> &&args);
    };

public:
    class Substitution
    {
    public:
        DECLARE Substitution(const std::map<Variable, Term> &valuation);
        DECLARE Substitution(std::map<Variable, Term> &&valuation);
        DECLARE Term operator ()(const Variable &variable) const;

        const std::map<Variable, Term> data;
    };

    class Term
    {
        const std::shared_ptr<TermPrivate> termPtr;
        const TermPrivate &term;
        Term& operator =(const Term&) = delete;

    public:
        DECLARE Term();
        DECLARE Term(const Term &other);
        DECLARE Term(const Variable &variable);
        DECLARE Term(const ConstantSymbol &constantSymbol);
        DECLARE Term(const OperationSymbol &operationSymbol, const std::vector<Term> &args);
        DECLARE Term(const OperationSymbol &operationSymbol, std::vector<Term> &&args);
        DECLARE bool operator ==(const Term &other) const;
        DECLARE bool operator !=(const Term &other) const;
        DECLARE int compare(const Term &other) const;
        DECLARE bool operator <(const Term &other) const;
        DECLARE size_t hash() const;
        DECLARE SymbolType type() const;
        DECLARE uint64_t id() const;
        DECLARE const Symbol& symbol() const;
        DECLARE size_t arity() const;
        DECLARE const std::vector<Term>& args() const;
        DECLARE bool isFreeVariable(const Variable &variable) const;
        DECLARE const std::set<Variable>& getFreeVariables() const;
        DECLARE bool isEmpty() const;
        DECLARE Term operator [](const Substitution &valuation) const;
        DECLARE static const Term& dummy();
    };

    DECLARE static std::vector<Variable> oneVariable(const Variable &variable);
    DECLARE static std::vector<Term> oneTerm(const Term &term);
    DECLARE static std::vector<Term> twoTerms(const Term &term1, const Term &term2);
};

typedef TermEnvironment::Term Term;
typedef TermEnvironment::Substitution Substitution;

class FormulaEnvironment
{
public:
    class Formula;

private:
    class FormulaPrivate
    {
    public:
        const Symbol symbol;
        const std::vector<Term> terms;
        const std::vector<Formula> formulas;
        const std::vector<Variable> variables;
        mutable std::set<Variable> freeVariables;
        mutable bool freeVariablesComputed;

        DECLARE FormulaPrivate();
        DECLARE FormulaPrivate(const Symbol &symbol);
        DECLARE FormulaPrivate(const Symbol &symbol, const std::vector<Term> &terms);
        DECLARE FormulaPrivate(const Symbol &symbol, std::vector<Term> &&terms);
        DECLARE FormulaPrivate(const Symbol &symbol, const std::vector<Formula> &formulas);
        DECLARE FormulaPrivate(const Symbol &symbol, std::vector<Formula> &&formulas);
        DECLARE FormulaPrivate(const Symbol &symbol, const Formula &formula, const std::vector<Variable> &variables);
        DECLARE FormulaPrivate(const Symbol &symbol, const Formula &formula, std::vector<Variable> &&variables);
        DECLARE FormulaPrivate(const Symbol &symbol, const std::vector<Term> &terms, const std::vector<Formula> &formulas);
        DECLARE FormulaPrivate(const Symbol &symbol, const std::vector<Term> &terms, std::vector<Formula> &&formulas);
        DECLARE FormulaPrivate(const Symbol &symbol, std::vector<Term> &&terms, const std::vector<Formula> &formulas);
        DECLARE FormulaPrivate(const Symbol &symbol, std::vector<Term> &&terms, std::vector<Formula> &&formulas);
        DECLARE FormulaPrivate(const FormulaPrivate &other);
        DECLARE FormulaPrivate(FormulaPrivate &&other);
        DECLARE bool operator ==(const FormulaPrivate &other) const;
        DECLARE bool operator !=(const FormulaPrivate &other) const;
        DECLARE int compare(const FormulaPrivate &other) const;
        DECLARE bool operator <(const FormulaPrivate &other) const;
        DECLARE bool isFreeVariable(const Variable &variable) const;
        DECLARE const std::set<Variable>& getFreeVariables() const;
        DECLARE static const FormulaPrivate& dummy();
    };

    DECLARE static std::vector<Formula> oneFormula(const Formula &formula);
    DECLARE static std::vector<Formula> twoFormulas(const Formula &formula1, const Formula &formula2);

    class EmptyFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE EmptyFormulaPrivate();
    };

    class FalseFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE FalseFormulaPrivate();
    };

    class TrueFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE TrueFormulaPrivate();
    };

    class EqualityFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE EqualityFormulaPrivate(const std::vector<Term> &terms);
        DECLARE EqualityFormulaPrivate(std::vector<Term> &&terms);
        DECLARE EqualityFormulaPrivate(const Term &term1, const Term &term2);
    };

    class RelationFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE RelationFormulaPrivate(const RelationSymbol &symbol, const std::vector<Term> &terms);
        DECLARE RelationFormulaPrivate(const RelationSymbol &symbol, std::vector<Term> &&terms);
    };

    class NegationFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE NegationFormulaPrivate(const Formula &formula);
    };

    class ConjunctionFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE ConjunctionFormulaPrivate(const std::vector<Formula> &formulas);
        DECLARE ConjunctionFormulaPrivate(std::vector<Formula> &&formulas);
        DECLARE ConjunctionFormulaPrivate(const Formula &formula1, const Formula &formula2);
    };

    class DisjunctionFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE DisjunctionFormulaPrivate(const std::vector<Formula> &formulas);
        DECLARE DisjunctionFormulaPrivate(std::vector<Formula> &&formulas);
        DECLARE DisjunctionFormulaPrivate(const Formula &formula1, const Formula &formula2);
    };

    class ImplicationFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE ImplicationFormulaPrivate(const std::vector<Formula> &formulas);
        DECLARE ImplicationFormulaPrivate(std::vector<Formula> &&formulas);
        DECLARE ImplicationFormulaPrivate(const Formula &formula1, const Formula &formula2);
    };

    class EquivalenceFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE EquivalenceFormulaPrivate(const std::vector<Formula> &formulas);
        DECLARE EquivalenceFormulaPrivate(std::vector<Formula> &&formulas);
        DECLARE EquivalenceFormulaPrivate(const Formula &formula1, const Formula &formula2);
    };

    class UniversalFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE UniversalFormulaPrivate(const Formula &formula, const Variable &variable);
        DECLARE UniversalFormulaPrivate(const Formula &formula, const std::vector<Variable> &variables);
        DECLARE UniversalFormulaPrivate(const Formula &formula, std::vector<Variable> &&variables);
    };

    class ExistentialFormulaPrivate : public FormulaPrivate
    {
    public:
        DECLARE ExistentialFormulaPrivate(const Formula &formula, const Variable &variable);
        DECLARE ExistentialFormulaPrivate(const Formula &formula, const std::vector<Variable> &variables);
        DECLARE ExistentialFormulaPrivate(const Formula &formula, std::vector<Variable> &&variables);
    };

public:

    struct FalseFormula;
    struct TrueFormula;
    struct RelationFormula;
    struct EqualityFormula;
    struct NegationFormula;
    struct ConjunctionFormula;
    struct DisjunctionFormula;
    struct ImplicationFormula;
    struct EquivalenceFormula;
    struct UniversalFormula;
    struct ExistentialFormula;

    class Formula
    {
        const std::shared_ptr<FormulaPrivate> formulaPtr;
        const FormulaPrivate &formula;

        Formula& operator =(const Formula&) = delete;
        DECLARE Formula(FormulaPrivate *formulaPtr);

    public:
        DECLARE Formula();
        DECLARE Formula(const Formula &other);
        DECLARE bool operator ==(const Formula &other) const;
        DECLARE bool operator !=(const Formula &other) const;
        DECLARE int compare(const Formula &other) const;
        DECLARE bool operator <(const Formula &other) const;
        DECLARE const Symbol& symbol() const;
        DECLARE SymbolType type() const;
        DECLARE uint64_t id() const;
        DECLARE const std::vector<Term>& terms() const;
        DECLARE const std::vector<Formula>& formulas() const;
        DECLARE const std::vector<Variable>& variables() const;
        DECLARE bool isFreeVariable(const Variable &variable) const;
        DECLARE const std::set<Variable>& getFreeVariables() const;
        DECLARE bool isEmpty() const;
        DECLARE Formula operator [](const TermEnvironment::Substitution &substitution) const;
        DECLARE static const Formula& dummy();

        friend struct FalseFormula;
        friend struct TrueFormula;
        friend struct RelationFormula;
        friend struct EqualityFormula;
        friend struct NegationFormula;
        friend struct ConjunctionFormula;
        friend struct DisjunctionFormula;
        friend struct ImplicationFormula;
        friend struct EquivalenceFormula;
        friend struct UniversalFormula;
        friend struct ExistentialFormula;
    };

    struct EmptyFormula : public Formula
    {
        DECLARE EmptyFormula();
    };

    struct FalseFormula : public Formula
    {
        DECLARE FalseFormula();
    };

    struct TrueFormula : public Formula
    {
        DECLARE TrueFormula();
    };

    struct RelationFormula : public Formula
    {
        DECLARE RelationFormula(const RelationSymbol &symbol, const std::vector<Term> &terms);
        DECLARE RelationFormula(const RelationSymbol &symbol, std::vector<Term> &&terms);
    };

    struct EqualityFormula : public Formula
    {
        DECLARE EqualityFormula(const std::vector<Term> &terms);
        DECLARE EqualityFormula(std::vector<Term> &&terms);
        DECLARE EqualityFormula(const Term &term1, const Term &term2);
    };

    struct NegationFormula : public Formula
    {
        DECLARE NegationFormula(const Formula &formula);
    };

    struct ConjunctionFormula : public Formula
    {
        DECLARE ConjunctionFormula(const std::vector<Formula> &formulas);
        DECLARE ConjunctionFormula(std::vector<Formula> &&formulas);
        DECLARE ConjunctionFormula(const Formula &formula1, const Formula &formula2);
    };

    struct DisjunctionFormula : public Formula
    {
        DECLARE DisjunctionFormula(const std::vector<Formula> &formulas);
        DECLARE DisjunctionFormula(std::vector<Formula> &&formulas);
        DECLARE DisjunctionFormula(const Formula &formula1, const Formula &formula2);
    };

    struct ImplicationFormula : public Formula
    {
        DECLARE ImplicationFormula(const std::vector<Formula> &formulas);
        DECLARE ImplicationFormula(std::vector<Formula> &&formulas);
        DECLARE ImplicationFormula(const Formula &formula1, const Formula &formula2);
    };

    struct EquivalenceFormula : public Formula
    {
        DECLARE EquivalenceFormula(const std::vector<Formula> &formulas);
        DECLARE EquivalenceFormula(std::vector<Formula> &&formulas);
        DECLARE EquivalenceFormula(const Formula &formula1, const Formula &formula2);
    };

    struct UniversalFormula : public Formula
    {
        DECLARE UniversalFormula(const Formula &formula, const Variable &variable);
        DECLARE UniversalFormula(const Formula &formula, const std::vector<Variable> &variables);
        DECLARE UniversalFormula(const Formula &formula, std::vector<Variable> &&variables);
    };

    struct ExistentialFormula : public Formula
    {
        DECLARE ExistentialFormula(const Formula &formula, const Variable &variable);
        DECLARE ExistentialFormula(const Formula &formula, const std::vector<Variable> &variables);
        DECLARE ExistentialFormula(const Formula &formula, std::vector<Variable> &&variables);
    };
};

typedef FormulaEnvironment::Formula Formula;

#ifdef INLINE

#include "language_imp.h"

#endif

#endif // LANGUAGE_H
