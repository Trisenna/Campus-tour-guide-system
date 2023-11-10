#include "MainWindow.h"
#include <fstream>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <iostream>
#include <thread>
#include <iterator>

#include "code.h"
#include "dialog.h"
#include <bits/stl_pair.h>
static int COLOR = 1;

const int INFINITY_DISTANCE = std::numeric_limits<int>::max();

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QString vexFilePath = QFileDialog::getOpenFileName(this, "Choose Vex File", "", "Text Files (*.txt)");
    QString edgeFilePath = QFileDialog::getOpenFileName(this, "Choose Edge File", "", "Text Files (*.txt)");
    path = edgeFilePath;

    if (!vexFilePath.isEmpty() && !edgeFilePath.isEmpty()) {
        // Read data from Vex.txt
        readVexFile(vexFilePath);

        // Read data from Edge.txt
        readEdgeFile(edgeFilePath);


    }
    for(int i = 0; i < Code.size(); i++) {
        isVisited.push_back(false);
    }
    for(int i = 0; i < edges.size(); i++) {
        edgeisVisited.push_back(false);
    }
    drawMap();
    setupUI();
    this->setGeometry(100, 200, 900, 700);
}
void MainWindow::updateWeight() {


    edgeData.clear();
    edges.clear();


    QString edgeFilePath = QFileDialog::getOpenFileName(this, "Choose Edge File", "", "Text Files (*.txt)");

    if (!edgeFilePath.isEmpty()) {

        // Read data from Edge.txt
        readEdgeFile(edgeFilePath);


    }
    this->repaint();



}
void MainWindow::displayNodeDescription() {
    QString str = static_cast<QString>(this->sender()->objectName());
    qDebug() << str;
    QString tem = str[str.size() - 1];
    int a = tem.toInt();

    QMessageBox::information(this, "Node Description", Code[a - 1].description);
}

QString MainWindow::getDescription(int nodeIndex) const {
    return Code[nodeIndex].description;
}

void MainWindow::loadMapData() {
    QString vexFilePath = QFileDialog::getOpenFileName(this, "Choose Vex File", "", "Text Files (*.txt)");
    QString edgeFilePath = QFileDialog::getOpenFileName(this, "Choose Edge File", "", "Text Files (*.txt)");
    path = edgeFilePath;

    if (!vexFilePath.isEmpty() && !edgeFilePath.isEmpty()) {
        // Read data from Vex.txt
        readVexFile(vexFilePath);

        // Read data from Edge.txt
        readEdgeFile(edgeFilePath);


    }

}
void sleep(unsigned int msec) {
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::DFS(int i) {
    std::cout << Code[i].name.toStdString() << std::endl;

    //将该节点设置为已访问
    isVisited[i] = true;
    //查找节点i的第一个邻接节点
    int w = getFirstNeighbor(i);

    bool flag = true;


    //如果存在，进行递归
    while(w != -1) {

        //如果w节点没有被访问过
        if(!isVisited[w] ) {
            flag = false;
            for (auto& edge : edges) {
                if (edge.start == i + 1 && edge.end == w + 1) {
                    edge.color = COLOR % 5 + 1;
                    break;
                }
            }
            for(auto& edge : edges) {
                if (edge.start == w + 1 && edge.end == i + 1) {
                    edge.color = COLOR % 5 + 1;
                    break;
                }
            }

            DFS(w);
        }
        //如果w节点已经被访问过
        w = getNextNeighbor(i, w);

        this->repaint();
    }
    isVisited[i] = false;
    COLOR++;
    this->repaint();
    sleep(200);

}
int MainWindow::getFirstNeighbor(int index) {
    int numNodes = nodeCoordinates.size();
    std::vector<std::vector<int>> distanceMatrix(numNodes, std::vector<int>(numNodes, INFINITY_DISTANCE));

    for (const auto& edge : edges) {
        int start = edge.start - 1;
        int end = edge.end - 1;
        int weight = edge.weight;
        distanceMatrix[start][end] = weight;

        distanceMatrix[end][start] = weight;

        // Store the predecessor
    }

    for(int i = 0; i < numNodes; i++) {
        if(distanceMatrix[index][i] != INFINITY_DISTANCE) {
            return i;
        }
    }
    return -1;
}
int MainWindow::getNextNeighbor(int i, int w) {
    int numNodes = nodeCoordinates.size();
    std::vector<std::vector<int>> distanceMatrix(numNodes, std::vector<int>(numNodes, INFINITY_DISTANCE));

    for (const auto& edge : edges) {
        int start = edge.start - 1;
        int end = edge.end - 1;
        int weight = edge.weight;
        distanceMatrix[start][end] = weight;

        distanceMatrix[end][start] = weight;

        // Store the predecessor
    }
    for(int j = w + 1; j < numNodes; j++) {
        if(distanceMatrix[i][j] != INFINITY_DISTANCE) {
            return j;
        }
    }
    return -1;
}
void MainWindow::DFSslot() {
    int i = dfsInput->text().toInt();
    for(int i = 0; i < Code.size(); i++) {
        isVisited[i] = false;
    }
    dfsInput->clear();
    clearInputFields();
    COLOR = 1;
    if(i > 0 && i <= Code.size()) {
        DFS(i - 1);
    }
}
void MainWindow::findShortestPath() {
    int startNode = startNodeInput->text().toInt();
    int endNode = endNodeInput->text().toInt();
    if(startNode > endNode) {
        int tem = 0;
        tem = startNode;
        startNode = endNode;
        endNode = tem;
    }
    startNodeInput->clear();
    endNodeInput->clear();

    // Set all lines to gray color
    for ( auto& edge : edges) {


        edge.color = 0;
        this->repaint();
        // Assuming weight is stored in the third element

    }
    int numNodes = nodeCoordinates.size();
    std::vector<std::vector<int>> distanceMatrix(numNodes, std::vector<int>(numNodes, INFINITY_DISTANCE));
    std::vector<std::vector<int>> predecessor(numNodes, std::vector<int>(numNodes, -1));

    for (const auto& edge : edges) {
        int start = edge.start - 1;
        int end = edge.end - 1;
        int weight = edge.weight;
        distanceMatrix[start][end] = weight;

        predecessor[start][end] = start;

        // Store the predecessor
    }
//    for (int i = 0; i < numNodes; i++) {
//        for (int j = 0; j < numNodes; j++) {
//            qDebug() << distanceMatrix[i][j];
//        }
//        qDebug() << "\n";
//    }
//    for (int i = 0; i < numNodes; i++) {
//        for (int j = 0; j < numNodes; j++) {
//            qDebug() << predecessor[i][j];
//        }
//        qDebug() << "\n";
//    }
    //找到两点间的最短路径并将路径记录下来
    for (int k = 0; k < numNodes; k++) {
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                if (distanceMatrix[i][k] != INFINITY_DISTANCE && distanceMatrix[k][j] != INFINITY_DISTANCE && distanceMatrix[i][k] + distanceMatrix[k][j] < distanceMatrix[i][j]) {
                    distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j]; // Update the distance

                    predecessor[i][j] = predecessor[k][j]; // Update the predecessor
                }
            }
        }
    }
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            qDebug() << predecessor[i][j];
        }
        qDebug() << "\n";
    }
    // Find the shortest path from startNode to endNode
    std::vector<int> path;

    int current = endNode - 1;
    while (current != -1) {
        path.push_back(current ); // Add the node to the path
        current = predecessor[startNode - 1][current]; // Move to the predecessor
    }
    std::reverse(path.begin(), path.end()); // Reverse the path
    for(int i = 0; i < path.size(); i++) {
        qDebug() << path[i] ;
    }
    // Set the color of the edges on the shortest path to red
    for (int i = 0; i < path.size() - 1; i++) {
        int start = path[i] + 1;
        int end = path[i + 1] + 1;
        for (auto& edge : edges) {
            if (edge.start == start && edge.end == end) {
                edge.color = 1; // Red color
                break;
            }
        }
    }
    this->repaint(); // Repaint the window

}

void MainWindow::displayShortestPath(const std::vector<int>& path) {
    for(int i = 0; i < path.size() - 1; i++) {
        for (const auto& edge : edges) {
            int startNode = edge.start - 1 ;
            int endNode = edge.end - 1;
            if (startNode == path[i] && endNode == path[i + 1]) {
                edges[i].color = 1;
                this->repaint();
                // Assuming weight is stored in the third element
            }
        }

    }

    // Now, you have the names of nodes in the shortest path in the QStringList pathNames.
    // You can use this information to update your UI or display it as needed.
}
void MainWindow::clearInputFields() {
    startNodeInput->clear();
    endNodeInput->clear();

    // Set all lines to gray color
    for ( auto& edge : edges) {


        edge.color = 0;
        this->repaint();
        // Assuming weight is stored in the third element

    }

    // Update UI
    // ...
}
void MainWindow::paintEvent(QPaintEvent *) {
    QPainter pbk(this);

    // Draw lines
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // Anti-aliasing

    for (const auto& edge : edges) {
        int startNode = edge.start;
        int endNode = edge.end;
        if(startNode != endNode) {
            int color = edge.color;
            QPoint startPoint = nodeCoordinates[startNode - 1];
            QPoint endPoint = nodeCoordinates[endNode - 1];

            // Draw line
            QPen pen;
            pen.setWidth(5);
            if(color == 0) {
                pen.setColor(Qt::gray);
            } else if(color == 1) {
                pen.setColor(Qt::blue);
            } else if(color == 2) {
                pen.setColor(Qt::red);
            } else if(color == 3) {
                pen.setColor(Qt::black);
            } else if(color == 4) {
                pen.setColor(Qt::yellow);
            } else if(color == 5) {
                pen.setColor(Qt::green);
            }
            painter.setPen(pen);
            painter.drawLine(startPoint.x() + 7, startPoint.y() + 7, endPoint.x() + 7, endPoint.y() + 7);

            // Display weight
            pen.setColor("Purple");
            painter.setPen(pen);
            painter.drawText((startPoint + endPoint) / 2, QString::number(edge.weight));

            // Save line for future reference
            lines.push_back(new QGraphicsLineItem(startPoint.x() + 7, startPoint.y() + 7, endPoint.x() + 7, endPoint.y() + 7));

        }
    }
    // Don't forget to end the QPainter
    painter.end();

}
void MainWindow::drawMap() {
    QPushButton* locate[Code.size()];

    for(int i = 1; i <= Code.size(); i++) {
        std::cout << "fdsfdsf";
        locate[i - 1] = new QPushButton(this);
        QString tem;
        tem = Code[i - 1].name;
        tem.append("-");
        tem.append(QString::number(Code[i - 1].index));
        locate[i - 1]->setText(tem);
        locate[i - 1]->setObjectName(tem);
        locate[i - 1]->resize(70, 20);
        locate[i - 1]->setGeometry(Code[i - 1].x, Code[i - 1].y, 70, 20);

        // 设置按钮遮罩
        QRect rect(0, 0,  70, 20);
        QRegion region(rect, QRegion::Rectangle);
        locate[i - 1]->setMask(region);
        connect(locate[i - 1], &QPushButton::clicked, this, &MainWindow::displayNodeDescription);

    }
    // Draw input fields and buttons
    startNodeInput = new QLineEdit(this);
    endNodeInput = new QLineEdit(this);

    QPushButton *findShortestPathButton = new QPushButton("Find Shortest Path", this);
    connect(findShortestPathButton, &QPushButton::clicked, this, &MainWindow::findShortestPath);

    startNodeInput->setGeometry(500, 500, 50, 20);
    endNodeInput->setGeometry(600, 500, 50, 20);
    findShortestPathButton->setGeometry(500, 530, 150, 30);

    // Draw clear button
    QPushButton *clearButton = new QPushButton("Clear", this);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearInputFields);

    clearButton->setGeometry(700, 20, 80, 30);

    QPushButton *dfsButton = new QPushButton("遍历", this);
    connect(dfsButton, &QPushButton::clicked, this, &MainWindow::DFSslot);
    dfsButton->setGeometry(500, 400, 50, 20);
    dfsInput = new QLineEdit(this);
    dfsInput->setGeometry(600, 400, 50, 20);

    QPushButton* addButton = new QPushButton("Add Edge", this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addEdge);
    addButton->setGeometry(550, 350, 100, 20);

    QPushButton* deleteButton = new QPushButton("Delete Edge", this);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteEdge);
    deleteButton->setGeometry(550, 300, 100, 20);

    QPushButton* minTree = new QPushButton("minTree", this);
    connect(minTree, &QPushButton::clicked, this, &MainWindow::findMinimumSpanningTree);
    minTree->setGeometry(500, 450, 80, 20);
}

void MainWindow::setupUI() {
    this->setGeometry(100, 200, 600, 600);
    auto loadMapButton = new QPushButton("Load Map Data", this);
    connect(loadMapButton, &QPushButton::clicked, this, &MainWindow::updateWeight);


    loadMapButton->setGeometry(10, 20, 150, 30);

    scene = new QGraphicsScene(this);
    this->setGeometry(100, 200, 600, 800);

    drawMap();
    this->repaint();

}
void MainWindow::readVexFile(const QString& filePath) {
    std::ifstream file(filePath.toStdString());
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int nodeIndex, x, y;
            std::string nodeName, nodeDescription;
            this->update();
            if (iss >> nodeIndex >> x >> y >> nodeName >> nodeDescription) {
                nodeCoordinates.push_back(QPoint(x, y));
                Code.push_back(code(x, y, nodeIndex, QString::fromStdString(nodeName), QString::fromStdString(nodeDescription)));


            }
        }

        file.close();

    }
}
void MainWindow::writeToFile( const QString& content) {
    // 创建一个文件对象
    QFile file(path);

    // 打开文件，如果无法打开，则输出错误信息并返回
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing: " << file.errorString();
        return;
    }

    // 创建一个文本流，并将其关联到文件
    QTextStream out(&file);

    // 将内容写入文件，并在每次写入后加上换行符
    out << content ;

    // 关闭文件
    file.close();
}
void MainWindow::deleteEdge() {
    Dialog dia(this);

    dia.exec();
    int* add = dia.getName();
    int startNode = add[0];
    int endNode = add[1];
    for(int i = 0; i < edges.size(); i++) {
        if((edges[i].start == startNode && edges[i].end == endNode) || (edges[i].start == endNode && edges[i].end == startNode)) {
            edges[i].start = edges[i].end;
            edges[i].weight = 0; // Remove the edge from the vector
            break;
        }
    }
    QString s;
    for(int i = 0; i < edges.size(); i++) {
        s.append(QString::number(edges[i].start));
        s.append(" ");
        s.append(QString::number(edges[i].end));
        s.append("\n");
    }
    writeToFile(s);
    this->repaint();


}
void MainWindow::addEdge() {
    Dialog dia(this);

    dia.exec();
    int* add = dia.getName();
    int startNode = add[0];
    int endNode = add[1];
    edgeData.push_back(std::make_tuple(startNode, endNode, 0, 0));
    edges.push_back(Edge(startNode, endNode, 0, 0));
    for(int j = 0; j < edges.size(); j++) {
        int startnode = edges[j].start;
        int endnode = edges[j].end;
        int x1;
        int y1;
        int x2;
        int y2;
        for(int i = 0; i < Code.size(); i++) {
            if(Code[i].index == startnode) {
                x1 = Code[i].x;
                y1 = Code[i].y;
                break;
            }
        }
        for(int i = 0; i < Code.size(); i++) {
            if(Code[i].index == endnode) {
                x2 = Code[i].x;
                y2 = Code[i].y;
                break;
            }
        }
        int weight = (int)sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        edges[j].weight = weight;
    }
    QString s;
    for(int i = 0; i < edges.size(); i++) {
        s.append(QString::number(edges[i].start));
        s.append(" ");
        s.append(QString::number(edges[i].end));
        s.append("\n");
    }
    writeToFile(s);
    this->repaint();


}
void MainWindow::readEdgeFile(const QString& filePath) {
    std::ifstream file(filePath.toStdString());
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int startNode, endNode;

            if (iss >> startNode >> endNode ) {
                edgeData.push_back(std::make_tuple(startNode, endNode, 0, 0));
                edges.push_back(Edge(startNode, endNode, 0, 0));
            }

        }


        file.close();
        for(int j = 0; j < edges.size(); j++) {
            int startnode = edges[j].start;
            int endnode = edges[j].end;
            int x1;
            int y1;
            int x2;
            int y2;
            for(int i = 0; i < Code.size(); i++) {
                if(Code[i].index == startnode) {
                    x1 = Code[i].x;
                    y1 = Code[i].y;
                    break;
                }
            }
            for(int i = 0; i < Code.size(); i++) {
                if(Code[i].index == endnode) {
                    x2 = Code[i].x;
                    y2 = Code[i].y;
                    break;
                }
            }
            int weight = (int)sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
            edges[j].weight = weight;
        }


    }

}
void MainWindow::findMinimumSpanningTree() {
    int numNodes = nodeCoordinates.size();
    std::vector<std::vector<int>> distanceMatrix(numNodes, std::vector<int>(numNodes, INFINITY_DISTANCE));
    bool isVisited[numNodes];
    for(int i = 0; i < numNodes; i++) {
        isVisited[i] = false;
    }
    //矩阵
    for (const auto& edge : edges) {
        int start = edge.start - 1;
        int end = edge.end - 1;
        int weight = edge.weight;
        if(start != end) {
            distanceMatrix[start][end] = weight;

            distanceMatrix[end][start] = weight;
        }

        // Store the predecessor
    }
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            std::cout << distanceMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    //设置开始位置为0
    int start = 0;
    int minEdge = INFINITY_DISTANCE;
    int nextNode = 0;
    int preNode = 0;

    std::vector<Edge> minEdges;
    std::vector<int> min;
    min.push_back(start);
    isVisited[start] = true;
    for(int i = 0; i < numNodes - 1; i++) {
        int m = min.size();
        for(int j = 0; j < m; j++) {
            int c = min[j];
            for(int k = 0; k < numNodes; k++) {
                if( distanceMatrix[c][k] != INFINITY_DISTANCE && minEdge > distanceMatrix[c][k] && !isVisited[k]) {
                    minEdge = distanceMatrix[c][k];
                    nextNode = k;
                    preNode = c;
                }
            }
        }
        isVisited[nextNode] = true;
        min.push_back(nextNode);
        minEdges.push_back(Edge(preNode, nextNode));
        minEdge = INFINITY_DISTANCE;
        nextNode = 0;
        preNode = 0;
    }
    std::vector<Edge>::iterator it;
    for(it = minEdges.begin(); it != minEdges.end(); it++) {

        int a = it->start;
        int b = it->end;
        qDebug() << a << " " << b;
        for(auto& edge : edges) {
            if((edge.start == a + 1 && edge.end == b + 1) || (edge.end == a + 1 && edge.start == b + 1)) {

                edge.color = 1;
            }
        }
    }
    this->repaint();

}


