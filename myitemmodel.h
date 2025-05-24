#ifndef MYITEMMODEL_H
#define MYITEMMODEL_H
#include <QObject>
#include <QStandardItemModel>

class MyItemModel :public QStandardItemModel
{
    Q_OBJECT
public:
    // MyItemModel();
    using QStandardItemModel::QStandardItemModel;
    QHash<int, QByteArray>roleNames()const override{
        QHash<int, QByteArray> roles;
        roles[Qt::DisplayRole]="name";
        roles[Qt::UserRole+1]="id";
        return roles;
    }
};

#endif // MYITEMMODEL_H
