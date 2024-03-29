#ifndef CREATE_VIEW_H_
#define CREATE_VIEW_H_

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QButtonGroup>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include "TypeDifficulty.h"

class CreateView : public QWidget {
    Q_OBJECT
    private:
        QLabel nameGame;
        QLineEdit lineName;
        QCheckBox clearZone;
        QCheckBox survival;
        QButtonGroup modeSelect;
        QCheckBox idf;
        QCheckBox p90;
        QCheckBox scout;
        QButtonGroup operatorSelect;
        QGridLayout mainLayout;
        QPushButton createButton;
        QPushButton backButton;
        QLabel operatorLabel;
        QLabel weaponLabel;
        QLabel habilityLabel;
        QLabel rateLabel;
        QLabel difficultyLabel;
        QComboBox difficultyBox;
        void initWidget();
        void initStylesheet();
        void initBackground();
    public:
        explicit CreateView(QWidget* parent = 0);
        virtual ~CreateView();
    Q_SIGNALS:
        void createClicked(const QString& name, int mode, int operatorSelect,
                            TypeDifficulty difficulty);
        void backClicked();
        // void selectOperator(QAbstractButton* opSelect);
    public slots:
        void onCreateClicked();
        void onBackClicked();
    private slots:
        void viewIDF();
        void viewP90();
        void viewScout();
};

#endif
