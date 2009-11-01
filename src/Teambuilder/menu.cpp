#include "menu.h"
#include "mainwindow.h"
#include "../PokemonInfo/pokemoninfo.h"
#include "../Utilities/otherwidgets.h"

TB_Menu::TB_Menu()
        : QImageBackground("db/menu_background.png")
{
    setWindowTitle(tr("Menu"));

    QVBoxLayout *layout = new QVBoxLayout(this);

    QImageButton *teambuilder, *online, *credits, *exit;

    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(teambuilder = new QImageButton("db/Teambuilder0.png", "db/Teambuilder1.png"), 0, Qt::AlignCenter);
    layout->addWidget(online = new QImageButton("db/GoOnline0.png", "db/GoOnline1.png"), 0, Qt::AlignCenter);
    layout->addWidget(credits = new QImageButton("db/Credits0.png", "db/Credits1.png"), 0, Qt::AlignCenter);
    layout->addWidget(exit = new QImageButton("db/Quit0.png", "db/Quit1.png"), 0, Qt::AlignCenter);

    connect (teambuilder, SIGNAL(clicked()), SLOT(launchTeambuilder()));
    connect (online, SIGNAL(clicked()), SLOT(goOnline()));
    connect (credits, SIGNAL(clicked()), SLOT(launchCredits()));
    connect (exit, SIGNAL(clicked()), SLOT(exit()));
}

QMenuBar * TB_Menu::createMenuBar(MainWindow *w)
{
/*    QMenuBar *menuBar = new QMenuBar();
    QMenu *menuFichier = menuBar->addMenu("&File");
    menuFichier->addAction(tr("&Load Team"),w,SLOT(loadTeamDialog()),Qt::CTRL+Qt::Key_L);
    menuFichier->addAction(tr("&Quit"),w,SLOT(close()),Qt::CTRL+Qt::Key_Q);

    return menuBar; */
    (void) w;
    return NULL;
}

void TB_Menu::launchCredits()
{
    emit goToCredits();
}

void TB_Menu::launchTeambuilder()
{
    emit goToTeambuilder();
}

void TB_Menu::goOnline()
{
    emit goToOnline();
}

void TB_Menu::exit()
{
    emit goToExit();
}
