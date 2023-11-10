#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::on_buttonBox_accepted() {
    arr[0] = ui->lineEdit->text().toInt();
    arr[1] = ui->lineEdit_2->text().toInt();
}


void Dialog::on_buttonBox_rejected() {
    arr[0] = -1;
    arr[1] = -1;

}

