#ifndef PLAYBAR_H
#define PLAYBAR_H

#include <QWidget>
#include <QMediaPlayer>
#include <QDebug>
#include <QDialog>

enum playBarErrors {BAD_VOLUME, BAD_PITCH};

namespace Ui {
class playBar;
}

class playBar : public QWidget
{
    Q_OBJECT

    public:
        explicit playBar(QWidget *parent = 0);
        bool fforwardHeld() const;
        bool rewindHeld() const;
        ~playBar();

    private slots:

        void on_skipForward_clicked();

        void on_skipBack_clicked();

        void on_repeat_toggled(bool checked);

        void on_playPause_clicked();

        void on_stopButton_clicked();

        void on_ffoward_pressed();

        void on_rewind_pressed();

        void on_volumeSlider_sliderMoved(int position);

        void on_shuffleButton_clicked();

    signals:

            void playToggle();
            void stop();
            void fforward();
            void rewind();
            void skipForward();
            void skipBack();
            void volumeChanged(int position);
            void shuffle();
            void repeatSet(bool repeat);


private:
    Ui::playBar *ui;

};

#endif // PLAYBAR_H
