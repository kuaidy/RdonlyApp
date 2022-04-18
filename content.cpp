#include "content.h"
#include "ui_content.h"

Content::Content(const QModelIndex &index,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Content)
{
    ui->setupUi(this);

//    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    articleinfo=index.data(Qt::UserRole+1).value< ArticleInfo *>();
    //QStringList lstcontent=articleinfo->content.split("\n",Qt::SkipEmptyParts);
    //for(int i=0;i<lstcontent.count();i++){
//        lstcontent[i]=lstcontent[i].replace(rx,"<img src=\"data:image/png;base64,"+imagedata+"\">");
//        ui->textBrowser->append(lstcontent[i]);
        QRegularExpression rx;
        rx.setPattern("<figure(.*?)src=\\\"(.*?)\\\"(.*?)figure>");
        //rx.setPattern("src=\\\"(.*?)\\\"");
//        QRegularExpressionMatch match = rx.match(articleinfo->content);
        QRegularExpressionMatchIterator matchi = rx.globalMatch(articleinfo->content);

        if(matchi.hasNext()){
            while (matchi.hasNext()) {
                QRegularExpressionMatch match = matchi.next();
                //获取图片信息
                QString imagesrc = match.captured(2).replace("https","http");
                if(!QString(imagesrc).isEmpty()){
                    QNetworkRequest request;
                    // 发送https请求前准备工作;
                    QSslConfiguration config;
                    QSslConfiguration conf = request.sslConfiguration();
                    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
                    conf.setProtocol(QSsl::TlsV1_3OrLater);
                    request.setSslConfiguration(conf);

                    QNetworkAccessManager *netmanager=new QNetworkAccessManager(this);
                    request.setUrl(QUrl(imagesrc));
                    netmanager->get(request);
                    connect(netmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(SlotReplyArticle(QNetworkReply*)));
                }
            }
        }else{
            ui->textBrowser->setHtml("<h1>"+articleinfo->title+"</h1>"+articleinfo->content.remove("<br/>").replace("<img","<br/><img"));
        }
}

Content::~Content()
{
    delete ui;
}

void Content::on_ConBack_clicked()
{
    emit(Signal_ConBack_Clicked());
    this->hide();
}

void Content::SlotReplyArticle(QNetworkReply* reply)
{
    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());
    int imgwidth=pixmap.width();
    int imgheight=pixmap.height();
    int tarwidth=ui->textBrowser->width()-10;
    if(imgwidth!=0){
        int tarheight=(int)(tarwidth*imgheight)/imgwidth;
        pixmap=pixmap.scaled(tarwidth,tarheight,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QByteArray byte;
    QBuffer buf(&byte);
    buf.open(QIODevice::WriteOnly);
    pixmap.save(&buf, "PNG");
    QString imagedata= byte.toBase64();
    buf.close();

//    articleinfo->content=articleinfo->content.remove("\n");

//    qDebug()<<reply->url();
//    qDebug()<<articleinfo->content;
    QString imgurl=reply->url().toString().replace("http","https");
//    qDebug()<<imgurl;
    QRegularExpression rx;
    rx.setPattern("<figure(.*?)src=\\\""+imgurl+"\\\"(.*?)figure>");
//    ui->textBrowser->setText(articleinfo->content.replace(rx,"<img src=\"data:image/png;base64,"+imagedata+"\">"));
//    ui->textBrowser->setHtml("<img src=\"data:image/png;base64,"+imagedata+"\">");
//    ui->textBrowser->setHtml(articleinfo->content.remove(rx));
//    ui->textBrowser->append("<img src=\"data:image/png;base64,"+imagedata+"\">");

    articleinfo->content=articleinfo->content.replace(rx,"<img src=\"data:image/png;base64,"+imagedata+"\">");
//    qDebug()<<articleinfo->content;
    rx.setPattern("<figure(.*?)figure>");
    int index=articleinfo->content.indexOf(rx);
    if(index==-1){
        //直接增加br换行就不会乱
        ui->textBrowser->setHtml("<h1>"+articleinfo->title+"</h1>"+articleinfo->content.remove("\n").remove("<br/>").replace("<img","<br/><img"));
        ui->textBrowser->moveCursor(QTextCursor::Start);

        //        QStringList lstarticle=articleinfo->content.split("\n",Qt::SkipEmptyParts);
        //        for(int i=0;i<lstarticle.count();++i){
        //            qDebug()<<lstarticle[i];
        //            lstarticle[i]=lstarticle[i].remove("<ul>").remove("</ul>").remove("</li>").replace("<li>","\n");
        //            ui->textBrowser->append(lstarticle[i]);

        //        }
    }
}

