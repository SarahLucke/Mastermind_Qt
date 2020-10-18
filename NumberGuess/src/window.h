#ifndef WINDOW_H
#define WINDOW_H

#include "logic.h"

#include <QWidget>
#include <QGridLayout>
#include <QList>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QComboBox>
#include <QMessageBox>

class Window : public QWidget
{
    Q_OBJECT

private:
    struct s_line{
        QString nums[NUMBER_COUNT];
        QString result;
    };
    QString OUTPUT_RED = "<font color='red' size='10'>*</font>";
    QString OUTPUT_ORANGE = "<font color='orange' size='10'>*</font>";
    QString OUTPUT_GREEN = "<font color='green' size='10'>*</font>";
    QString HELP_TEXT = QStringLiteral(
                "The player has to guess a number of five digits. After each guess the player gets hints to determine which digits are correct.<br><br>"
                "$green Green stands for a number you guessed correctly and put in the right spot in the number array.<br>"
                "$orange Orange stands for a number you guessed correctly, but is not in the right spot of the number array.<br>"
                "$red Red stands for a number you guessed that doesn't belong in the number array.<br><br>"
                "The two modes, easy and hard, differentiate in the way, the results are shown. "
                "In easy mode, the color is directly associated with the numbers, while in hard mode, this direct association is missing. "
                "In hard mode, the player only gets feedback, how many numbers in the current guess belong to each of the three categories, but he doesn't get a hint, which."
                );

private:
    void clearScreen();
    void deleteWidgetAt(int all_row, int k);

protected:
    bool close();

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
    QGridLayout inputLayout;
    QList<QSpinBox*> inputs;
    QLabel inputLabel;
    QPushButton* helpButton;
    QPushButton* submitButton;
    QPushButton* retryButton;
    QList<s_line> m_results;
    Logic* logic;
    Logic::e_modes mode;
    QComboBox* modeSelection;
    int row;
    int all_row;
    int col=0;

private slots:
    void OnClick_Submit();
    void OnClick_Help();
    void OnClick_Retry();
    void OnClick_Mode();
};

#endif // WINDOW_H
