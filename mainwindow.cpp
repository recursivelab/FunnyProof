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
#include "error.h"
#include "readwrite.h"
#include "theory.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::print(const QString &s)
{
    ui->output->appendPlainText(s);
}

void MainWindow::textChanged()
{

    ui->output->clear();

    {
        Dictionary d;
        Reader r(ui->input->toPlainText().toStdWString(), d);

        try {
//            Writer w;
//            Term t = r.parseTerm();

//            print(QString::fromStdWString(w(t, d)));

//            if (r.pos<r.str.size()) {
//                QString message = QString::fromStdWString(r.messageText);

//                message += "\n------------------\n";
//                message += QString::fromStdWString(r.str.substr(r.messagePos));
//                print(message);
//            }
        } catch(const Exception &e) {
            QString message = QString::fromStdWString(r.messageText);

            message += "\n------------------\n";
            message += QString::fromStdWString(r.str.substr(r.messagePos));
            print(message);
            message += "\n------------------\n";
            print(QString::fromStdWString(e.description));
        }
    }

    try {
        Dictionary d;
        Reader r(ui->input->toPlainText().toStdWString(), d);
        Writer w;
        Formula f = r.parseFormula();
        std::vector<Formula> args;
        std::set<Variable> vars;

        print(QString::fromStdWString(w(f,d)));
        print("System:");

        Goal g;
        System s;

        g.insert(FormulaEnvironment::NegationFormula(f));
        s.insert(g);
        systemToLiterals(s);
        produceInequalities(s);
        removeEqualityInequalityContradictions(s);

        for (auto i = s.cbegin(); i!=s.cend(); ++i) {
            const Goal &goal = *i;

            print("Goal:");

            for (auto j = goal.cbegin(); j!=goal.cend(); ++j) {
                print(QString::fromStdWString(w(*j, d)));
            }

            print("Classes:");

            std::vector<std::set<Term>> c = equivalenceClasses(goal);

            for (size_t j = 0; j<c.size(); ++j) {
                QString str("{");
                std::set<Term> s = c[j];

                for (auto k = s.cbegin(); k!=s.cend(); ++k) {
                    if (k!=s.cbegin()) {
                        str += ", ";
                    }

                    str += QString::fromStdWString(w(*k, d));
                }

                str += "}";
                print(str);
            }
        }

        if (concludeContradiction(s)) {
            print("Yes");
        } else {
            print("No");
        }
    } catch(const Exception &e) {
        print(QString::fromStdWString(e.description));
    }
}
