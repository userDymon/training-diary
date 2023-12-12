#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>

class Exercise {
public:
    Exercise(const QString& name, int sets, int repsPerSet);

    QString getName() const;
    int getSets() const;
    int getRepsPerSet() const;

private:
    QString name;
    int sets;
    int repsPerSet;
};

#endif // EXERCISE_H
