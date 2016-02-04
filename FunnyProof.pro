################################################################################
#                                                                              #
#     FunnyProof - Easy for use proof assistant.                               #
#     Copyright (C) 2015  Nedeljko Stefanovic                                  #
#                                                                              #
#     This program is free software: you can redistribute it and/or modify     #
#     it under the terms of version 3 of the GNU General Public License as     #
#     published by the Free Software Foundation.                               #
#                                                                              #
#     This program is distributed in the hope that it will be useful,          #
#     but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#     GNU General Public License for more details.                             #
#                                                                              #
#     You should have received a copy of the GNU General Public License        #
#     along with this program.  If not, see <http://www.gnu.org/licenses/>.    #
#                                                                              #
################################################################################


TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

DEFINES += INLINE

SOURCES += main.cpp \
    dictionary.cpp \
    language.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    config.h \
    error.h \
    language.h \
    dictionary.h \
    language_imp.h

