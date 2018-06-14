#include "window.h"

/*
 * TODO:
 * change event for hard mode,
 * help,
 * retry crashes
 * */

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    row = 0;
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
}

void Window::OnClick_Submit(){
    int guessed[] = {inputs[0]->value(), inputs[1]->value(), inputs[2]->value(), inputs[3]->value(), inputs[4]->value()};
    int result[NUMBER_COUNT];
    logic->GetResult(guessed, result);
    switch (mode){
    case Logic::EASY:
        for(int i=NUMBER_COUNT-1; i>=0; i--){
            if(result[i]==Logic::CORRECT){
                inputs[i]->setEnabled(false);
            }else{
                inputs[i]->setFocus();
            }
        }
        break;
    case Logic::HARD:
        inputs[0]->setFocus();
        break;
    default:
        break;
    }
    // format results:
    s_line line;
    switch(mode){
    case Logic::EASY:
        for(int i=0; i<NUMBER_COUNT; i++){
            switch(result[i]){
            case Logic::WRONG:
                line.nums[i]= "<font color='red'>" + QString::number(guessed[i]) + "</font>";
                break;
            case Logic::CORRECT:
                line.nums[i]= "<font color='green'>" + QString::number(guessed[i]) + "</font>";
                // set number in input
                break;
            case Logic::WRONG_PLACE:
                line.nums[i]= "<font color='orange'>" + QString::number(guessed[i]) + "</font>";
                break;
            default:
                line.nums[i]= QString::number(guessed[i]);
                break;
            }
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
                line.result += "<font color='red'>*</font>";
                break;
            case Logic::CORRECT:
                line.result += "<font color='green'>*</font>";
                // set number in input
                break;
            case Logic::WRONG_PLACE:
                line.result += "<font color='yellow'>*</font>";
                break;
            default:
                line.result += "*";
                break;
            }
        }
        break;
    default:
        break;
    }
    // write results:
    // add new result to list of results:
    m_results.push_back(line);

    all_row=row;

    // write all results:
    for(int i=0; i<m_results.count(); i++){
        for(int j=0; j<NUMBER_COUNT; j++)
            inputLayout.addWidget(new QLabel(m_results.at(i).nums[j]), all_row, j);
        inputLayout.addWidget(new QLabel(m_results.at(i).result), all_row++, NUMBER_COUNT);
    }
    int cnt=0;
    for(int i=0; i<NUMBER_COUNT; i++){
        if(result[i]==Logic::CORRECT)
            cnt++;
    }
    if(cnt==NUMBER_COUNT){
        submitButton->setEnabled(false);
        int trys = all_row-row;
        QString out = "Congratulations! Took you " + QString::number(trys) + " trys!";
        outputLabel = new QLabel(out,this);
        inputLayout.addWidget(outputLabel, all_row, col, 1, NUMBER_COUNT-1);
        retryButton = new QPushButton("Retry", this);
        inputLayout.addWidget(retryButton, all_row++, NUMBER_COUNT);
        connect(retryButton, SIGNAL (released()), this, SLOT (OnClick_Retry()));
    }
    inputLayout.setRowStretch(all_row,50);
}

void Window::OnClick_Help(){
    QMessageBox msgBox;
    msgBox.setText("I need somebody...");
    msgBox.exec();
}

void Window::OnClick_Retry(){
    delete logic;
    logic = new Logic(mode);
    delete outputLabel;
    outputLabel = NULL;
    delete retryButton;
    retryButton = NULL;
    m_results.clear();
    // delete rows:
    int i=row;
    for(int k=0; k<=NUMBER_COUNT; k++){
        delete inputLayout.itemAtPosition(i, k);
    }
    submitButton->setEnabled(true);
    for(int i=0; i<NUMBER_COUNT; i++){
        inputs.at(i)->setEnabled(true);
        inputs.at(i)->setValue(0);
    }
}

bool Window::close(){
    delete outputLabel;
    delete helpButton;
    delete submitButton;
    delete retryButton;
    delete logic;
    delete modeSelection;
    return true;
}

Window::~Window()
{
}
