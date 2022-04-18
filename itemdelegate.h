#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPainterPath>

class ArticleInfo{
    public:
        QString title;
        QString imgurl;
        QString content;
        QString date;
        QPixmap ArticleImage;
        bool isload;
};

class ItemDelegate:public QStyledItemDelegate
{
public:
    ItemDelegate(QObject *parent = 0);

    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ITEMDELEGATE_H
