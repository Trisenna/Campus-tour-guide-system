#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "code.h"
#include "edge.h"
#include <QtWidgets>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *);

  private slots:

    void displayNodeDescription();
    QString getDescription(int nodeIndex) const;
    void loadMapData();
    void findShortestPath();
    void clearInputFields();

    void displayShortestPath(const std::vector<int> &path);
  private:
    QGraphicsScene* scene;
    std::vector<QRadioButton*> radioButtons;
    std::vector<QPoint> nodeCoordinates;
    std::vector<std::tuple<int, int, int, int>> edgeData;
    std::vector<int> Waycolor;
    std::vector<QGraphicsLineItem*> lines;  // 添加这一行声明
    std::vector<QString> description;
    std::vector<code> Code;
    std::vector<Edge> edges;
    QLineEdit *startNodeInput;
    QLineEdit *endNodeInput;
    QVBoxLayout *layout;

    void drawMap();
    void setupUI();
    void readVexFile(const QString& filePath);
    void readEdgeFile(const QString& filePath);
};

#endif // MAINWINDOW_H
