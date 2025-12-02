#include <QWidget>
#include <QPushButton>

namespace Ui { class Calculator; }

class Calculator : public QWidget {
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;
    void styleButtons();
    void connectButtons();
    void calculate();
    void updateDisplay(QString input);

    QString currentInput;
    QString previousInput;
    QString currentOperator;
    QString currentFunction;
    bool waitForNewInput;

    QString displayText;

private slots:
    void onDigitClicked();

    void onOperatorClicked();

    void onFunctionClicked();

    void onEqualsClicked();
    void onClearClicked();
};
