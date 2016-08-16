#include <algorithm>
#include "theory.h"

#include <iostream>
#include "readwrite.h"
using namespace std;

typedef std::set<Formula> Goal;

bool operator ==(const Goal &first, const Goal &second)
{
    if (first.size()!=second.size()) {
        return false;
    }

    auto i = first.cbegin();
    auto j = second.cbegin();

    while (i!=first.cend() && j!=second.cend()) {
        if (*i!=*j) {
            return false;
        }

        ++i;
        ++j;
    }

    return true;
}

bool operator !=(const Goal &first, const Goal &second)
{
    return operator ==(first, second)==false;
}

bool operator <(const Goal &first, const Goal &second)
{
    if (first.size()!=second.size()) {
        return first.size()<second.size();
    }

    auto i = first.cbegin();
    auto j = second.cbegin();

    while (i!=first.cend() && j!=second.cend()) {
        if (*i!=*j) {
            return *i<*j;
        }

        ++i;
        ++j;
    }

    return false;
}

typedef std::set<Goal> System;

int cmp(const Goal &g1, const Goal &g2)
{
    if (g1.size()<g2.size()) {
        if (std::includes(g2.cbegin(), g2.cend(), g1.cbegin(), g1.cend())) {
            return -1;
        }

        return 0;
    }

    if (g1.size()>g2.size()) {
        if (std::includes(g1.cbegin(), g1.cend(), g2.cbegin(), g2.cend())) {
            return 1;
        }

        return 0;
    }

    return 0;
}

void removeSupergoals(System &goals)
{
    while (true) {
        bool changed = false;

        for (auto i = goals.cbegin(); i!=goals.cend(); ++i) {
            auto j = i;
            const Goal &goal1  = *i;

            for (++j; j!=goals.cend(); ++j) {
                const Goal &goal2 = *j;
                int result = cmp(goal1, goal2);

                if (result!=0) {
                    changed = true;
                    goals.erase(result>0 ? i : j);

                    break;
                }
            }

            if (changed) {
                break;
            }
        }

        if (changed==false) {
            break;
        }
    }
}

void tryNone(System &goals)
{
    for (auto i = goals.cbegin(); i!=goals.cend(); ++i) {
        const Goal &goal = *i;

        for (auto j = goal.cbegin(); j!=goal.cend(); ++j) {
            const Formula &formula = *j;

            if (formula.type()==NONE_SYMBOL) {
                throw(1);
            }
        }
    }
}

bool tryLiteral(System &goals)
{
    for (auto i = goals.cbegin(); i!=goals.cend(); ++i) {
        const Goal &goal = *i;

        for (auto j = goal.cbegin(); j!=goal.cend(); ++j) {
            const Formula &formula = *j;
            std::vector<Formula> args;
            std::set<Variable> vars;
            UniformType type = formula.uniformType(args, vars);

            if (type==LITERAL) {
                const Formula &f = args[0];

                if (f.type()==NONE_SYMBOL) {
                    throw(1);
                }

                if (f.type()==FALSE_SYMBOL) {
                    goals.erase(i);

                    return true;
                }

                if (f.type()==TRUE_SYMBOL) {
                    Goal g = goal;

                    g.erase(*j);
                    goals.erase(i);
                    goals.insert(g);

                    return true;
                }

                if (f!=formula) {
                    Goal g = goal;

                    g.erase(*j);
                    g.insert(f);
                    goals.erase(i);
                    goals.insert(g);

                    return true;
                }
            }
        }
    }

    return false;
}

bool tryAlphaGamaDelta(System &goals)
{
    for (auto i = goals.cbegin(); i!=goals.cend(); ++i) {
        const Goal &goal = *i;

        for (auto j = goal.cbegin(); j!=goal.cend(); ++j) {
            const Formula &formula = *j;
            std::vector<Formula> args;
            std::set<Variable> vars;
            UniformType type = formula.uniformType(args, vars);

            if (type==ALPHA) {
                Goal g = goal;

                g.erase(*j);

                for (size_t i = 0; i<args.size(); ++i) {
                    g.insert(args[i]);
                }

                goals.erase(i);
                goals.insert(g);

                return true;
            }

            if (type==GAMMA) {
                Goal g = goal;

                g.erase(*j);

                std::map<Variable, Term> sub;

                for (auto k = vars.cbegin(); k!=vars.cend(); ++k) {
                    sub.insert(std::pair<Variable, Term>(*k, Term(Variable())));
                }

                Substitution subst(sub);

                g.insert(args[0][subst]);
                goals.erase(i);
                goals.insert(g);

                return true;
            }

            if (type==DELTA) {
                Goal g = goal;

                g.erase(*j);

                std::map<Variable, Term> sub;

                for (auto k = vars.cbegin(); k!=vars.cend(); ++k) {
                    sub.insert(std::pair<Variable, Term>(*k, Term(ConstantSymbol())));
                }

                Substitution subst(sub);

                g.insert(args[0][subst]);
                goals.erase(i);
                goals.insert(g);

                return true;
            }
        }
    }

    return false;
}

bool tryBeta(System &goals)
{
    for (auto i = goals.cbegin(); i!=goals.cend(); ++i) {
        const Goal &goal = *i;

        for (auto j = goal.cbegin(); j!=goal.cend(); ++j) {
            const Formula &formula = *j;
            std::vector<Formula> args;
            std::set<Variable> vars;
            UniformType type = formula.uniformType(args, vars);

            if (type==BETA) {
                Goal g = goal;

                g.erase(*j);
                goals.erase(i);

                for (size_t i = 0; i<args.size(); ++i) {
                    Goal newGoal = g;

                    newGoal.insert(args[i]);
                    goals.insert(newGoal);
                }

                return true;
            }
        }
    }

    return false;
}

void systemToLiterals(System &goals)
{
    while (goals.empty()==false) {
        if (tryLiteral(goals)) {
            continue;
        }

        if (tryAlphaGamaDelta(goals)) {
            continue;
        }

        if (tryBeta(goals)) {
            continue;
        }

        break;
    }
}

bool containsInequality(const Goal &goal, const Term &t1, const Term &t2)
{
    for (auto i = goal.cbegin(); i!=goal.cend(); ++i) {
        const Formula &f = *i;

        if (f.type()==NONEQUALITY) {
            bool found1 = false;
            bool found2 = false;

            for (size_t i = 0; i<f.terms().size(); ++i) {
                if (found1==false && f.terms()[i]==t1) {
                    if (found2) {
                        return true;
                    }

                    found1 = true;
                } else if (found2==false && f.terms()[i]==t2) {
                    if (found1) {
                        return true;
                    }

                    found2 = true;
                }
            }
        }
    }

    return false;
}

bool produceInequalities(const Goal &goal, std::set<Goal> &result)
{
    result.clear();

    for (auto i = goal.cbegin(); i!=goal.cend(); ++i) {
        const Formula &f = *i;

        switch (f.type()) {
            case NONEQUALITY:
        {
            const std::vector<Term> &terms = f.terms();
            std::set<Term> s;

            for (size_t j = 0; j<terms.size(); ++j) {
                s.insert(terms[j]);
            }

            if (s.size()<terms.size()) {
                return true;
            }

            for (auto j = s.cbegin(); j!=s.cend(); ++j) {
                const Term &t1 = *j;

                if (t1.type()!=OPERATION) {
                    continue;
                }

                auto k = j;

                for (++k; k!=s.cend(); ++k) {
                    const Term &t2 = *k;

                    if (t2.symbol()!=t1.symbol()) {
                        continue;
                    }

                    bool found = false;

                    for (size_t l=0; l<t1.args().size() && l<t2.args().size(); ++l) {
                        if (containsInequality(goal, t1.args()[l], t2.args()[l])) {
                            found = true;

                            break;
                        }
                    }

                    if (found) {
                        continue;
                    }

                    for (size_t l=0; l<t1.args().size() && l<t2.args().size(); ++l) {
                        Goal other = goal;

                        other.insert(FormulaEnvironment::NonequalityFormula(t1.args()[l], t2.args()[l]));
                        result.insert(other);
                    }

                    return true;
                }
            }
        }

            break;

        case NEGATION:
        {
            const Formula &f1 = f.formulas()[0];

            if (f1.type()!=RELATION) {
                break;
            }

            for (auto j = goal.cbegin(); j!=goal.cend(); ++j) {
                const Formula &f2 = *j;

                if (f2.type()!=RELATION || f2.symbol()!=f1.symbol()) {
                    continue;
                }

                if (f2==f1) {
                    result.clear();

                    return true;
                }

                bool found = false;

                for (size_t l=0; l<f1.terms().size() && l<f2.terms().size(); ++l) {
                    if (containsInequality(goal, f1.terms()[l], f2.terms()[l])) {
                        found = true;

                        break;
                    }
                }

                if (found) {
                    continue;
                }

                for (size_t k = 0; k<f1.terms().size() && k<f2.terms().size(); ++k) {
                    Goal other = goal;

                    other.insert(FormulaEnvironment::NonequalityFormula(f1.terms()[k], f2.terms()[k]));
                    result.insert(other);
                }

                return true;
            }
        }

        default:
            break;
        }
    }

    return result.empty()==false;
}

std::vector<std::set<Term>> equivalenceClasses(const Goal &goal)
{
    std::vector<std::set<Term>> result;

    for (auto i = goal.cbegin(); i!=goal.cend(); ++i) {
        const Formula &f = *i;

        if (f.type()==EQUALITY) {
            size_t pos = result.size();

            for (size_t j = 0; j<f.terms().size(); ++j) {
                const Term &t = f.terms()[j];
                bool found = false;

                for (size_t k = 0; k<result.size(); ++k) {
                    if (result[k].count(t)>0) {
                        found = true;

                        if (k!=pos) {
                            if (pos<result.size()) {
                                std::set<Term> u = std::move(result[k]);

                                for (auto l = result[pos].cbegin(); l!=result[pos].cend(); ++l) {
                                    u.insert(*l);
                                }

                                std::vector<std::set<Term>> newResult;

                                for (size_t l = 0; l<result.size(); ++l) {
                                    if (l!=k && l!=pos) {
                                        newResult.push_back(std::move(result[l]));
                                    }
                                }

                                result = std::move(newResult);
                                result.push_back(std::move(u));
                                pos = result.size()-1;
                            } else {
                                result[k].insert(t);
                                pos = k;
                            }
                        }

                        break;
                    }
                }

                if (found==false) {
                    if (pos<result.size()) {
                        result[pos].insert(t);
                    } else {
                        std::set<Term> s;

                        s.insert(t);
                        result.push_back(std::move(s));
                        pos = result.size()-1;
                    }
                }
            }
        } else if (f.type()==NONEQUALITY) {
            for (size_t j = 0; j<f.terms().size(); ++j) {
                const Term &t = f.terms()[j];

                size_t pos;

                for (pos = 0; pos<result.size(); ++pos) {
                    if (result[pos].count(t)>0) {
                        break;
                    }
                }

                if (pos==result.size()) {
                    std::set<Term> s;

                    s.insert(t);
                    result.push_back(s);
                }
            }
        }
    }

    return result;
}

void produceInequalities(System &goals)
{
    while (true) {
        bool changed = false;

        for (auto i = goals.cbegin(); i!=goals.cend(); ++i) {
            std::set<Goal> replacement;

            if (produceInequalities(*i, replacement)) {
                for (auto j = goals.cbegin(); j!=goals.cend(); ++j) {
                    if (j!=i) {
                        replacement.insert(*j);
                    }
                }

                goals = replacement;
                changed = true;

                break;
            }
        }

        if (changed==false) {
            break;
        }

        removeSupergoals(goals);
    }
}

void removeEqualityInequalityContradictions(System &goals)
{
    std::set<Goal> gs;

    for (auto i = goals.cbegin(); i!=goals.cend(); ++i) {
        bool found = false;
        auto c = equivalenceClasses(*i);

        for (size_t j = 0; j<c.size(); ++j) {
            const std::set<Term> terms = c[j];

            for (auto k=terms.cbegin(); k!=terms.cend(); ++k) {
                for (auto l=k; l!=terms.cend(); ++l) {
                    if (containsInequality(*i, *k, *l)) {
                        containsInequality(*i, *k, *l);
                        found = true;

                        break;
                    }
                }

                if (found) {
                    break;
                }
            }

            if (found) {
                break;
            }
        }

        if (found==false) {
            gs.insert(*i);
        }
    }

    goals = std::move(gs);
}

bool concludeContradiction(const System &system)
{
    System goals = system;
    
    removeSupergoals(goals);
    systemToLiterals(goals);
    removeSupergoals(goals);
    produceInequalities(goals);
    removeSupergoals(goals);
    removeEqualityInequalityContradictions(goals);

    if (goals.empty()) {
        return true;
    }

    const Goal &goal = *(goals.cbegin());
    auto c = equivalenceClasses(goal);

    for (auto j = goal.cbegin(); j!=goal.cend(); ++j) {
        const Formula &formula = *j;

        if (formula.type()==NONEQUALITY) {
            for (size_t k = 1; k<formula.terms().size(); ++k) {
                const Term &t1 = formula.terms()[k];

                size_t p1;

                for (p1 = 0; p1<c.size(); ++p1) {
                    if (c[p1].count(t1)>0) {
                        break;
                    }
                }

                const std::set<Term> &c1 = c[p1];

                for (size_t l = 0; l<k; ++l) {
                    const Term &t2 = formula.terms()[l];
                    size_t p2;

                    for (p2 = 0; p2<c.size(); ++p2) {
                        if (c[p2].count(t2)>0) {
                            break;
                        }
                    }

                    const std::set<Term> &c2 = c[p2];

                    for (auto q1 = c1.cbegin(); q1!=c1.cend(); ++q1) {
                        for (auto q2 = c2.cbegin(); q2!=c2.cend(); ++q2) {
                            bool ok;
                            Substitution substitution = TermEnvironment::unificator(t1, t2, ok);

                            if (ok) {
                                System newSystem;

                                for (auto g = goals.cbegin(); g!=goals.cend(); ++g) {
                                    if (g!=goals.cbegin()) {
                                        Goal newGoal;

                                        for (auto f = g->cbegin(); f!=g->cend(); ++f) {
                                            newGoal.insert((*f)[substitution]);
                                        }

                                        newSystem.insert(newGoal);
                                    }
                                }

                                if (concludeContradiction(newSystem)) {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (auto j = goal.cbegin(); j!=goal.cend(); ++j) {
        const Formula &formula = *j;

        if (formula.type()==NONEQUALITY) {
            for (size_t k = 1; k<formula.terms().size(); ++k) {
                const Term &t1 = formula.terms()[k];

                for (size_t l = 0; l<k; ++l) {
                    const Term &t2 = formula.terms()[l];

                    for (auto cl = c.cbegin(); cl!=c.cend(); ++cl) {
                        const std::set<Term> &cls = *cl;

                        for (auto tm1 = cls.cbegin(); tm1!=cls.cend(); ++tm1) {
                            Term trm1 = *tm1;

                            for (auto tm2 = cls.cbegin(); tm2!=cls.cend(); ++tm2) {
                                Term trm2 = *tm2;
                                bool ok;
                                std::vector<std::pair<Term, Term>> unificationTask;

                                unificationTask.push_back(std::pair<Term, Term>(t1, trm1));
                                unificationTask.push_back(std::pair<Term, Term>(t2, trm2));

                                Substitution substitution =TermEnvironment::unificator(unificationTask, ok);

                                if (ok) {
                                    System newSystem;

                                    for (auto g = goals.cbegin(); g!=goals.cend(); ++g) {
                                        if (g!=goals.cbegin()) {
                                            Goal newGoal;

                                            for (auto f = g->cbegin(); f!=g->cend(); ++f) {
                                                newGoal.insert((*f)[substitution]);
                                            }

                                            newSystem.insert(newGoal);
                                        }
                                    }

                                    if (concludeContradiction(newSystem)) {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return goals.empty();
}

Theory::Theory(const std::set<Formula> &axioms) :
    axioms(axioms)
{
    theoremsSet = axioms;
}

Theory::Theory(std::set<Formula> &&axioms) :
    axioms(axioms)
{
    theoremsSet = axioms;
}

Theory::Theory(const Theory &theory) :
    axioms(theory.axioms)
{
    theoremsSet = axioms;
}

Theory::Theory(Theory &&theory) :
    axioms(theory.axioms)
{
    theoremsSet = std::move(theory.theoremsSet);
}

const std::set<Formula> &Theory::theorems() const
{
    return theoremsSet;
}

bool Theory::contains(const Formula &formula) const
{
    return theoremsSet.count(formula) > 0;
}

bool Theory::draw(const Formula &formula) const
{
    if (contains(formula)) {
        return true;
    }

    Goal g = theoremsSet;

    g.insert(FormulaEnvironment::NegationFormula(formula));

    System s;

    s.insert(g);

    if (concludeContradiction(s)) {
        theoremsSet.insert(formula);

        return true;
    }

    return false;
}
