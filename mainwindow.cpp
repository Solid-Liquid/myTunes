#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QFileDialog>
mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow)
{
    mediaLoaded = false;
    mediaPlaying = false;

    ui->setupUi(this);
    this->setWindowTitle("myTunes");
    ui->status->updateStatusLabel("No Media Selected");
    player = new QMediaPlayer(this);

    lib = new musicLibrary(this);
    ui->scrollArea->setWidget(lib);
    //lib->horizontalHeader()->setStretchLastSection(true);
    //lib->setRowHeight(0,20);

    connect(ui->actionAdd_music,SIGNAL(triggered()),lib,SLOT(addMusic()));
    connect(ui->actionChange_music_directory,SIGNAL(triggered()),lib,SLOT(setDirectory()));
    connect(lib,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(setActiveMedia(int,int)));
    connect(ui->control,SIGNAL(shuffle()),this,SLOT(shuffle()));
    connect(ui->control,SIGNAL(repeatSet(bool)),lib,SLOT(setRepeat(bool)));
    connect(lib,SIGNAL(directoryChanged()),this,SLOT(stopMusic()));

    connect(player,&QMediaPlayer::positionChanged,ui->status,&musicStatus::on_musicSlider_sliderMoved);
    connect(player,&QMediaPlayer::positionChanged,ui->status,&musicStatus::updateCurrentTime);
    connect(ui->status,SIGNAL(positionChanged(qint64)),player,SLOT(setPosition(qint64)));
    connect(player,&QMediaPlayer::durationChanged,ui->status,&musicStatus::setMusicDuration);
    connect(player,&QMediaPlayer::durationChanged,ui->status,&musicStatus::setPlayTime);
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(endOfSong(qint64)));

    connect(ui->control,SIGNAL(stop()),this,SLOT(stopMusic()));
    connect(ui->control,SIGNAL(playToggle()),this,SLOT(playPauseToggle()) );
    connect(ui->control,SIGNAL(fforward()),this,SLOT(fastForward()));
    connect(ui->control,SIGNAL(rewind()),this,SLOT(rewind()));
    connect(ui->control, SIGNAL(volumeChanged(int)), player, SLOT(setVolume(int)));
    connect(ui->control,SIGNAL(skipForward()),this,SLOT(skipForward()));
    connect(ui->control,SIGNAL(skipBack()),this,SLOT(skipBackward()));

    video = new QVideoWidget(this);
}

mainWindow::~mainWindow()
{
    delete ui;
    delete player;
    delete lib;
    delete video;
}

void mainWindow::playPauseToggle()
{
    if(mediaLoaded)
    {
        if(mediaPlaying)
            pauseMusic();
        else
            playMusic();
    }
    else
        if(lib->getMediaCount()>0)
            //setActiveMedia(0,0);
            emit lib->cellDoubleClicked(0,0);
}

void mainWindow::playMusic()
{
    if(player->errorString()=="")
    {
        mediaPlaying=true;
        player->play();
        stringstream ss;
        string temp;
        ss<<lib->getCurrentSongPosition()+1;
        ss>>temp;
        std::string header = std::string("Playing (") + temp+ ")\: ";
        ui->status->updateStatusLabel( QString::fromStdString(header+lib->getCurrentSongTitle()) );
    }
    else
        ui->status->updateStatusLabel("Error: Failed to play media file");

}

void mainWindow::pauseMusic()
{
    player->pause();
    mediaPlaying=false;
    stringstream ss;
    string temp;
    ss<<lib->getCurrentSongPosition()+1;
    ss>>temp;
    std::string header = std::string("Paused (") + temp+ ")\: ";
    ui->status->updateStatusLabel( QString::fromStdString(header+lib->getCurrentSongTitle()) );
}

void mainWindow::stopMusic()
{
    player->stop();
    video->setGeometry(0,0,0,0);
    player->durationChanged(0);
    mediaPlaying = false;
    mediaLoaded = false;
    ui->status->updateStatusLabel("No Media Selected");
}

void mainWindow::endOfSong(qint64 position)
{
    if(position==player->duration() && mediaPlaying)
    {
        int temp = lib->getCurrentSongPosition();
        skipForward();
        if(temp==lib->getCurrentSongPosition())
            stopMusic();
    }
}

void mainWindow::skipForward()
{
    int temp = lib->getCurrentSongPosition();
    lib->nextSong();
    if(temp==lib->getCurrentSongPosition())
        stopMusic();
    else
        setActiveMedia(lib->getCurrentSongPosition(),0);
}

void mainWindow::skipBackward()
{
    if(player->position() <= 5000)
    {
        lib->previousSong();
        setActiveMedia(lib->getCurrentSongPosition(),0);
    }
    else
        player->setPosition(0);
}

void mainWindow::fastForward()
{
    if(mediaLoaded)
    {
        int i = 0;
        bool resume=false;
        qint64 factor = player->duration()*0.01;

        if(mediaPlaying)
        {
            pauseMusic();
            resume = true;
        }

        while(ui->control->fforwardHeld())
        {
            ++i;
            if(i==10000)
            {
                player->setPosition(player->position()+factor);
                i=0;
            }
            QApplication::processEvents();
        }

        if(resume)
            playMusic();
    }
}

void mainWindow::rewind()
{
    if(mediaLoaded)
    {
        bool resume=false;
        int i=0;
        qint64 factor = player->duration()*0.01;

        if(mediaPlaying)
        {
            pauseMusic();
            resume = true;
        }
        while(ui->control->rewindHeld())
        {
            ++i;
            if(i==10000)
            {
                player->setPosition(player->position()-factor);
                i=0;
            }
            QApplication::processEvents();
        }
        if(resume)
            playMusic();
    }
}

void mainWindow::setActiveMedia(int row, int col)
{
    stopMusic();
    if(lib->getMediaCount()>0)
        player->setMedia(lib->getCurrentSongLocation());
    if(player->errorString()=="" && lib->getMediaCount()>0)
    {
        mediaLoaded = true;
        player->setVideoOutput(video);
        video->setGeometry(150,150,720,480);
        video->show();
        playMusic();
    }
    else
    {
        ui->status->updateStatusLabel("Error: Failed to play media file");
        mediaLoaded = false;
    }
}

void mainWindow::shuffle()
{
    if(lib->getMediaCount()>0)
    lib->shuffleMedia();
    setActiveMedia(0,0);
}


