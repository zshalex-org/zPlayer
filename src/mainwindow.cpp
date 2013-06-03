#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_formatCtx(NULL),
    m_CodecCtx(NULL)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

//    avcodec_init();
    av_register_all();
    showInputFormat();
    showOutputFormat();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showInputFormat()
{
    AVInputFormat *f = NULL;
    while (true) {
        f = av_iformat_next(f);
        if (f != NULL) {
            ui->inputList->addItem(QString(f->name));
//            f->
        } else {
            break;
        }
    }

    void *p = NULL;
    const char *name;
    while (true) {
        name = avio_enum_protocols(&p,0);
        if (name != NULL) {
            ui->inputProList->addItem(QString(name));
        } else {
            break;
        }
    }
}

void MainWindow::showOutputFormat()
{
    AVOutputFormat *f = NULL;
    while (true) {
        f = av_oformat_next(f);
        if (f != NULL) {
            ui->outputList->addItem(QString(f->name));
//            f->
        } else {
            break;
        }
    }

    void *p = NULL;
    const char *name;
    while (true) {
        name = avio_enum_protocols(&p,1);
        if (name != NULL) {
            ui->outputProList->addItem(QString(name));
        } else {
            break;
        }
    }
}

void MainWindow::on_openBtn_clicked()
{
    const char    *url = "test.mp4";
    AVFormatContext *s = NULL;
    int ret = avformat_open_input(&s, url, NULL, NULL);
    if (ret < 0) {
        qDebug() << "open file error";
    } else {
        qDebug() << "open file success";
        m_formatCtx = s;
        showStreamInfo();
    }
}

void MainWindow::on_closeBtn_clicked()
{
    if (m_formatCtx != NULL) {
        qDebug() << "close file";
        avformat_close_input(&m_formatCtx);
    }
}

void MainWindow::on_readBtn_clicked()
{
    if (m_formatCtx != NULL) {
        if (av_read_frame(m_formatCtx,&m_packet) == 0) {
            if (m_packet.stream_index == 0) {
                qDebug() << m_CodecCtx->codec_id << m_packet.stream_index;
                AVFrame *frame = avcodec_alloc_frame();
                int got_picture_ptr = 0;
                int res = avcodec_decode_video2(m_CodecCtx,frame,&got_picture_ptr,&m_packet);

                if (res >= 0) {
                    qDebug() << "decode success";
                    av_free(frame);
                } else {
                    qDebug() << "decode error";
                }
            }
            av_free_packet(&m_packet);
        } else {
            qDebug() << "read a frame error";
        }
    }
}

void MainWindow::showStreamInfo()
{
    if (m_formatCtx == NULL)
        return;
    int frameRate = 0;
    int frames = 0;
    int time = 0;
    QString timeBase;
    QString name;
    AVCodec *pCodec;
    ui->tableWidget->setRowCount(m_formatCtx->nb_streams);
    for (int i = 0; i < m_formatCtx->nb_streams; i++) {
        pCodec = avcodec_find_decoder(m_formatCtx->streams[i]->codec->codec_id);
        name = pCodec->name;

        if (i == 0) {
            m_CodecCtx = m_formatCtx->streams[i]->codec;
            m_Codec = pCodec;
            qDebug() << "avcodec_open" << avcodec_open(m_CodecCtx,m_Codec);
        }

        if (m_formatCtx->streams[i]->avg_frame_rate.den == 0)
            frameRate = 0;
        else
            frameRate = m_formatCtx->streams[i]->avg_frame_rate.num /
                    m_formatCtx->streams[i]->avg_frame_rate.den;
        frames = m_formatCtx->streams[i]->nb_frames;
        if (frameRate == 0)
            time = 0;
        else
            time = frames / frameRate;

        timeBase = QString("%1 / %2")
                .arg(m_formatCtx->streams[i]->time_base.num)
                .arg(m_formatCtx->streams[i]->time_base.den);

        ui->tableWidget->setItem(i,0,new QTableWidgetItem(name));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(frameRate)));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(frames)));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(time) + "s"));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(timeBase));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(m_formatCtx->streams[i]->start_time)));
        ui->tableWidget->setItem(i,6,new QTableWidgetItem(QString::number(m_formatCtx->streams[i]->duration)));
//        qDebug() << m_formatCtx->streams[m_packet.stream_index]->codec_info_nb_frames;
    }
}
