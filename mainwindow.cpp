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
    ui->automatedProofAnalysis->appendPlainText(s);
}

void MainWindow::updateProof()
{
    ui->proofAnalysis->clear();

    Dictionary d;

    try {
        Reader r(ui->axioms->toPlainText().toStdWString(), d);
        Formula axiom = r.parseFormula();
        Writer w;

        ui->proofAnalysis->appendPlainText("Axiom:");
        ui->proofAnalysis->appendPlainText(QString::fromStdWString(w(axiom, d)));

        Reader r1(ui->goal->text().toStdWString(), d);
        Formula goal = r1.parseFormula();

        ui->proofAnalysis->appendPlainText("Goal:");
        ui->proofAnalysis->appendPlainText(QString::fromStdWString(w(goal, d)));

        std::set<Formula> s;

        s.insert(axiom);

        Theory t(s);
        Reader r2(ui->proof->toPlainText().toStdWString(), d);

        ui->proofAnalysis->appendPlainText("Proof:");

        std::vector<Formula> proof;

        while (t.draw(goal)==false) {
            bool end = false;

            try {
                Formula f = r2.parseFormula();

                proof.push_back(f);

                if (t.draw(f)) {
                    ui->proofAnalysis->appendPlainText(QString::fromStdWString(w(f, d)));
                } else {
                    end = true;
                }
            } catch(...) {
                end = true;
            }

            if (end) {
                break;
            }
        }

        if (t.draw(goal)) {
            ui->proofAnalysis->appendPlainText("Success");
        }

    } catch(...) {
    }
}

void MainWindow::analysisFormulaChanged()
{
    ui->automatedProofAnalysis->clear();

    try {
        Dictionary d;
        Reader r(ui->formulaForAnalysis->toPlainText().toStdWString(), d);
        Writer w;
        Formula f = r.parseFormula();

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
    }
}

void MainWindow::axiomChanged()
{
    updateProof();
}

void MainWindow::goalChanged()
{
    updateProof();
}

void MainWindow::proofChanged()
{
    updateProof();
}

void MainWindow::simplificationFormulaChanged()
{
    Dictionary d;
    Writer w;

    ui->simplificatedFormula->clear();

    try {
        Reader r(ui->formulaForSimplification->toPlainText().toStdWString(), d);
        Formula f = r.parseFormula();

        ui->simplificatedFormula->setPlainText(QString::fromStdWString(w(f.simplify(), d)));
    } catch(...) {
    }
}
