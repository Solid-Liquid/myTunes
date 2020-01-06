#ifndef MUSICSTATUS_H
#define MUSICSTATUS_H

#include <QWidget>
#include <QDebug>
#include <QTime>

namespace Ui {
class musicStatus;
}

class musicStatus : public QWidget
{
    Q_OBJECT

    public:
        explicit musicStatus(QWidget *parent = 0);
        ~musicStatus();

    public slots:
        void on_musicSlider_sliderMoved(int position);
        void setMusicDuration(int duration);
        void setPlayTime(qint64 time);
        void updateCurrentTime(qint64 time);
        void updateStatusLabel(QString status);

    signals:
        void positionChanged(qint64 position);

private slots:
        void on_musicSlider_sliderReleased();

private:
        Ui::musicStatus *ui;
        QString toPlaytimeFormat(qint64 time);
        qint64 max;
};

#endif // MUSICSTATUS_H
