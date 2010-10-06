#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include <QtGui>
#include "../PokemonInfo/battlestructs.h"
#include "../Utilities/otherwidgets.h"
#include "basebattlewindow.h"


class AttackZone;
class PokeZone;
class BattleDisplay;
class TargetSelection;
class StruggleZone;

class BattleInfo : public BaseBattleInfo
{
public:
    BattleInfo(const TeamBattle &myteam, const PlayerInfo &me, const PlayerInfo &opp, bool doubles, int myself, int oppo);

    /* Possible choices */
    bool possible;

    QList<BattleChoices> choices;
    QList<BattleChoice> choice;
    QList<bool> available;
    QList<bool> done;

    int currentSlot;

    /* My team */
    TeamBattle myteam;

    bool sent;

    QList<BattleStats> mystats;

    const PokeBattle &currentPoke(int spot) const;
    PokeBattle &currentPoke(int spot);

    QList<PokeBattle> m_tempPoke;
    PokeBattle &tempPoke(int spot);

    int number(int spot) const {
        return spot / 2;
    }

    virtual void switchPoke(int spot, int poke, bool own) {
        BaseBattleInfo::switchPoke(spot, poke);
        if (!own) {
            return;
        }
        myteam.switchPokemon(number(spot), poke);
        currentShallow(spot) = myteam.poke(number(spot));
        tempPoke(spot) = myteam.poke(number(spot));
    }

    int lastMove[6];
};

/* The battle window called by the client, online */

class Client;

class BattleWindow : public BaseBattleWindow
{
    Q_OBJECT

public:
    BattleWindow(int battleid, const PlayerInfo &me, const PlayerInfo &opponent, const TeamBattle &myteam, const BattleConfiguration &conf);

    BattleInfo &info() {
        return *(BattleInfo*)(&BaseBattleWindow::info());
    }

    const BattleInfo &info() const {
        return *(BattleInfo*)(&BaseBattleWindow::info());
    }

    enum {
        MoveTab= 0,
        PokeTab= 1
    };

    enum {
        TargetTab = 2,
        StruggleTab = 3
    };

    TeamBattle &team();
    const TeamBattle &team() const;

    void switchToNaught(int spot);
    void switchTo(int pokezone, int spot, bool forced);

    void addSpectator(bool add, int id);

    /* Disable / enable buttons */
    void updateChoices();
    /* sends the choice */
    void sendChoice(const BattleChoice &b);
    QString nick(int spot) const;

    int ownSlot() const;
public slots:
    void switchClicked(int zone);
    void attackClicked(int zone);
    void sendMessage();
    void attackButton();
    void clickClose();
    void emitCancel();
    void switchToPokeZone();
signals:
    void forfeit(int battleid);
protected:
    void closeEvent(QCloseEvent *);
    virtual void dealWithCommandInfo(QDataStream &s, int command, int spot, int truespot);

protected slots:
    void animateHPBar();
    void changeAttackText(int i);
    void targetChosen(int i);
private:

    int idme() const {
        return info().pInfo[info().myself].id;
    }

    int idopp() const {
        return info().pInfo[info().opponent].id;
    }

    void goToNextChoice();
    void cancel();

    void disableAll();
    void enableAll();

    void openRearrangeWindow(const ShallowShownTeam &t);

    QStackedWidget *mystack;
    QTabWidget *mytab;
    QListWidget *myspecs;
    AttackZone *myazones[2];
    StruggleZone *szone;
    TargetSelection *tarZone;
    QList<QButtonGroup*> mybgroups;
    PokeZone *mypzone;
    QPushButton *myswitch, *myattack, *mycancel;
};

class BattleDisplay : public BaseBattleDisplay
{
    Q_OBJECT
public:
    BattleDisplay(BattleInfo &i);

    void updateHp(int spot);
    void updateToolTip(int spot);

    BattleInfo &info() const {
        return *(BattleInfo *)(&BaseBattleDisplay::info());
    }
public slots:
    void changeBarMode();

protected:
    const PokeBattle &mypoke(int spot) const {return info().currentPoke(spot); }
    const ShallowBattlePoke &foe(int spot) const {return info().currentShallow(spot); }

    QList<bool> percentageMode;
};


class AbstractAttackButton;

/* An attack zone is the zone where the attacks are displayed */
class AttackZone : public QWidget
{
    Q_OBJECT
public:
    AttackZone(const PokeBattle &poke, int gen);

    AbstractAttackButton *tattacks[4];
    QAbstractButton *attacks[4];
signals:
    void clicked(int attack);

private:
    QSignalMapper *mymapper;
};

class AbstractAttackButton
{
public:
    //AbstractAttackButton();
    virtual void updateAttack(const BattleMove& b, const PokeBattle &p, int gen) = 0;

    QAbstractButton *pointer() {
        return dynamic_cast<QAbstractButton *> (this);
    }

    QLabel *name;
    QLabel *pp;
};

class ImageAttackButton : public QImageButton, public AbstractAttackButton
{
    Q_OBJECT
public:
    ImageAttackButton(const BattleMove& b, const PokeBattle &p, int gen);
    virtual void updateAttack(const BattleMove& b, const PokeBattle &p, int gen);
};

class OldAttackButton : public QPushButton, public AbstractAttackButton
{
    Q_OBJECT
public:
    OldAttackButton(const BattleMove& b, const PokeBattle &p, int gen);
    virtual void updateAttack(const BattleMove& b, const PokeBattle &p, int gen);
};

class PokeButton;

/* When you want to switch pokemons, that's what you see */
class PokeZone : public QWidget
{
    Q_OBJECT
public:
    PokeZone(const TeamBattle &team);

    PokeButton *pokes[6];
signals:
    void switchTo(int poke);

private:
    QSignalMapper *mymapper;
};

class PokeButton : public QPushButton
{
    Q_OBJECT
public:
    PokeButton(const PokeBattle &p);
    void changePokemon(const PokeBattle &p);
    void update();
    void updateToolTip();
private:

    const PokeBattle *p;
};

class TargetSelection : public QWidget
{
    Q_OBJECT
public:
    TargetSelection(const BattleInfo &info);

    void updateData(const BattleInfo &info, int move, int gen);
signals:
    void targetSelected(int target);
private:
    QPushButton * pokes[4];
};

class StruggleZone : public QWidget
{
    Q_OBJECT
public:
    StruggleZone();

signals:
    void attackClicked();
};

class RearrangeWindow : public QWidget
{
    Q_OBJECT
public:
    RearrangeWindow(TeamBattle &t, const ShallowShownTeam &op);
signals:
    void forfeit();
    void done();
public slots:
    void runExchanges();
private:
    TeamBattle *myteam;

    QPushButton *buttons[6];
};

#endif // BATTLEWINDOW_H
