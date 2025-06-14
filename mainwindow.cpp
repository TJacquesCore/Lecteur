#include "mainwindow.h"
#include "ui_mainwindow.h"



std::unordered_set<std::string> set{".mp3",".wav",".flac",".aac",".m4a",".opus"};


bool is_a_muisc_file(const std::string& path)
{
    //fonction pour verifier si c'est un ficher audio
    if(set.find(path)!=set.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::play_music_by_button()
{
    QString path= ui->playlistWidget->currentItem()->text();

    path=QString::fromStdString(location)+"/"+path;

    qDebug()<<"path est :"<<path;
    player->stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    player->setSource(QUrl::fromLocalFile(path));
    ui->labelTitle->setText(ui->playlistWidget->currentItem()->text());
    player->play();

}

void MainWindow::play_music_by_list(QListWidgetItem* item)
{
    //fonction pour jouer la music;
    QString path= item->text();
    path=QString::fromStdString(location)+"/"+path;

    qDebug()<<"path est :"<<path;
    player->stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    player->setSource(QUrl::fromLocalFile(path));
    ui->labelTitle->setText(item->text());
    player->play();
}

std::string MainWindow::reload_location()
{
    //juste hisoire de pas avoir a reselectionner encore et encore une location a chaque fois qu'on ouvre ce putin de programme

    std::ifstream emplacement_de_choix{"location.txt"};
    if(emplacement_de_choix.is_open())
    {
        std::string passager;
        std::getline(emplacement_de_choix,passager);
        if(passager.empty())
        {
            qDebug()<<"ahhh merde le ficher n'a pas ete charge";
            QString musicDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
            std::string secours=musicDir.toStdString();
            emplacement_de_choix.close();
            return secours;
        }
        return passager;
    }

    else
    {
        qDebug()<<"ahhh merde le ficher n'a pas ete charge";
      QString musicDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
        std::string secours=musicDir.toStdString();
      emplacement_de_choix.close();
        return secours;
    }


}

void MainWindow::refresh()
{
    //histoire de rafraichir un peu la liste;


    qDebug()<<"La location est: "<<location;

    std::filesystem::path path =location;
    ui->playlistWidget->clear();
    qDebug()<<"a l'entre de la boucle for";


    for(auto& entry: std::filesystem::directory_iterator(path))
    {

        if(entry.is_regular_file())
        {

            if(is_a_muisc_file(entry.path().extension().string()))
            {

                ui->playlistWidget->addItem(QString::fromStdString(entry.path().filename().string()));
            }
        }
    }

}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player->setAudioOutput(output);
    location=reload_location();
    refresh(); //raffraichir


    QObject::connect(ui->btnNext,&QPushButton::clicked,this,[&]()
                     {
        //avancer de une music
        if(ui->playlistWidget->count()>0)
            {
        ui->playlistWidget->setCurrentRow((ui->playlistWidget->currentRow()+1)%ui->playlistWidget->count());//de cette facon on fait une boucle circulaire
             play_music_by_button();
        }



    });

    QObject::connect(ui->btnPrevious,&QPushButton::clicked,this,[&]()
                     {
          if(ui->playlistWidget->count()>0)
              {

          ui->playlistWidget->setCurrentRow((ui->playlistWidget->currentRow()-1+ui->playlistWidget->count())%ui->playlistWidget->count());//de cette facon on fait une boucle circulaire
                         play_music_by_button();
          }

                     });
    QObject::connect(player,&QMediaPlayer::positionChanged,ui->progressSlider,&QSlider::setValue);
    QObject::connect(player,&QMediaPlayer::durationChanged,ui->progressSlider,&QSlider::setMaximum);
    QObject::connect(ui->progressSlider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);
    QObject::connect(ui->btnRaffraichir,&QPushButton::clicked,this,[&]()
                     {
        refresh();
    });
    QObject::connect(ui->playlistWidget,&QListWidget::itemClicked,this, &MainWindow::play_music_by_list);
    QObject::connect(ui->btnPlay,&QPushButton::clicked,this,[&]()
                     {
        if(player->isPlaying())
            {
            player->pause();
        }

        else
            {
            player->play();
        }
    });

    QObject::connect(ui->btnAdd, &QPushButton::clicked,this,[&]()

                     {
                         //on fait tout on prend le dossier on affiche tout les ficher de music dans le listwidget
                         QString file = QFileDialog::getExistingDirectory(this,"Selectionner un dossier",QDir::homePath(),QFileDialog::ShowDirsOnly);
                         if(!file.isEmpty())
                             {
                             //flux temporaire pour save la last location en mm temps
                              std::ofstream emplacement_de_choix{"location.txt"};
                             if(emplacement_de_choix.is_open())
                              {
                                  emplacement_de_choix<<file.toStdString();
                                 emplacement_de_choix.close();
                                  location=file.toStdString();
                                 refresh();
                             }
                             else
                             {
                                 qDebug()<<"ya un truc pas clair qui s'est passe au niveau de la lecture de location.txt qui save les last location";
                             }
                         }


                         refresh();


                     });



    QApplication::setWindowIcon(QIcon(":/icons/myicon.ico"));



}

MainWindow::~MainWindow()
{
    delete ui;
}
