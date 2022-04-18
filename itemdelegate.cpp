#include "itemdelegate.h"

ItemDelegate::ItemDelegate(QObject *parent):QStyledItemDelegate(parent)
{

}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.isValid())
    {
        ArticleInfo *articleinfo=index.data(Qt::UserRole+1).value< ArticleInfo *>();

//        qDebug()<<index.column();

        int x = option.rect.x();
        int y = option.rect.y();
        int w = option.rect.width();
        int h = option.rect.height();
        painter->save();

        QPen pen;
        pen.setColor(QColor(Qt::black));
        painter->setPen(pen);

        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setLetterSpacing(QFont::AbsoluteSpacing,0);
        font.setPixelSize(18);
        painter->setFont(font);

        if(articleinfo->isload){
//            int widthOfTitle = painter->fontMetrics().width(articleinfo->title);//字符串显示的像素大小
//            painter->drawText(w-widthOfTitle/2,100,articleinfo->title);
            QRect rect;
            //加载按钮
            painter->drawText(x+15,y,w-30,h-30,Qt::AlignHCenter|Qt::AlignVCenter,articleinfo->title,&rect);
//            painter->drawRect(x+15,y,w-30,h-30);
//            painter->fillRect(x,y,w,h,QColor(Qt::red));
        }else{
            painter->drawText(x,y+30,articleinfo->title);
            pen.setColor(QColor(Qt::gray));
            font.setPixelSize(12);
            painter->setFont(font);
            painter->drawText(x,y+50,articleinfo->date);
//            painter->drawLine(x,y,x+w,y);
        }
        painter->restore();

    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 80);
}
