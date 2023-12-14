#include "exercise.h"

Exercise::Exercise(const QString& name, int weight, int sets, int reps)
    : name(name), weight(weight) ,sets(sets), reps(reps) {}

QString Exercise::getName() const {
    return name;
}

int Exercise::getWeight() const {
    return weight;
}

int Exercise::getSets() const {
    return sets;
}

int Exercise::getReps() const {
    return reps;
}
