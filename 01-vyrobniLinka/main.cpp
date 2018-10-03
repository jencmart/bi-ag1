#include <iostream>

int visitedNodesCntGlobal = 0;
int64_t totalTimeGlobal = 0;

class Cycle {
};

///********************************************************************************************************
///***************************** VECTOR *******************************************************************
///********************************************************************************************************
template<class myType>
class vector {
    myType **m_first;
    int m_size;
    int m_capacity;

public:
    vector() : m_first(nullptr), m_size(0), m_capacity(1) { m_first = new myType *[m_capacity]; }

    vector(const vector &other) : m_first(nullptr), m_size(other.m_size), m_capacity(other.m_capacity) {
        m_first = new myType *[m_capacity];
        for (int i = 0; i < m_size; ++i)
            (m_first[i]) = new myType(other[i]);
    }

    ~vector() {
        for (int i = 0; i < m_size; ++i)
            delete (m_first[i]);
        delete[] m_first;
    };

    myType &operator[](int n) { return *m_first[n]; }

    const myType &operator[](int n) const { return *m_first[n]; }

    int size() const { return m_size; }

    void push_back(const myType &val) {
        if (m_size == m_capacity) {
            m_capacity *= 2;
            auto **tmp = new myType *[m_capacity];
            for (int i = 0; i < m_size; ++i)
                tmp[i] = m_first[i];
            delete[] m_first;
            m_first = tmp;
        }
        (m_first[m_size++]) = new myType(val);
    }
};

///********************************************************************************************************
///******************************** SET *******************************************************************
///********************************************************************************************************


class Set {
    enum Colour {
        RED, BLACK
    };

    struct setNode {
        explicit setNode(int val, setNode *parent, Colour colour) : m_val(val), m_colour(colour), m_parent(parent),
                                                                    m_left(nullptr), m_right(nullptr) {};

        ~setNode() {
            delete m_left;
            delete m_right;
        }

        int m_val;
        Colour m_colour;
        setNode *m_parent;
        setNode *m_left;
        setNode *m_right;
    };

    setNode *m_root;

    /// https://gist.github.com/nandor/9249431
    void balance(setNode *newNode) {
        setNode *uncle;
        bool side;

        // dokud mas ty i rodic cervenou barvu (spatne... )
        while (newNode->m_parent && newNode->m_parent->m_colour == RED) {
            // nastav strejdu; neboj nespadne to protoze root je vzdy black...
            if ((side = (newNode->m_parent == newNode->m_parent->m_parent->m_left)))
                uncle = newNode->m_parent->m_parent->m_right;
            else
                uncle = newNode->m_parent->m_parent->m_left;

            /// prijatelne
            if (uncle && uncle->m_colour == RED) {
                newNode->m_parent->m_colour = BLACK;
                uncle->m_colour = BLACK;
                newNode->m_parent->m_parent->m_colour = RED;
                newNode = newNode->m_parent->m_parent;
            }
                ///neprijatelne
            else {
                if (newNode == (side ? newNode->m_parent->m_right : newNode->m_parent->m_left)) {
                    newNode = newNode->m_parent;
                    side ? RotateLeft(newNode) : RotateRight(newNode);
                }

                newNode->m_parent->m_colour = BLACK;
                newNode->m_parent->m_parent->m_colour = RED;
                side ? RotateRight(newNode->m_parent->m_parent) : RotateLeft(newNode->m_parent->m_parent);
            }
        }

        //obnov root color protoze root je vzdy black
        m_root->m_colour = BLACK;
    }

    /// ROTATORS https://gist.github.com/nandor/9249431
    void RotateLeft(setNode *x) {
        setNode *y;

        y = x->m_right;
        x->m_right = y->m_left;
        if (y->m_left)
            y->m_left->m_parent = x;

        y->m_parent = x->m_parent;
        y->m_left = x;

        if (!x->m_parent)
            m_root = y;
        else if (x == x->m_parent->m_left)
            x->m_parent->m_left = y;
        else
            x->m_parent->m_right = y;

        x->m_parent = y;
    }

    void RotateRight(setNode *y) {
        setNode *x;

        x = y->m_left;
        y->m_left = x->m_right;
        if (x->m_right)
            x->m_right->m_parent = y;

        x->m_parent = y->m_parent;
        x->m_right = y;

        if (!y->m_parent)
            m_root = x;
        else if (y == y->m_parent->m_left)
            y->m_parent->m_left = x;
        else
            y->m_parent->m_right = x;

        y->m_parent = x;
    }


public:
    Set() : m_root(nullptr) {};

    ~Set() {
        delete m_root;
    }

    bool insert(int value) {
        setNode *searchNode = m_root;
        setNode *parent = nullptr;
        while (searchNode) {
            parent = searchNode;
            if (searchNode->m_val > value)
                searchNode = searchNode->m_left;
            else if (searchNode->m_val < value)
                searchNode = searchNode->m_right;
            else /// aka. hodnota uz ve strome je!
                return false;
        }

        setNode *newNode;

        if (parent == nullptr)
            newNode = m_root = new setNode(value, nullptr, BLACK);
        else {
            newNode = new setNode(value, parent, RED);
            if (parent->m_val < newNode->m_val)
                parent->m_right = newNode;
            else
                parent->m_left = newNode;
        }

        balance(newNode);

        return true;
    }

    bool find(int value) {
        setNode *searchNode = m_root;
        while (searchNode) {
            if (searchNode->m_val > value)
                searchNode = searchNode->m_left;
            else if (searchNode->m_val < value)
                searchNode = searchNode->m_right;
            else /// aka. hodnota uz ve strome je!
                return true;
        }
        return false;
    }
};

///********************************************************************************************************
///******************************** NODE ******************************************************************
///********************************************************************************************************
class Node {
    int m_constructTime;    // time needed to construct this piece
    int64_t m_startTime;        // time when this piece will be started to build
    bool m_parent;      // sign that node is "root"
    vector<int> m_childs;   // vector of childrens
public:
    explicit Node(int constructTime) : m_constructTime(constructTime), m_startTime(-1), m_parent(false) {};

    int getConstructTime() const { return m_constructTime; } //needed

    void setStartTime(int64_t i) { m_startTime = i; } //needed
    int64_t getStartTime() const { return m_startTime; } //needed
    bool isStartTimeKnown() const { return m_startTime != -1; }   //needed

    void setParentTrue() { m_parent = true; };       //needed
    bool getParent() const { return m_parent; }  //needed

    void addChild(int i) { m_childs.push_back(i); } //needed
    const vector<int> &getChildrens() const { return m_childs; }  //needed
};




///********************************************************************************************************
///********************************* DFS ******************************************************************
///********************************************************************************************************

int64_t dfs(int current, vector<Node> &graph, Set &visited) {
    if (visited.find(current) && !graph[current].isStartTimeKnown())
        throw Cycle();

    if (!graph[current].isStartTimeKnown())
        visitedNodesCntGlobal++;

    if (graph[current].getChildrens().size() == 0) {
        graph[current].setStartTime(0);
        if (graph[current].getConstructTime() + graph[current].getStartTime() > totalTimeGlobal)
            totalTimeGlobal = graph[current].getConstructTime() + graph[current].getStartTime();

        return graph[current].getConstructTime();
    }

    visited.insert(current);

    int64_t timeStart = 0;
    for (int j = 0; j < graph[current].getChildrens().size(); ++j) {
        int childId = graph[current].getChildrens()[j];
        int64_t newTime;

        if (graph[childId].isStartTimeKnown())
            newTime = graph[childId].getConstructTime() + graph[childId].getStartTime();
        else
            newTime = dfs(childId, graph, visited);

        timeStart = newTime > timeStart ? newTime : timeStart;
    }

    graph[current].setStartTime(timeStart);

    if (graph[current].getConstructTime() + graph[current].getStartTime() > totalTimeGlobal)
        totalTimeGlobal = graph[current].getConstructTime() + graph[current].getStartTime();

    return graph[current].getConstructTime() + graph[current].getStartTime();
}


///********************************************************************************************************
///**************************** SEARCH GRAPH **************************************************************
///********************************************************************************************************
bool searchGraph(vector<Node> &graph) {

    for (int i = 0; i < graph.size(); ++i) {
        if (!graph[i].getParent()) {
            Set visited;
            try { dfs(i, graph, visited); }
            catch (Cycle) {
                printf("No solution.\n");
                return false;
            }
        }
    }

    if (graph.size() != visitedNodesCntGlobal) {
        printf("No solution.\n");
        return false;
    }
    return true;
}

///********************************************************************************************************
///****************************** SCAN GRAPH **************************************************************
///********************************************************************************************************
void scanGraph(vector<Node> &graph) {
    int totalPartsCnt;
    scanf("%d", &totalPartsCnt);
    for (int i = 0; i < totalPartsCnt; ++i) {
        int t;
        scanf("%d", &t);
        graph.push_back(Node(t));
    }

    for (int i = 0; i < totalPartsCnt; ++i) {
        int prereqCnt;
        scanf("%d", &prereqCnt);
        for (int j = 0; j < prereqCnt; ++j) {
            int idPart;
            scanf("%d", &idPart);
            graph[i].addChild(idPart);
            graph[idPart].setParentTrue();
        }
    }
}

///********************************************************************************************************
///******************************** MAIN ******************************************************************
///********************************************************************************************************
int main() {
    vector<Node> graph;

    scanGraph(graph);

    if (!searchGraph(graph))
        return 0;

    printf("%ld\n", totalTimeGlobal);

    for (int i = 0; i < graph.size(); ++i)
        printf("%ld ", graph[i].getStartTime());

    printf("\n");

    return 0;
}