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
    names(other.names)
{
}

Dictionary::Environment::Environment(Environment &&other) :
    names(other.names)
{
}

Dictionary::Environment::Environment(const std::map<std::wstring, Symbol> &names) :
    names(names)
{
}

Dictionary::Environment::Environment(std::map<std::wstring, Symbol> &&names) :
    names(names)
{
}

DECLARE bool Dictionary::Environment::insert(const std::wstring &name, const Symbol &symbol)
{
    std::map<std::wstring, Symbol>::iterator i = names.find(name);

    if (i == names.end()) {
        names.emplace(std::make_pair(name, symbol));

        return true;
    }

    return false;
}

DECLARE bool Dictionary::Environment::insert(std::wstring &&name, const Symbol &symbol)
{
    std::map<std::wstring, Symbol>::iterator i = names.find(name);

    if (i == names.end()) {
        names.emplace(std::make_pair(name, symbol));

        return true;
    }

    return false;
}

const Symbol& Dictionary::Environment::operator ()(const std::wstring &name) const
{
    std::map<std::wstring, Symbol>::const_iterator i = names.find(name);

    if (i == names.cend()) {
        return Symbol::dummy();
    }

    return i->second;
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

#endif // DICTIONARY_IMP_H
