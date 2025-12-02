#include "calculator.h"
#include "ui_calculator.h"

#include <QList>

QString digits = "0123456789.";
QString symbols = "+-*/";
QString function = "AC%±";

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Calculator)
    , currentInput("")
    , previousInput("")
    , currentOperator("")
    , currentFunction("")
    , waitForNewInput(false)
{
    ui->setupUi(this);
    styleButtons();
    connectButtons();
}

void Calculator::styleButtons() {
    QList<QPushButton*> buttons = findChildren<QPushButton*>();

    for(QPushButton* button : buttons) {
        button->setFixedSize(71, 71);
        QString buttonText = button->text();
        if(digits.contains(buttonText) || buttonText == "⌫"){
            button->setStyleSheet("QPushButton {"
                                  "    border-radius: 35.5px;"
                                  "    background-color: #D3D3D3;"
                                  "    color: white;"
                                  "    font-size: 18px;"
                                  "    font-weight: bold;"
                                  "}"
                                  "QPushButton:pressed {"
                                  "    background-color: #C0C0C0;"
                                  "}");
        }else if(symbols.contains(buttonText) || buttonText == "="){
            button->setStyleSheet("QPushButton {"
                                  "    border-radius: 35.5px;"
                                  "    background-color: #FFA500;"
                                  "    color: white;"
                                  "    font-size: 18px;"
                                  "    font-weight: bold;"
                                  "}"
                                  "QPushButton:pressed {"
                                  "    background-color: #FFB74D;"
                                  "}");
        }else if(function.contains(buttonText)){
            button->setStyleSheet("QPushButton {"
                                  "    border-radius: 35.5px;"
                                  "    background-color: #505050;"
                                  "    color: white;"
                                  "    font-size: 18px;"
                                  "    font-weight: bold;"
                                  "}"
                                  "QPushButton:pressed {"
                                  "    background-color: #708090;"
                                  "}");
        }
    }
}


void Calculator::connectButtons() {
    QList<QPushButton*> buttons = findChildren<QPushButton*>();

    for(QPushButton* button : buttons) {
           QString text = button->text();

           if(digits.contains(text)) {
               connect(button, &QPushButton::clicked, this, &Calculator::onDigitClicked);
           }
           else if(symbols.contains(text)) {
               connect(button, &QPushButton::clicked, this, &Calculator::onOperatorClicked);
           }
           else if(text == "=") {
               connect(button, &QPushButton::clicked, this, &Calculator::onEqualsClicked);
           }
           else if(text == "%" || text == "±") {
               connect(button, &QPushButton::clicked, this, &Calculator::onFunctionClicked);
           }
           else if(text == "AC" || text == "⌫") {
               connect(button, &QPushButton::clicked, this, &Calculator::onClearClicked);
           }

    }

}


void Calculator::updateDisplay(QString input) {
    displayText += input;
    ui->label->setText(displayText);
}

void Calculator::onDigitClicked() {
      QPushButton* button = qobject_cast<QPushButton*>(sender());
      QString digit = button->text();

      if(waitForNewInput){
          currentInput = digit;
          waitForNewInput = false;
      } else{
          currentInput += digit;
      }
      updateDisplay(digit);
}

void Calculator::onOperatorClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString op = button->text();

    if(!currentInput.isEmpty()) {
        if(!previousInput.isEmpty() && !currentOperator.isEmpty()) {
            calculate();
        }
        previousInput = currentInput;
        currentOperator = op;
        waitForNewInput = true;
    }
    updateDisplay(currentOperator);
}


void Calculator::onEqualsClicked(){
    if((!previousInput.isEmpty() && !currentInput.isEmpty() && !currentOperator.isEmpty()) || currentFunction == "%") {
        calculate();
        previousInput = "";
        currentOperator = "";
    }
}


void Calculator::onFunctionClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString func = button->text();

    if(func == "%") {
        if(!currentInput.isEmpty()) {
            currentFunction = func;
            updateDisplay(func);
        }
    }
    else if(func == "±") {
        if(!currentInput.isEmpty()) {
            double value = currentInput.toDouble();
            QString currentValue = QString::number(value);
            value = -value;
            currentInput = QString::number(value);
            displayText = displayText.left(displayText.length() - currentValue.length());
            displayText = displayText + "(" + currentInput + ")";
            if(ui->label) ui->label->setText(displayText);
        }
    }
}

void Calculator::onClearClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString clearmetod = button->text();
    if(clearmetod == "⌫"){
        if(displayText.length() == 1){
        displayText = displayText.left(displayText.length() - 1);
        currentInput = currentInput.left(currentInput.length() - 1);
        ui->label->setText("0");
        } else{
            displayText = displayText.left(displayText.length() - 1);
            currentInput = currentInput.left(currentInput.length() - 1);
            previousInput = currentInput;
            if(ui->label)
                ui->label->setText(displayText);
        }
    } else{
        currentInput = "";
        previousInput = "";
        currentOperator = "";
        waitForNewInput = false;

        if(ui->label) {
            ui->label->setText("0");
            displayText = "";
        }
    }

}


void Calculator::calculate() {
    double prev = previousInput.toDouble();
    double curr = currentInput.toDouble();
    double result = 0;

    if(currentOperator == "+")
        result = prev + curr;
    else if(currentOperator == "-")
        result = prev - curr;
    else if(currentOperator == "*")
        result = prev * curr;
    else if(currentFunction == "%"){
        result = curr/100;
        currentFunction = "";
    } else if(currentOperator == "/") {
        if(curr != 0) 
            result = prev / curr;
        else result = 0;
    }
    currentInput = QString::number(result);
    if(ui->label)
        ui->label->setText(currentInput);
    displayText = QString::number(result);

}

Calculator::~Calculator() {
    delete ui;
}

