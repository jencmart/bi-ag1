#include <iostream>
#include <cstdlib>     /* qsort */

struct Edge
{
    int m_from;
    int m_to;
    int m_len;
    int m_id;
    Edge(int from, int to, int len, int id) : m_from(from), m_to(to), m_len(len) ,m_id(id){}
};

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

    static int sortBy (Edge **r1, Edge **r2)
    { return (*r1)->m_len - (*r2)->m_len; }

    static int compare (const void * a, const void * b)
    {

        auto *edgeA = (Edge *)a;
        auto *edgeB = (Edge *)b;


     //   return (edgeA->m_len > edgeB->m_len) - (edgeA->m_len < edgeB->m_len);

        if(edgeA->m_len > edgeB->m_len)
            return 1;
        if(edgeA->m_len < edgeB->m_len)
            return -1;
        return 0;
    }


    void sort()
    {
        qsort (m_first, m_size, sizeof(myType*), (int (*)(const void *,const void *)) sortBy);
    }
};



uint64_t soucetDelekUlic;
int globalV;



void kruskalMSP(vector<Edge> &graph);

int main()
{
    int n, m;
    scanf("%d%d",&n,&m);
    globalV = n;

    vector<Edge> edges;
    for (int i = 0; i < m; ++i)
    {
        int x,y,k;
        scanf("%d%d%d", &x, &y,&k);
        edges.push_back(Edge(x,y,k, i));
    }

    edges.sort();


    kruskalMSP(edges);

    return 0;
}

struct UnionNode {
    int m_parent;
    int m_rank;

    UnionNode(int parent) : m_parent(parent), m_rank(0) {};
};

int findParrent(int x, vector<UnionNode> &union_rank) {
    if (x != union_rank[x].m_parent)
        union_rank[x] = findParrent(union_rank[x].m_parent, union_rank);
    return union_rank[x].m_parent;
}

void unionInsert(int x, int y, vector<UnionNode> &union_rank) {
    int rootX = findParrent(x, union_rank);
    int rootY = findParrent(y, union_rank);

    if (union_rank[rootX].m_rank < union_rank[rootY].m_rank)
        union_rank[rootX].m_parent = rootY;

    else if (union_rank[rootX].m_rank > union_rank[rootY].m_rank)
        union_rank[rootY].m_parent = rootX;

    else {
        union_rank[rootY].m_parent = rootX;
        union_rank[rootX].m_rank++;
    }
}


struct Node
{
    Node(int to) : m_to(to), m_selected(false){}
    int m_to;
    bool m_selected;
};


void dfsSelect(int currPos, vector<vector<int>>  & spannigTree, vector<bool> & selected, vector<bool> & opened, vector<bool> & closed)
{
    if(closed[currPos] || opened[currPos])
        return;

    if(spannigTree[currPos].size() == 1 && opened[spannigTree[currPos][0]] == true )
    {
        closed[currPos] = true;
        return;
    }

    opened[currPos] = true;

    for (int i = 0; i < spannigTree[currPos].size(); ++i)
    {
        if(closed[spannigTree[currPos][i]] || opened[spannigTree[currPos][i]])
            continue;
        dfsSelect(spannigTree[currPos][i], spannigTree, selected, opened, closed);
        if(selected[spannigTree[currPos][i]] == false)
        {
            selected[currPos] = true;
        }
    }

    closed[currPos] = true;
}


/// toto cele by melo take bezet  O(e * log e )
/// nejhure projdi 200 000 uzlu
/// pro kazdy vyhledej log n predchudce. (udajne log n) ja to nedokazu spocitat
void kruskalMSP(vector<Edge> &graph)
{
    vector<UnionNode> unionRank;
    vector<int> seznamIdUlic;

    vector<vector<int>> spannigTree;

    for (int i = 0; i < globalV; ++i)
        spannigTree.push_back(vector<int>());

    for (int i = 0; i < globalV; ++i)
        unionRank.push_back(UnionNode(i));

    //unsigned finalCount = 0;
    int cnt = 0;
    int rootPos = 0;
    for (int j = 0; j < graph.size(); ++j) {
        int parentA = findParrent(graph[j].m_from, unionRank);
        int parentB = findParrent(graph[j].m_to, unionRank);

        if (parentA != parentB) {
            unionInsert(parentA, parentB, unionRank);
            soucetDelekUlic += graph[j].m_len;
            cnt++;
            seznamIdUlic.push_back(graph[j].m_id);
            spannigTree[graph[j].m_from].push_back(graph[j].m_to);
            spannigTree[graph[j].m_to].push_back(graph[j].m_from);
            rootPos = graph[j].m_from;
        }

        if (cnt == globalV - 1)
            break;
    }

    vector<bool> selected;
    vector<bool> opened;
    vector<bool> closed;

    for (int i = 0; i < globalV; ++i)
    {
        selected.push_back(false);
        closed.push_back(false);
        opened.push_back(false);
    }

    dfsSelect(rootPos, spannigTree, selected,opened, closed);

// delky ulic, pocet mest
    // seznam ulic
    // seznam mest

    int cntCity = 0;

    for (int l = 0; l < selected.size(); ++l)
    {
        if(selected[l])
            cntCity++;
    }


    printf("%lu %d\n", soucetDelekUlic, cntCity);
    for (int k = 0; k < seznamIdUlic.size(); ++k)
    {
        printf("%d ", seznamIdUlic[k]);
    }

    printf("\n");
    for (int l = 0; l < selected.size(); ++l)
    {
        if(selected[l])
            printf("%d ", l);
    }
}
