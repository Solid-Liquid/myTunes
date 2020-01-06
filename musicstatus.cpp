#include "musicstatus.h"
#include "ui_musicstatus.h"

musicStatus::musicStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::musicStatus)
{
    max=0;
    ui->setupUi(this);
    ui->lengthLCD->display("00:00:00");
    ui->currentTimeLCD->display("00:00:00");
}

musicStatus::~musicStatus()
{
    delete ui;
}

void musicStatus::on_musicSlider_sliderMoved(int position)
{
    if(!ui->musicSlider->isSliderDown())
        ui->musicSlider->setValue(position);
}

void musicStatus::setMusicDuration(int duration)
{
    ui->musicSlider->setMaximum(duration);
}

void musicStatus::on_musicSlider_sliderReleased()
{
    emit positionChanged(ui->musicSlider->value());
}

void musicStatus::setPlayTime(qint64 time)
{
//    double setTime = (double)time;
//    setTime = setTime/60000;
    max=time;
    ui->lengthLCD->display(toPlaytimeFormat(time));
}

void musicStatus::updateCurrentTime(qint64 time)
{
//    double setTime = (double)time;
//    setTime = setTime/60000;
    if(time<=max)
        ui->currentTimeLCD->display(toPlaytimeFormat(time));
}

QString musicStatus::toPlaytimeFormat(qint64 time)
{
    int seconds = (time/1000) % 60;
    int minutes = (time/60000) % 60;
    int hours = (time/3600000) % 24;
    QTime format(hours, minutes,seconds);
    return format.toString();
}

void musicStatus::updateStatusLabel(QString status)
{
    ui->statusLabel->setText(status);
}
