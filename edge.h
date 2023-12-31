#ifndef EDGE_H
#define EDGE_H


class Edge {
  public:
    Edge() {};
    Edge(int x, int y, int z, int d) {
        start = x;
        end = y;
        weight = z;
        color = d;
    }
    Edge(int start, int end) {
        this->start = start;
        this->end = end;
    }
    int start;
    int end;
    int weight;
    int color;
};

#endif // EDGE_H
