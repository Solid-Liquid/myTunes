#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QUrl>
#include <QTableWidget>
#include <QListWidget>
#include <QString>
#include <QTableWidgetItem>
#include <QApplication>
#include <QDesktopWidget>
#include <QObject>
#include <QtCore/QCoreApplication>
#include <iostream>
#include <QFileDialog>
#include <vector>
#include <string>
#include <algorithm>
#include <sys/types.h>
#include <QDebug>
#include <sstream>
#include <QTableWidget>
#include <QRect>
#include <dirent.h>
#include <fstream>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QHeaderView>
#include <QMediaPlayer>
#include <QTime>

using namespace std;

class musicLibrary: public QTableWidget
{
    Q_OBJECT

    public:
        musicLibrary(const musicLibrary &other);
        musicLibrary(QWidget *parent = 0, std::string dir = "music", int row = 0, int col = 0, int w = 10, int h = 10);
        ~musicLibrary();
        musicLibrary& operator=(const musicLibrary &other);       
        string getDirectory() const;
        int getCurrentSongPosition() const;
        string getCurrentSongTitle() const;
        QUrl getCurrentSongLocation() const;
        int getMediaCount() const;

    public slots:
        void addMusic();
        void setDirectory();
        void shuffleMedia();
        void toggleRepeat();
        void setRepeat(bool repeatVal);
        void nextSong();
        void previousSong();

    signals:
        void directoryChanged();

    private slots:
        void updateCurrentSong(int row, int col);

    private:
        std::string libraryPath;
        int currentSong;
        QMediaPlaylist *playlist;
        bool repeat;

        void copy(const musicLibrary &other);
        void initialize();
        void read_directory(const string& path);
        void loadTable();
        QString toPlaytimeFormat(qint64 time);
};

#endif // MUSICLIBRARY_H
