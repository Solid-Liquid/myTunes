#include "playbar.h"
#include "ui_playbar.h"


playBar::playBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::playBar)
{
    ui->setupUi(this);
}

playBar::~playBar()
{
    delete ui;
}

void playBar::on_skipForward_clicked()
{
    emit skipForward();
}

void playBar::on_skipBack_clicked()
{
    emit skipBack();
}

void playBar::on_playPause_clicked()
{
    emit playToggle();
}

void playBar::on_stopButton_clicked()
{
    emit stop();
}

void playBar::on_repeat_toggled(bool checked)
{
    emit repeatSet(checked);
}

void playBar::on_ffoward_pressed()
{
    emit fforward();
}

bool playBar::fforwardHeld() const
{
    return ui->ffoward->isDown();
}

void playBar::on_rewind_pressed()
{
    emit rewind();
}

bool playBar::rewindHeld() const
{
    return ui->rewind->isDown();
}

void playBar::on_volumeSlider_sliderMoved(int position)
{
   if (position < 0 || position > 100)
      throw BAD_VOLUME;
   emit volumeChanged(position);

}

void playBar::on_shuffleButton_clicked()
{
    emit shuffle();
}
