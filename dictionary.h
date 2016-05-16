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
public:
    class Environment
    {
        std::map<std::wstring, Symbol> symbols;
        std::map<Symbol, std::wstring> names;

    public:
        DECLARE Environment();
        DECLARE Environment(const Environment &other);
        DECLARE Environment(Environment &&other);
        DECLARE Environment(const std::map<std::wstring, Symbol> &symbols);
        DECLARE Environment(std::map<std::wstring, Symbol> &&symbols);
        DECLARE bool insert(const std::wstring &name, const Symbol &symbol);
        DECLARE bool insert(std::wstring &&name, const Symbol &symbol);
        DECLARE bool insert(const Symbol &symbol, const std::wstring &name);
        DECLARE bool insert(const Symbol &symbol, std::wstring &&name);
        DECLARE const Symbol& operator ()(const std::wstring &name) const;
        DECLARE std::wstring operator ()(const Symbol &symbol) const;
        DECLARE const std::map<std::wstring, Symbol>& getSymbols() const;
        DECLARE const std::map<Symbol, std::wstring>& getNames() const;
    };

private:
    std::vector<Environment> environents;

public:
    DECLARE Dictionary();
    DECLARE void push();
    DECLARE bool pop();
    DECLARE bool mergeTop2Environments();
    DECLARE Symbol operator ()(const std::wstring &name) const;
    DECLARE std::wstring operator ()(const Symbol &symbol) const;
    DECLARE const Environment& getCurrentEnvironment() const;
    DECLARE const std::vector<Environment>& getEnvironments() const;
    DECLARE bool insert(const std::wstring &name, const Symbol &symbol);
    DECLARE bool insert(std::wstring &&name, const Symbol &symbol);
    DECLARE std::wstring operator ()(const Term &term) const;
    DECLARE std::wstring operator ()(const Formula &formula) const;
};


#ifdef INLINE

#include "dictionary_imp.h"

#endif

#endif // DICTIONARY_H
