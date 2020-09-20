#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QColorDialog>
#include <QListWidget>

QSqlQuery query;
QSqlDatabase db;
QSqlQueryModel *model;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_creat_sql_Button_clicked()
{
    bool query_OK=0;
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./stu.db");
    bool ok=db.open();
    if(ok)
        QMessageBox::information(this,"Information","open sucessfully");
    else
        QMessageBox::information(this,"Information","open fail");

    QSqlQuery query;
    query_OK = query.exec("create table stu(name vchar primary key,gender vchar ,number vchar)");
    if(query_OK) {
        qDebug()<< "create table success!";
        QString insert_buf;
        insert_buf = QString("insert into stu values('%1','%2','%3');").arg("student1", "男", "20220413");
        query_OK = query.exec(insert_buf);
        if(!query_OK) {
            qDebug()<< "create student info"  << query.lastError();
        }

        insert_buf = QString("insert into stu values('%1','%2','%3');").arg("student2", "男", "20200414");
        query_OK = query.exec(insert_buf);
        if(!query_OK) {
            qDebug()<< "create student info"  << query.lastError();
        }

        insert_buf = QString("insert into stu values('%1','%2','%3');").arg("student3", "女", "20200414");
        query_OK = query.exec(insert_buf);
        if(!query_OK) {
            qDebug()<< "create student info"  << query.lastError();
        }

    show_view();
    clear_focus();
    } else {
        qDebug()<< "create table fail!"  << query.lastError();
    }
    show_view();
}

void  Widget::show_view()
{
    model = new QSqlQueryModel(this);
    model->setQuery("select * from stu");
    model->setHeaderData(0,Qt::Horizontal,tr("Name"));
    model->setHeaderData(1,Qt::Horizontal,tr("Gender"));
    model->setHeaderData(2,Qt::Horizontal,tr("Number"));
    ui->tableView->setModel(model);
}

void Widget::on_save_pushButton_clicked()
{
    name = ui->name_lineEdit->text();
    gender = ui->gender_comboBox->currentText();
    number=ui->number_lineEdit->text();
    if(name.isEmpty() || gender.isEmpty() || number.isEmpty())
    {
        QMessageBox::warning(this,"Incomplete information","error");
        return;
    }
    QString insert_buf;
    QSqlQuery query;
    insert_buf=QObject::tr("insert into stu values('%1','%2','%3')").arg(name, gender,number);
    query.exec(insert_buf);
    show_view();
    clear_focus();

}

void Widget::clear_focus()
{
    ui->name_lineEdit->clear();
    ui->number_lineEdit->clear();
    ui->name_lineEdit->setFocus();
}

void Widget::on_find_pushButton_clicked()
{
    find_buf=ui->fun_lineEdit->text();
    QSqlQuery query;
    query.exec("select * from stu");
    query.isActive();
    while(query.next())
    {
        QString ele0=query.value(0).toString();
        QString ele1=query.value(1).toString();
        QString ele2=query.value(2).toString();
        if((ele0==find_buf) || (ele1==find_buf) || (ele2==find_buf))
        {
            ui->name_lineEdit->setText(ele0);
            ui->gender_comboBox->setCurrentText(ele1);
            ui->number_lineEdit->setText(ele2);
            return;
        }
    }
    QMessageBox::information(this,"Not Find","No this record",QMessageBox::Ok);
}

void Widget::on_delete_pushButton_clicked()
{
    QString delete_buf;
    delete_buf = ui->fun_lineEdit->text();
    QSqlQuery query;
    query.exec("select * from stu");
    query.isActive();
    QString insert_buf;
    while(query.next())
    {
        QString ele0=query.value(0).toString();
        QString ele1=query.value(1).toString();
        QString ele2=query.value(2).toString();
        if((ele0==delete_buf) || (ele1==delete_buf) || (ele2==delete_buf))
        {
            if(ele0==delete_buf)
            {
                insert_buf=QObject::tr("delete from stu where name='%1'").arg(delete_buf);
            }
            else if(ele0==delete_buf)
            {
                insert_buf=QObject::tr("delete from stu where gender='%1'").arg(delete_buf);
            }
            else if(ele0==delete_buf)
            {
                insert_buf=QObject::tr("delete from stu where number='%1'").arg(delete_buf);
            }
            bool ok=query.exec(insert_buf);
            if(ok)
                QMessageBox::information(this,"delete successfully","Ok",QMessageBox::Ok);
            else
                QMessageBox::information(this,"delete fail","Error",QMessageBox::Ok);
            show_view();
            return;
        }
    }
    QMessageBox::information(this,"Delete Error","No this record",QMessageBox::Ok);
}

void Widget::on_change_pushButton_clicked()
{
    ui->fun_lineEdit->setText(find_buf);
    on_delete_pushButton_clicked();
    on_save_pushButton_clicked();
    ui->fun_lineEdit->clear();
}

void Widget::on_pushButton_clicked()
{
    close();
}
