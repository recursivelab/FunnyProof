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
    std::wstring empty;
    std::map<Symbol, std::wstring>::const_iterator i = names.find(symbol);

    if (i == names.cend()) {
        return empty;
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
    if (environents.size() == 1) {
        return false;
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
    std::wstring empty;
    size_t i = environents.size();

    do {
        --i;

        const std::wstring &result = environents[i](symbol);

        if (result.empty() == false) {
            return result;
        }
    } while (i != 0);

    return empty;
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
    std::wstring empty;

    switch (term.type()) {
    case NONE_SYMBOL:
        return empty;

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

            result += operator ()(term.getArgs()[i]);
        }

        result += L")";

        return result;
    }
    default:
        throw(1);

        break;
    }
}

#endif // DICTIONARY_IMP_H
