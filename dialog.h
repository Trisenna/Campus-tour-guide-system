#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT

  public:
    explicit Dialog(QWidget *parent = nullptr);
    int arr[2];
    ~Dialog();
    int* getName() {

        return  arr;
    }

  private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

  private:
    Ui::Dialog *ui;

};

#endif // DIALOG_H
