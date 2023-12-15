#include "exercise.h"

Exercise::Exercise(const QString& name, int weight, int sets, int reps)
    : name(name), weight(weight) ,sets(sets), reps(reps) {}

Exercise::Exercise(const Exercise& other): name(other.getName()), weight(other.getWeight()) ,sets(other.getSets()), reps(other.getReps()) {}

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
