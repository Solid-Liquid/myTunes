#include "musiclibrary.h"

musicLibrary::musicLibrary(const musicLibrary &other)
{
    copy(other);
    initialize();
}

musicLibrary::musicLibrary(QWidget *parent, std::string dir,  int row , int col, int w , int h)
{
    playlist = new QMediaPlaylist();
    currentSong = 0;
    repeat = false;
    QTableWidget::setParent(parent);
    setGeometry(row,col,w,h);
    ifstream in;
    in.open("userlib.cfg");
    if(in.fail())
    {
        QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Welcome to myTunes", "No music folder has been set. Please select the folder you would like to use for storing music, or one that already contains music.", QMessageBox::Ok).exec();
        setDirectory();
    }
    else
    {
        getline(in,libraryPath);
        in.close();
        initialize();
    }
    connect(this,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(updateCurrentSong(int,int)));
    show();
}

musicLibrary::~musicLibrary()
{
    delete playlist;
}

musicLibrary& musicLibrary::operator=(const musicLibrary &other)
{
    if(this != &other)
        copy(other);
    return *this;
}

void musicLibrary::copy(const musicLibrary &other)
{
    libraryPath = other.libraryPath;
    currentSong = other.currentSong;
    playlist = other.playlist;
}

void musicLibrary::initialize()
{
    QStringList tableHeader;
    setRowCount(0);
    read_directory(libraryPath);
    tableHeader<<"Title"<<"Artist"<<"Album"<<"Length"<<"Genre"<<"Location";
    verticalScrollBar();
    setHorizontalHeaderLabels(tableHeader);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setShowGrid(true);
    setStyleSheet("QTableView {selection-background-color: blue;}");
    setGeometry(QApplication::desktop()->screenGeometry());
    //sortItems(0);
    //resizeRowsToContents();
    //resizeColumnsToContents();
    horizontalHeader()->setStretchLastSection(true);
    //horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    selectRow(0);
}

void musicLibrary::setDirectory()
{
    ofstream out;
    ifstream in;
    out.open("userlib.cfg");
    QString dir = QFileDialog::getExistingDirectory(this, QFileDialog::tr("Set Folder Containing Music"),"",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir.toStdString()=="")
    {
        if(libraryPath=="")
        {
            dir = QDir().mkdir("myTunes_Music");
            dir = "myTunes_Music";
        }
        else
            dir = QString::fromStdString(libraryPath);
    }
    out<<dir.toStdString();
    out.close();
    in.open("userlib.cfg");
    getline(in,libraryPath);
    in.close();
    initialize();
}

string musicLibrary::getDirectory() const
{
    return libraryPath;
}

void musicLibrary::read_directory( const string& path)
{
  //string message;
  //vector<string> files;
  dirent* dirEntry;
  DIR* dp;
  int largest = 0;
  errno = 0;
  string thePath("Path ");
  thePath+=path;
  qDebug(thePath.c_str());
  dp = opendir( path.empty() ? "." : path.c_str() );
  if (dp)
  {
    playlist->clear();
    while (true)
    {
      errno = 0;
      dirEntry = readdir( dp );
      if (dirEntry == NULL)
          break;
      if(string(dirEntry->d_name) == "." || string(dirEntry->d_name) == "..")
          continue;
      playlist->addMedia(QUrl(QString::fromStdString(libraryPath+"/"+dirEntry->d_name)));
      if(string(dirEntry->d_name).size() > largest)
          largest = string(dirEntry->d_name).size();
    }
    closedir( dp );
    setColumnCount(6);
    setRowCount(playlist->mediaCount());
  }
  loadTable();
  emit directoryChanged();
}

void musicLibrary::addMusic()
{
    QFileDialog d;
    QString fileName = d.getOpenFileName(this,
         tr("Open Music File"), "", tr("Music Files (*.mp3 *.wav *.m4a *.mp4)"));
    string whatToDo;
    whatToDo = "copy \""+fileName.toStdString() + "\"  \""+libraryPath+"\"";
    for(int i=0;i<whatToDo.size();++i)
    {
        if(whatToDo[i]=='/')
            whatToDo[i]='\\';
    }

    std::cout<<whatToDo<<endl;
    system(whatToDo.c_str());
    initialize();
}

void musicLibrary::updateCurrentSong(int row, int col)
{
    currentSong = row;
    playlist->setCurrentIndex(row);
}

int musicLibrary::getCurrentSongPosition() const
{
    return currentSong;
}

string musicLibrary::getCurrentSongTitle() const
{
    return item(currentSong,0)->text().toStdString();
}

QUrl musicLibrary::getCurrentSongLocation() const
{
    return playlist->media(currentSong).canonicalUrl();
}

void musicLibrary::loadTable()
{
    //QMediaPlayer* durationCheck;
    //durationCheck = new QMediaPlayer();
    for(int i = 0; i < playlist->mediaCount(); ++i)
    {
        QString path = playlist->media(i).canonicalUrl().toString();
        //durationCheck->setMedia(QUrl::fromLocalFile(path));
        string name = path.toStdString();
        name = name.substr(name.find_last_of("/")+1);
        setItem(i,0, new QTableWidgetItem(QString::fromStdString(name)));
        setItem(i,1, new QTableWidgetItem("Artist Title"));
        setItem(i,2, new QTableWidgetItem("Album Title"));
        setItem(i,3, new QTableWidgetItem("Length"));
        setItem(i,4,new QTableWidgetItem("Genre"));
        setItem(i,5, new QTableWidgetItem(path));
        //durationCheck->stop();
    }
    //delete durationCheck;
}

QString musicLibrary::toPlaytimeFormat(qint64 time)
{
    int seconds = (time/1000) % 60;
    int minutes = (time/60000) % 60;
    int hours = (time/3600000) % 24;
    QTime format(hours, minutes,seconds);
    return format.toString();
}

void musicLibrary::shuffleMedia()
{
    playlist->shuffle();
    loadTable();
    currentSong=0;
}

void musicLibrary::toggleRepeat()
{
    if(repeat)
        repeat=false;
    else
        repeat=true;
}

void musicLibrary::setRepeat(bool repeatVal)
{
    repeat = repeatVal;
}

void musicLibrary::nextSong()
{
    if(playlist->currentIndex()+1<playlist->mediaCount())
       playlist->setCurrentIndex(playlist->currentIndex()+1);
    else if(playlist->currentIndex()+1>=playlist->mediaCount() && repeat==true)
        playlist->setCurrentIndex(0);
    currentSong = playlist->currentIndex();
}

void musicLibrary::previousSong()
{
    if(playlist->currentIndex()-1>=0)
    {
       playlist->setCurrentIndex(playlist->currentIndex()-1);
       currentSong = playlist->currentIndex();
    }

//    else if(playlist->currentIndex()+1>=playlist->mediaCount() && repeat==true)
//        playlist->setCurrentIndex(0);
}

int musicLibrary::getMediaCount() const
{
    return playlist->mediaCount();
}
