/*******************************************************************************
 *                                                                             *
 *    FunnyProof - Easy for use proof assistant.                               *
 *    Copyright (C) 2015  Nedeljko Stefanovic                                  *
 *                                                                             *
 *    This program is free software: you can redistribute it and/or modify     *
 *    it under the terms of version 3 of the GNU General Public License as     *
 *    published by the Free Software Foundation.                               *
 *                                                                             *
 *    This program is distributed in the hope that it will be useful,          *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *    GNU General Public License for more details.                             *
 *                                                                             *
 *    You should have received a copy of the GNU General Public License        *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                             *
 ******************************************************************************/


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


#ifndef LANGUAGE_H
#define LANGUAGE_H


#include <inttypes.h>
#include <set>
#include <vector>


enum SymbolType
{
    NONE,
    VARIABLE,
    CONSTANT,
    OPERATION,
    RELATION
};

class Symbol
{
    Symbol() = delete;

public:

    const SymbolType type;
    const uint64_t id;
    const std::size_t arity;

protected:
    Symbol(SymbolType type, std::size_t arity = 0) :
        type(type),
        id([]() -> uint64_t { static uint64_t counter = 0; return ++counter; }()),
        arity(arity)
    {
    }

public:
    Symbol(const Symbol &other) :
        type(other.type),
        id(other.id),
        arity(other.arity)
    {
    }

    bool operator ==(const Symbol &other) const
    {
        return other.id == id;
    }

    bool operator !=(const Symbol &other) const
    {
        return other.id != id;
    }

    bool operator <(const Symbol &other) const
    {
        return id < other.id;
    }
};

class OperationSymbol : public Symbol
{
public:
    OperationSymbol(std::size_t arity) :
        Symbol(OPERATION, arity)
    {
    }

    OperationSymbol(const OperationSymbol &other) :
        Symbol(other)
    {
    }

    OperationSymbol(const Symbol&) = delete;
};

class ConstantSymbol : public OperationSymbol
{
public:
    ConstantSymbol() :
        OperationSymbol(0)
    {
    }

    ConstantSymbol(const ConstantSymbol &other) :
        OperationSymbol(other)
    {
    }

    ConstantSymbol(const OperationSymbol&) = delete;
};

class RelationSymbol : public Symbol
{
public:
    RelationSymbol(std::size_t arity) :
        Symbol(RELATION, arity)
    {
    }

    RelationSymbol(const RelationSymbol &other) :
        Symbol(other)
    {
    }

    RelationSymbol(const Symbol&) = delete;
};

class Variable : public Symbol
{
public:
    Variable(const Symbol &other) :
        Symbol(other)
    {
        if (other.type != VARIABLE) {
            throw(0);
        }
    }

    Variable() :
        Symbol(VARIABLE)
    {
    }

    Variable(const Variable &other) :
        Symbol(other)
    {
    }

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
        mutable uint64_t refs;

    protected:
        TermPrivate(Symbol symbol, const std::vector<Term> &args) :
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

        TermPrivate(Symbol symbol) :
            symbol(symbol),
            args(),
            freeVariables(),
            freeVariablesComputed(symbol.type == CONSTANT),
            refs(1)
        {
        }

        TermPrivate(Symbol symbol, std::vector<Term> &&args) :
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

    public:
        TermPrivate(const TermPrivate &other) :
            symbol(other.symbol),
            args(other.args),
            freeVariables(other.freeVariables),
            freeVariablesComputed(other.freeVariablesComputed),
            refs(other.refs)
        {
        }

        TermPrivate(TermPrivate &&other) :
            symbol(other.symbol),
            args(std::move(other.args)),
            freeVariables(std::move(other.freeVariables)),
            freeVariablesComputed(other.freeVariablesComputed),
            refs(other.refs)
        {
        }

        bool operator ==(const TermPrivate &other) const
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

        bool operator !=(const TermPrivate &other) const
        {
            return operator ==(other) == false;
        }

        bool operator <(const TermPrivate &other) const
        {
            if (symbol != other.symbol) {
                return symbol < other.symbol;
            }

            if (args.size() != other.args.size()) {
                return args.size() < other.args.size();
            }

            for (std::size_t i = 0; i < args.size(); ++i) {
                if (args[i] != other.args[i]) {
                    return args[i] < other.args[i];
                }
            }

            return false;
        }

        bool isFreeVariable(const Variable &variable) const
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

                    return false;
                }

                break;

            default:
                throw(0);

                break;
            }
        }

        const std::set<Variable>& getFreeVariables() const
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
    };

    class VariableTermPrivate : public TermPrivate
    {
    public:
        VariableTermPrivate(const Variable &variable) :
            TermPrivate(variable)
        {
        }
    };

    class ConstantTermPrivate : public TermPrivate
    {
    public:
        ConstantTermPrivate(const ConstantSymbol &constant) :
            TermPrivate(constant)
        {
        }
    };

    class OperationTermPrivate : public TermPrivate
    {
    public:
        OperationTermPrivate(const OperationSymbol &operation, const std::vector<Term> &args) :
            TermPrivate(operation, args)
        {
        }
    };

public:
    class Term
    {
        Term& operator =(const Term&) = delete;

        static std::set<TermPrivate>& variableTerms()
        {
            static std::set<TermPrivate> object;

            return object;
        }

        static std::set<TermPrivate>& constantTerms()
        {
            static std::set<TermPrivate> object;

            return object;
        }

        static std::set<TermPrivate>& operationTerms()
        {
            static std::set<TermPrivate> object;

            return object;
        }

        const std::set<TermPrivate>::const_iterator position;

        static const std::set<TermPrivate>::const_iterator insert(const TermPrivate &term)
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

    public:
        bool operator ==(const Term &other) const
        {
            if (isEmpty()) {
                return other.isEmpty();
            }

            if (other.isEmpty()) {
                return false;
            }

            return position->symbol.id == other.position->symbol.id;
        }

        bool operator !=(const Term &other) const
        {
            return operator ==(other) == false;
        }

        bool operator <(const Term &other) const
        {
            if (isEmpty()) {
                return other.isEmpty() == false;
            }

            if (other.isEmpty()) {
                return false;
            }

            return position->symbol < other.position->symbol;
        }

        SymbolType type() const
        {
            return position->symbol.type;
        }

        uint64_t id() const
        {
            return position->symbol.id;
        }

        bool isFreeVariable(const Variable &variable) const
        {
            return position->isFreeVariable(variable);
        }

        const std::set<Variable>& getFreeVariables() const
        {
            return position->getFreeVariables();
        }

        bool isEmpty() const
        {
            return &*position == nullptr;
        }

        Term() :
            position()
        {
        }

        Term(const Term &other) :
            position(other.position)
        {
        }

        Term(const Variable &variable) :
            position(insert(VariableTermPrivate(variable)))
        {
        }

        Term(const ConstantSymbol &constantSymbol) :
            position(insert(ConstantTermPrivate(constantSymbol)))
        {
        }

        Term(const OperationSymbol &operationSymbol, const std::vector<Term> &args) :
            position(insert(OperationTermPrivate(operationSymbol, args)))
        {
        }

        Term(const OperationSymbol &operationSymbol, std::vector<Term> &&args) :
            position(insert(OperationTermPrivate(operationSymbol, args)))
        {
        }

        ~Term()
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
    };
};

typedef TermEnvironment::Term Term;

#endif // LANGUAGE_H
