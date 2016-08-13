#ifndef THEORY_H
#define THEORY_H

#include <language.h>

typedef std::set<Formula> Goal;

bool operator ==(const Goal &first, const Goal &second);
bool operator !=(const Goal &first, const Goal &second);
bool operator <(const Goal &first, const Goal &second);

typedef std::set<Goal> System;

int cmp(const Goal &g1, const Goal &g2);
void removeSupergoals(System &goals);
void systemToLiterals(System &goals);
bool containsInequality(const Goal &goal, const Term &t1, const Term &t2);
bool produceInequalities(const Goal &goal, std::set<Goal> &result);
std::vector<std::set<Term>> equivalenceClasses(const Goal &goal);
void produceInequalities(System &goals);
void removeEqualityInequalityContradictions(System &goals);
bool concludeContradiction(const System &system);

class Theory
{
    mutable std::set<Formula> theoremsSet;

public:
    const std::set<Formula> axioms;

    Theory(const std::set<Formula> &axioms);
    Theory(std::set<Formula> &&axioms);
    Theory(const Theory &theory);
    Theory(Theory &&theory);
    const std::set<Formula>& theorems() const;
    bool contains(const Formula &formula) const;
    bool draw(const Formula &formula) const;
};

#endif // THEORY_H
