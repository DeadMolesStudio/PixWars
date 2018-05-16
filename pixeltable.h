#ifndef PIXELTABLE_H
#define PIXELTABLE_H

#include <QObject>
#include <QQmlListProperty>
#include <qpix.h>

class PixelTable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Qpix> data READ data NOTIFY dataChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    PixelTable(QObject *parent = nullptr);

    QQmlListProperty<Qpix> data();

    Q_INVOKABLE void add(unsigned int id);
    Q_INVOKABLE void setQpixById(unsigned int id, const unsigned new_color);
    Q_INVOKABLE QString unblockQpixIn(unsigned int id);

    int count() const;

signals:
    void dataChanged();
    void countChanged();

public slots:

private:
    static void appendData(QQmlListProperty<Qpix> *list, Qpix *value);
    static int countData(QQmlListProperty<Qpix> *list);
    static Qpix *atData(QQmlListProperty<Qpix> *list, int index);
    static void clearData(QQmlListProperty<Qpix> *list);

    QList<Qpix*> listPixels;
};

#endif // PIXELTABLE_H
