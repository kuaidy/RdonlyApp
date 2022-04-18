#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model=new QStandardItemModel();

    QNetworkRequest request;
    //直接获取网站数据
    QNetworkAccessManager *netmanager=new QNetworkAccessManager(this);
    request.setUrl(QUrl("http://www.rdonly.com/wp-json/wp/v2/posts"));
    netmanager->get(request);
    connect(netmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(SlotReplyArticle(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotReplyArticle(QNetworkReply* reply)
{
    // 获取http状态码
//    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//    if(statusCode.isValid()){}
        //qDebug() << "status code=" << statusCode.toInt();

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        //qDebug() << "Failed: " << reply->errorString();
    }
    else {
        QJsonDocument document=QJsonDocument::fromJson(reply->readAll());
        QJsonArray arr= document.array();

        for(int i=0;i<arr.count();i++){
            m_articleinfo=new ArticleInfo();
            QJsonObject objartile=arr[i].toObject();
            m_articleinfo->title=objartile.take("title").toObject().take("rendered").toString();
            m_articleinfo->date=objartile.take("date").toString();
            m_articleinfo->content=objartile.take("content").toObject().take("rendered").toString();
            QVariant variant;
            variant.setValue(m_articleinfo);
            QStandardItem *item = new QStandardItem;
            item->setData(QVariant::fromValue(variant),Qt::UserRole+1);
            m_model->appendRow(item);
            //获取网络图片
//            QNetworkAccessManager *imgmanager=new QNetworkAccessManager();
//            imgmanager->get(QNetworkRequest(QUrl(arr[i].toString())));
//            connect(imgmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(SlotReplyPixmap(QNetworkReply*)));
        }
        m_articleinfo=new ArticleInfo();
        m_articleinfo->title="--点击加载更多内容--";
        m_articleinfo->isload=true;
        QVariant variant;
        variant.setValue(m_articleinfo);
        QStandardItem *item = new QStandardItem;
        item->setData(QVariant::fromValue(variant),Qt::UserRole+1);
        m_model->appendRow(item);

        m_delegate = new ItemDelegate(this);
        //为视图设置委托
        ui->listView->setItemDelegate(m_delegate);
        m_proxyModel = new QSortFilterProxyModel(ui->listView);
        m_proxyModel->setSourceModel(m_model);
        m_proxyModel->setFilterRole(Qt::UserRole);
        m_proxyModel->setDynamicSortFilter(true);
        //为委托设置模型
        ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->listView->setModel(m_proxyModel);
    }
}

void MainWindow::initData(){
    this->show();
}

void MainWindow::SlotReplyPixmap(QNetworkReply* ImgReply){
    QPixmap pixmap;
    pixmap.loadFromData(ImgReply->readAll());

    m_articleinfo=new ArticleInfo();
    m_articleinfo->title="测试标题";
    m_articleinfo->content="测试内容";
    m_articleinfo->imgurl="https://www.rdonly.com/wp-content/uploads/2021/05/2021-5-18-1621348550470.png";
    m_articleinfo->ArticleImage=pixmap;

    QVariant variant;
    variant.setValue(m_articleinfo);
    QStandardItem *item = new QStandardItem;
    item->setData(QVariant :: fromValue(variant),Qt::UserRole+1);
    m_model->appendRow(item);
}

void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    ArticleInfo *article=index.data(Qt::UserRole+1).value< ArticleInfo *>();
    if(article->isload){
        m_model->removeRow(index.row());
        page++;
        QNetworkAccessManager *netmanager=new QNetworkAccessManager(this);
        QNetworkRequest request;
        QString url=QString("http://www.rdonly.com/wp-json/wp/v2/posts?page=%1").arg(page);
        request.setUrl(QUrl(url));
        netmanager->get(request);
        connect(netmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(SlotReplyArticle(QNetworkReply*)));
    }else{
        this->hide();
        Content *ContentWin=new Content(index,nullptr);
        connect(ContentWin, SIGNAL(Signal_ConBack_Clicked()),this,SLOT(initData()));
        ContentWin->show();
    //    ui->scrollArea->setWidget(test);
    }

}

