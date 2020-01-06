#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <playbar.h>
#include <musicstatus.h>
#include <QFileDialog>
#include <QGridLayout>
#include <QListWidget>
#include <musiclibrary.h>
#include <QMediaPlaylist>
#include <QString>
#include <QTableWidgetItem>
#include <QVideoWidget>

namespace Ui {
class mainWindow;
}

class mainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit mainWindow(QWidget *parent = 0);
        ~mainWindow();

    public slots:
        void stopMusic();
        void skipForward();
        void skipBackward();
        void fastForward();
        void rewind();
        void playPauseToggle();
        void setActiveMedia(int row, int col);
        void shuffle();

    signals:

    private slots:
        void playMusic();
        void pauseMusic();
        void endOfSong(qint64 position);

    private:
            Ui::mainWindow *ui;
            musicLibrary *lib;
            QMediaPlayer *player;
            QGridLayout *layout;
            bool mediaPlaying;
            bool mediaLoaded;
            QVideoWidget* video;
            //musicStatus object is named "status"
            //playBar object is named "control"
};

#endif // MAINWINDOW_H
