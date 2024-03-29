#ifndef LAUNCHER_H_
#define LAUNCHER_H_

#include <QWidget>
#include <QStackedWidget>
#include <optional>
#include "Connect.h"
#include "InitView.h"
#include "MenuView.h"
#include "CreateView.h"
#include "JoinView.h"
#include "ClientProtocol.h"
#include "Queue.h"
#include "Snapshot.h"
#include "EventDTO.h"
#include "EventSender.h"
#include "SnapshotReceiver.h"
#include "GameDrawner.h"
#include "TypeDifficulty.h"
#include "PlayerReproductor.h"
#include <memory>

class Launcher : public QWidget {
    Q_OBJECT
    private:
        QStackedWidget mainWidget;
        InitView initView;
        ConnectView connectView;
        MenuView menuView;
        CreateView createView;
        JoinView joinView;
        std::optional<std::shared_ptr<Socket>> socket;
        ClientProtocol<Socket> clientProtocol;
        PlayerReproductor player;
        bool endGame;
        void initWidget();
        void initFont();
        void initGame(int menu, uint16_t idPlayer, uint8_t numPlayers,
                        bool& error);
    
    public:
        explicit Launcher(QWidget* parent = 0);
        virtual ~Launcher();
    Q_SIGNALS:
        void errorConnection();
    private slots:
        void goToMain();
        void goToConnect();
        void goToMenu();
        void goToCreate();
        void goToJoin();
        void createProtocol(const QString& ip, const QString& port);
        void sendCreateMatch(const QString& name, int mode, 
                            int operatorSelect, TypeDifficulty difficulty);
        void sendJoinMatch(int code, int operatorSelect);
    protected:
        void closeEvent(QCloseEvent* event) override;

};

#endif
