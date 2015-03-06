#ifndef COMPONENT_H
#define COMPONENT_H

#include <QDebug>
#include <QString>

class CircuitBoard;

class Component
{
public:
    Component():unique(false){}
    QString name(){return componentName;}
    virtual
        void setName(QString s){componentName = s;}
    virtual
        void deleteKeyAction(){}
    void setUnique(bool b){unique = b;}
    bool operator<(Component &c){return (unique && name() != c.name())?(componentName < c.componentName):((long long)(this) < (long long)(&c));}
    bool operator>(Component &c){return (unique && name() != c.name())?(componentName > c.componentName):((long long)(this) > (long long)(&c));}
    bool operator==(Component &c){return (long long)(this) == (long long)(&c);}
    //for easily matching by name
    bool operator%(Component &c){return (unique)?(componentName == c.componentName):((long long)(this) == (long long)(&c));}
    friend
        QDebug operator<<(QDebug dbg, Component& c){dbg.nospace() << &c << " " << c.name(); return dbg.space();}
protected:
    CircuitBoard* board;
private:
    QString componentName;
    bool unique;
};

#endif // COMPONENT_H
