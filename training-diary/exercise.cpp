#include "exercise.h"

Exercise::Exercise(const QString& name, int sets, int repsPerSet)
    : name(name), sets(sets), repsPerSet(repsPerSet) {}

QString Exercise::getName() const {
    return name;
}

int Exercise::getSets() const {
    return sets;
}

int Exercise::getRepsPerSet() const {
    return repsPerSet;
}
