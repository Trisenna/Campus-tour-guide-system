#ifndef CODE_H
#define CODE_H

#include <QString>



class code {
  public:
    code() {};
    code(int x, int y, int index, QString name, QString des) {
        this->x = x;
        this->y = y;
        this->index = index;
        this->name = name;
        this->description = des;
    }
    int x;
    int y;
    int index;
    QString  name;
    QString description;
};

#endif // CODE_H
