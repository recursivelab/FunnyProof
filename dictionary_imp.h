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


#ifndef DICTIONARY_IMP_H
#define DICTIONARY_IMP_H

#include "dictionary.h"

Dictionary::Environment::Environment()
{
}

Dictionary::Environment::Environment(const Environment &other) :
    symbols(other.symbols)
{
}

Dictionary::Environment::Environment(Environment &&other) :
    symbols(other.symbols)
{
}

Dictionary::Environment::Environment(const std::map<std::wstring, Symbol> &names) :
    symbols(names)
{
}

Dictionary::Environment::Environment(std::map<std::wstring, Symbol> &&names) :
    symbols(names)
{
}

DECLARE bool Dictionary::Environment::insert(const std::wstring &name, const Symbol &symbol)
{
    if (symbols.find(name) == symbols.end()) {
        symbols.emplace(std::make_pair(name, symbol));
        names.emplace(std::make_pair(symbol, name));

        return true;
    }

    return false;
}

DECLARE bool Dictionary::Environment::insert(std::wstring &&name, const Symbol &symbol)
{
    if (symbols.find(name) == symbols.end()) {
        symbols.emplace(std::make_pair(static_cast<std::wstring&>(name), symbol));
        names.emplace(std::make_pair(symbol, name));

        return true;
    }

    return false;
}

bool Dictionary::Environment::insert(const Symbol &symbol, const std::wstring &name)
{
    return insert(name, symbol);
}

bool Dictionary::Environment::insert(const Symbol &symbol, std::wstring &&name)
{
    return insert(name, symbol);
}

const Symbol& Dictionary::Environment::operator ()(const std::wstring &name) const
{
    std::map<std::wstring, Symbol>::const_iterator i = symbols.find(name);

    if (i == symbols.cend()) {
        return Symbol::dummy();
    }

    return i->second;
}

std::wstring Dictionary::Environment::operator ()(const Symbol &symbol) const
{
    std::map<Symbol, std::wstring>::const_iterator i = names.find(symbol);

    if (i == names.cend()) {
        return std::wstring();
    }

    return i->second;
}

const std::map<std::wstring, Symbol>& Dictionary::Environment::getSymbols() const
{
    return symbols;
}

const std::map<Symbol, std::wstring>& Dictionary::Environment::getNames() const
{
    return names;
}

Dictionary::Dictionary()
{
    push();
}

void Dictionary::push()
{
    environents.push_back(Environment());
}

bool Dictionary::pop()
{
    if (environents.size()<=1) {
        return false;
    }

    environents.pop_back();

    return true;
}

bool Dictionary::mergeTop2Environments()
{
    if (environents.size()<=1) {
        return false;
    }

    const Environment &firstEnvironment = environents[environents.size()-1];
    Environment &secondEnvironment = environents[environents.size()-2];
    const std::map<Symbol, std::wstring> &firstNames = firstEnvironment.getNames();

    for (auto i = firstNames.cbegin(); i!=firstNames.cend(); ++i) {
        secondEnvironment.insert(i->first, i->second);
    }

    environents.pop_back();

    return true;
}

Symbol Dictionary::operator ()(const std::wstring &name) const
{
    size_t i = environents.size();

    do {
        --i;

        Symbol result = environents[i](name);

        if (result.type != NONE_SYMBOL) {
            return result;
        }
    } while (i != 0);

    return Symbol::dummy();
}

std::wstring Dictionary::operator ()(const Symbol &symbol) const
{
    size_t i = environents.size();

    do {
        --i;

        const std::wstring &result = environents[i](symbol);

        if (result.empty() == false) {
            return result;
        }
    } while (i != 0);

    return std::wstring();
}

bool Dictionary::insert(const std::wstring &name, const Symbol &symbol)
{
    size_t i = environents.size();

    --i;

    return environents[i].insert(name, symbol);
}

bool Dictionary::insert(std::wstring &&name, const Symbol &symbol)
{
    size_t i = environents.size();

    --i;

    return environents[i].insert(name, symbol);
}

std::wstring Dictionary::operator ()(const Term &term) const
{
    switch (term.type()) {
    case NONE_SYMBOL:
        return std::wstring();

        break;

    case VARIABLE: case CONSTANT:
        return operator ()(term.symbol());

        break;

    case OPERATION:
    {
        std::wstring result = operator ()(term.symbol());

        result += L"(";

        for (size_t i = 0; i < term.arity(); ++i) {
            if (i > 0) {
                result += L",";
            }

            result += operator ()(term.args()[i]);
        }

        result += L")";

        return result;
    }

    default:
        throw(1);

        break;
    }
}

std::wstring Dictionary::operator ()(const Formula &formula) const
{
    std::wstring result;

    switch (formula.type()) {
    case NONE_SYMBOL:
        result = L"↑";

        break;

    case FALSE_SYMBOL:
        result = L"⊥";

        break;

    case TRUE_SYMBOL:
        result = L"⊤";

        break;

    case EQUALITY:
    {
        const std::vector<Term> &terms = formula.terms();

        if (terms.size() < 2) {
            return L"⊤";
        }

        result = operator ()(terms[0]);

        for (size_t i = 1; i < terms.size(); ++i) {
            result += L"=";
            result += operator ()(terms[1]);
        }

        return result;
    }
        break;

    case RELATION:
    {
        const std::vector<Term> &terms = formula.terms();

        if (terms.size() < 2) {
            throw(1);
        }

        result += operator ()(formula.symbol());
        result += L"(";
        result += operator ()(terms[0]);

        for (size_t i = 1; i < terms.size(); ++i) {
            result += L",";
            result += operator ()(terms[1]);
        }

        result += L")";

        return result;
    }

        break;

    case NEGATION:
        if (formula.formulas().size() != 1) {
            throw(1);
        }

        result += L"¬";

        switch (formula.formulas()[0].type()) {
        case EQUALITY: case RELATION: case NEGATION: case UNIVERSAL: case EXISTENTIAL:
            result += operator ()(formula.formulas()[0]);

            return result;

            break;

        case CONJUNCTION: case DISJUNCTION: case IMPLICATION: case EQUIVALENCE:
            if (formula.formulas()[0].formulas().size() > 1) {
                result += L"(";
                result += operator ()(formula.formulas()[0]);
                result += L")";
            } else {
                result += operator ()(formula.formulas()[0]);
            }

            return result;

            break;

        default:
            throw(1);

            break;
        }

        break;

    case CONJUNCTION: case DISJUNCTION: case IMPLICATION: case EQUIVALENCE:
        if (formula.formulas().size() > 1) {
            result += L"(";
            result += operator ()(formula.formulas()[0]);

            for (size_t i = 1; i < formula.formulas().size(); ++i) {
                switch (formula.type()) {
                case CONJUNCTION:
                    result += L"∧";

                    break;

                case DISJUNCTION:
                    result += L"∨";

                    break;

                case IMPLICATION:
                    result += L"⇒";

                    break;

                case EQUIVALENCE:
                    result += L"⇔";

                    break;

                default:
                    throw(1);

                    break;
                }

                result += operator ()(formula.formulas()[i]);
            }

            result += L")";
        } else if (formula.formulas().size() == 1) {
            switch (formula.type()) {
            case CONJUNCTION: case DISJUNCTION:
                result += operator ()(formula.formulas()[0]);

                break;

            case IMPLICATION: case EQUIVALENCE:
                result = L"⊤";

                break;

            default:
                throw(1);

                break;
            }
        } else {
            switch (formula.type()) {
            case CONJUNCTION: case IMPLICATION: case EQUIVALENCE:
                result = L"⊤";

                break;

            case DISJUNCTION:
                result = L"⊥";

                break;

            default:
                throw(1);

                break;
            }
        }

        break;

    case UNIVERSAL: case EXISTENTIAL:
        if (formula.formulas().size() != 1) {
            throw(1);
        }

        if (formula.variables().empty()) {
            return operator ()(formula.formulas()[0]);
        }

        result += L"(";
        result += formula.type() == UNIVERSAL ? L"∀" : L"∃";

        result += L")";

        break;

    default:
        throw(1);

        break;
    }

    return std::wstring();
}

#endif // DICTIONARY_IMP_H
