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

#ifndef UTILITY_IMP_H
#define UTILITY_IMP_H

#include <cwctype>
#include "utility.h"

void toLower(std::wstring &str)
{
    for (size_t i = 0; i<str.size(); ++i) {
        str[i] = std::towlower(str[i]);
    }
}

void toUpper(std::wstring &str)
{
    for (size_t i = 0; i<str.size(); ++i) {
        str[i] = std::towupper(str[i]);
    }
}

std::wstring toLower(const std::wstring &str)
{
    std::wstring result;

    result.resize(str.size());

    for (size_t i = 0; i<str.size(); ++i) {
        result[i] = std::towlower(str[i]);
    }

    return result;
}

std::wstring toUpper(const std::wstring &str)
{
    std::wstring result;

    result.resize(str.size());

    for (size_t i = 0; i<str.size(); ++i) {
        result[i] = std::towupper(str[i]);
    }

    return result;
}

#endif // UTILITY_IMP_H
