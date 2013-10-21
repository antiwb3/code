#ifndef QSINGLETON_H_
#define QSINGLETON_H_

class QWidget;

template<class T, class Base>
class QSingleton : public Base
{
public:
    QSingleton(QWidget *parent);
    QSingleton(){};
    virtual ~QSingleton(){};

public:
    static T *Instance();
    static T *Instance(QWidget *parent);
    static void Release();
private:
    static T *ms_pObject;
};

template<class T, class Base>
QSingleton<T, Base>::QSingleton(QWidget *parent)
: Base(parent)
{
    
}

template<class T, class Base>
T *QSingleton<T, Base>::ms_pObject = NULL;

template<class T, class Base>
T *QSingleton<T, Base>::Instance(QWidget *parent)
{
    if (!ms_pObject)
    {
        ms_pObject = new T(parent);
    }
    else
    {
        ms_pObject->setParent(parent);
    }
    return ms_pObject;
}

template<class T, class Base>
T *QSingleton<T, Base>::Instance()
{
    if (!ms_pObject)
    {
        ms_pObject = new T();
    }
    return ms_pObject;
}

template<class T, class Base>
void QSingleton<T, Base>::Release()
{
    SAFE_DELETE(ms_pObject);
}

#endif //QSINGLETON_H_