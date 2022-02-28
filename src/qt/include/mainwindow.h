#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
/*!
* \brief Класс нужный для создания оконного приложения
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /*!
    * \brief Метод вычисляет текстовое значение производной
    */
    void on_calcderb_clicked();
    /*!
    * \brief Метод копирует текстовое значение производной в поле ввода функции
    */
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
