#ifndef CONTENT_H
#define CONTENT_H

#include <QFrame>
#include <QModelIndex>
#include <QJsonObject>
#include <QRegularExpression>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QBuffer>
#include <QSslSocket>
#include <QSsl>
#include "itemdelegate.h"

namespace Ui {
class Content;
}

class Content : public QFrame
{
    Q_OBJECT

public:
    explicit Content(const QModelIndex &index,QWidget *parent = nullptr);
    ~Content();

    ArticleInfo *articleinfo;
    QString strarticle;

signals:
    void Signal_ConBack_Clicked();

public slots:
    void on_ConBack_clicked();
    void SlotReplyArticle(QNetworkReply *reply);

private:
    Ui::Content *ui;
};

#endif // CONTENT_H
