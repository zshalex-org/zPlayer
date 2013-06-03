#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

extern "C" {
#include "ffmpeg/libavformat/avformat.h"
#include "ffmpeg/libavcodec/avcodec.h"
#include "ffmpeg/libavutil/avutil.h"
#include "ffmpeg/libavutil/mem.h"
#include "ffmpeg/libavcodec/avcodec.h"
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_openBtn_clicked();

    void on_closeBtn_clicked();

    void on_readBtn_clicked();

private:
    void showInputFormat();
    void showOutputFormat();
    void showStreamInfo();
private:
    Ui::MainWindow *ui;
    AVFormatContext *m_formatCtx;
    AVPacket m_packet;
    AVCodecContext *m_CodecCtx;
    AVCodec *m_Codec;
};

#endif // MAINWINDOW_H
