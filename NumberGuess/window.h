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

protected:
    bool close();

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
    QGridLayout inputLayout;
//    QVBoxLayout* layout_vertical;
    QList<QSpinBox*> inputs;
    QLabel inputLabel;
    QLabel* outputLabel;
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
};

#endif // WINDOW_H
