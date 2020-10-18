#include "window.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    all_row = row = 0;
    // configure buttons:
    submitButton = new QPushButton("Submit", this);
    helpButton = new QPushButton("Help", this);
    submitButton->setDefault(true);

    modeSelection = new QComboBox(this);
    modeSelection->addItem("easy");
    modeSelection->addItem("hard");
    mode = Logic::EASY;

    //configure layout-grid and input boxes:
    for(int i=0;i<NUMBER_COUNT;i++){
        inputLayout.setColumnMinimumWidth(i,10);
        inputLayout.setColumnStretch(i,0);
        inputs.push_back(new QSpinBox(this));
    }
    inputLayout.setColumnMinimumWidth(NUMBER_COUNT,20);
    inputLayout.setColumnStretch(NUMBER_COUNT,0);
    // empty col for stretching:
    inputLayout.setColumnStretch(NUMBER_COUNT+1, 20);
    // add widgets to layout grid:
    inputLayout.addWidget(modeSelection, row++, col, 1, NUMBER_COUNT-1);
    inputLayout.addWidget(&inputLabel, row, col, 1, NUMBER_COUNT-1);
    inputLayout.addWidget(helpButton, row++, NUMBER_COUNT);
    for(int i=0;i<NUMBER_COUNT;i++)
        inputLayout.addWidget(inputs[i], row, i);
    inputLayout.addWidget(submitButton,row++,NUMBER_COUNT);


    // set layout to this window:
    this->setLayout(&inputLayout);

    inputLabel.setText("input 5 different numbers:");

    logic = new Logic(mode);

    //connect button press events:
    connect(submitButton, SIGNAL (released()), this, SLOT (OnClick_Submit()));
    connect(helpButton, SIGNAL (released()), this, SLOT (OnClick_Help()));
    connect(modeSelection, SIGNAL (currentIndexChanged(const QString &)), this, SLOT (OnClick_Mode()));
}

void Window::OnClick_Mode(){
    mode = (Logic::e_modes) modeSelection->currentIndex();
    clearScreen();
}

void Window::OnClick_Submit(){
    int guessed[] = {inputs[0]->value(), inputs[1]->value(), inputs[2]->value(), inputs[3]->value(), inputs[4]->value()};
    int result[NUMBER_COUNT];
    logic->GetResult(guessed, result);
    // format results:
    s_line line;
    int cnt=0;
    switch(mode){
    case Logic::EASY:
        for(int i=NUMBER_COUNT-1; i>=0; i--){
            switch(result[i]){
            case Logic::WRONG:{
                line.nums[i] = OUTPUT_RED;
                inputs[i]->setFocus();
                break;
            }
            case Logic::CORRECT:{
                line.nums[i] = OUTPUT_GREEN;
                inputs[i]->setEnabled(false);
                cnt++;
                break;
            }
            case Logic::WRONG_PLACE:{
                line.nums[i] = OUTPUT_ORANGE;
                inputs[i]->setFocus();
                break;
            }
            default:
                line.nums[i]= QString::number(guessed[i]);
                break;
            }
            line.nums[i].replace("'10'", "'3'");
            line.nums[i].replace("*", QString::number(guessed[i]));
        }
        line.result = "";
        break;
    case Logic::HARD:
        for(int i=0; i<NUMBER_COUNT; i++){
            line.nums[i]= QString::number(guessed[i]);
        }
        std::sort(result, result+NUMBER_COUNT);
        for(int i=0; i<NUMBER_COUNT; i++){
            switch(result[i]){
            case Logic::WRONG:
                line.result += OUTPUT_RED;
                break;
            case Logic::CORRECT:
                line.result += OUTPUT_GREEN;
                cnt++;
                // set number in input
                break;
            case Logic::WRONG_PLACE:
                line.result += OUTPUT_ORANGE;
                break;
            default:
                line.result += "*";
                break;
            }
        }
        inputs[0]->setFocus();
        break;
    default:
        break;
    }
    // write results:
    // add new result to list of results:
    m_results.push_back(line);

    // delete old results:
    while (all_row >= row){
        for(int k=0; k<=NUMBER_COUNT; k++){
            deleteWidgetAt(all_row, k);
        }
        all_row--;
    }

    all_row=row;

    // write all results:
    for(int i=0; i<m_results.count(); i++){
        for(int j=0; j<NUMBER_COUNT; j++)
            inputLayout.addWidget(new QLabel(m_results.at(i).nums[j]), all_row, j);
        inputLayout.addWidget(new QLabel(m_results.at(i).result), all_row++, NUMBER_COUNT);
    }
    if(cnt==NUMBER_COUNT){
        submitButton->setEnabled(false);
        int trys = all_row-row;
        QString out = "Congratulations! Took you " + QString::number(trys) + " trys!";
        inputLayout.addWidget(new QLabel(out,this), all_row, col, 1, NUMBER_COUNT-1);
        QPushButton* retryButton = new QPushButton("Retry", this);
        inputLayout.addWidget(retryButton, all_row++, NUMBER_COUNT);
        connect(retryButton, SIGNAL (released()), this, SLOT (OnClick_Retry()));
    }
    inputLayout.setRowStretch(all_row,50);
}

void Window::OnClick_Help(){
    QMessageBox msgBox;
    QString txt = HELP_TEXT;
    txt.replace("$green", OUTPUT_GREEN);
    txt.replace("$orange", OUTPUT_ORANGE);
    txt.replace("$red", OUTPUT_RED);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(txt);
    msgBox.exec();
}

void Window::OnClick_Retry(){
    clearScreen();
}

void Window::clearScreen(){
    // roll a new game:
    delete logic;
    logic = new Logic(mode);
    // clear results list:
    m_results.clear();
    // delete result rows:
    while (all_row >= row){
        for(int k=0; k<=NUMBER_COUNT; k++){
            deleteWidgetAt(all_row, k);
        }
        all_row--;
    }
    inputLayout.setRowStretch(all_row,50);
    inputLayout.invalidate();
    // (re)enable buttons:
    submitButton->setEnabled(true);
    for(int i=0; i<NUMBER_COUNT; i++){
        inputs.at(i)->setEnabled(true);
        inputs.at(i)->setValue(0);
    }
}

void Window::deleteWidgetAt(int all_row, int k){
    QLayoutItem* item = inputLayout.itemAtPosition(all_row, k);
    if(item != nullptr){
        item->widget()->deleteLater();
        inputLayout.removeWidget(item->widget());
    }
}

bool Window::close(){
    delete helpButton;
    delete submitButton;
    delete logic;
    delete modeSelection;
    return true;
}

Window::~Window()
{
}
