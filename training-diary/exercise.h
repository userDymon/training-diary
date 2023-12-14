#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>

class Exercise {
public:
    Exercise(const QString& name, int weight, int sets, int reps);

    QString getName() const;
    int getWeight() const;
    int getSets() const;
    int getReps() const;

private:
    QString name;
    int weight;
    int sets;
    int reps;
};

#endif // EXERCISE_H
