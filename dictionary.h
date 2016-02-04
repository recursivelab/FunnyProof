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


#ifndef DICTIONARY_H
#define DICTIONARY_H


#include "language.h"


class Dictionary
{
    class Environment
    {
        std::map<std::wstring, Symbol> names;

    public:
        Environment() :
            names()
        {
        }

        Environment(const Environment &other) :
            names(other.names)
        {
        }

        Environment(Environment &&other) :
            names(other.names)
        {
        }

        Environment(const std::map<std::wstring, Symbol> &names) :
            names(names)
        {
        }

        Environment(std::map<std::wstring, Symbol> &&names) :
            names(names)
        {
        }

        bool insert(const std::wstring &name, const Symbol &symbol)
        {
            std::map<std::wstring, Symbol>::iterator i = names.find(name);

            if (i == names.end()) {
                names.emplace(std::make_pair(name, symbol));

                return true;
            }

            return false;
        }

        const Symbol& operator ()(const std::wstring &name) const
        {
            std::map<std::wstring, Symbol>::const_iterator i = names.find(name);

            if (i == names.cend()) {
                return Symbol::dummy();
            }

            return i->second;
        }
    };

    std::vector<Environment> environents;

public:
    Dictionary()
    {
        environents.resize(1);
    }

    void push()
    {
        environents.push_back(Environment());
    }

    bool pop()
    {
        if (environents.size() == 1) {
            return false;
        }

        environents.pop_back();

        return true;
    }

    Symbol operator ()(const std::wstring &name) const
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
};

#endif // DICTIONARY_H
