#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QList>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QAbstractItemView>
#include "itemdelegate.h"
#include "content.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    ItemDelegate *m_delegate;
    QStandardItemModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    ArticleInfo *m_articleinfo;

    int page=1;

private slots:
    void SlotReplyArticle(QNetworkReply* reply);
    void SlotReplyPixmap(QNetworkReply* reply);

    void on_pushButton_clicked();

    void initData();
    void on_listView_clicked(const QModelIndex &index);
};
#endif // MAINWINDOW_H
