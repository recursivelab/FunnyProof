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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readwrite.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Writer w;
    Dictionary d;
    Variable x;
    d.insert(L"x", x);
    ConstantSymbol c;
    d.insert(L"c", c);
    Term tx(x);
    Term tc(c);
    OperationSymbol f(2);
    d.insert(L"Φ", f);
    Term tf(f, TermEnvironment::twoTerms(tx, tc));
    FormulaEnvironment::EqualityFormula fe(tx, tc);
    RelationSymbol r(2);
    d.insert(L"ρ", r);
    FormulaEnvironment::RelationFormula fr(r, TermEnvironment::twoTerms(tc, tf));
    FormulaEnvironment::ConjunctionFormula fc(fr, fe);
    FormulaEnvironment::DisjunctionFormula fd(fe, fe);
    FormulaEnvironment::ImplicationFormula fi(fc, fd);

    ui->output->appendPlainText(QString::fromStdWString(w(tx, d)));
    ui->output->appendPlainText(QString::fromStdWString(w(fi, d)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
