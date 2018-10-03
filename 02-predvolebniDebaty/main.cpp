#include <iostream>

class AVLTree {
private:
    struct Node
    {
        Node(int value) : m_value(value), m_left(nullptr), m_right(nullptr), m_parent(nullptr), m_predaccessors(0), m_leftDepth(0), m_rightDept(0) {}
        int m_value;
        Node * m_left;
        Node * m_right;
        Node * m_parent;
        int m_predaccessors;
        int m_leftDepth;
        int m_rightDept;

        ~Node() { delete m_left;delete m_right; }

        void print()
        {
            if(m_left)
                m_left->print();
            printf("%d, ", m_value);
            if(m_right)
                m_right->print();
        }
    };

    Node * m_root;
    int m_size;

    int absMax(int a, int b)
    {
        if(a < 0)
            a *= -1;
        if(b < 0)
            b*= -1;

        return a > b ? a : b;
    }

/// rotate Right
void rotateR(Node * y, Node * x) // todo - mozna chyba
{
    int leftYcnt = y->m_predaccessors;
   // int leftXcnt = x->m_predaccessors;

    y->m_parent = x->m_parent;

    // prepoj b
    x->m_left = y->m_right;
    if(y->m_right)
        y->m_right->m_parent = x;

    // prepoj x y
    y->m_right = x;
    x->m_parent = y;

    if(y->m_parent) // povodne parent x
    {
        if(y->m_parent->m_right == x)
            y->m_parent->m_right = y;
        else
            y->m_parent->m_left = y;
    }

    //y stejne
    x->m_predaccessors -= (leftYcnt + 1);

    //uprav vejsky ; nejdriv ten co bude ten niz - tj x
    x->m_leftDepth = -1 * y->m_rightDept;
    y->m_rightDept = absMax(x->m_leftDepth,x->m_rightDept) + 1;
    return;
}

/// rotate Left
void rotateL(Node * y, Node * z) // todo - mozna chyba
{
    int underYleft = y->m_predaccessors;

    z->m_parent = y->m_parent; // 1/2 prepojeni hlavniho otce

    //prepoj b
    y->m_right = z->m_left;
    if(z->m_left)
        z->m_left->m_parent = y;

    // prepoj y z
    z->m_left = y;
    y->m_parent = z;

    // 2/2 prepojeni hlavniho parrenta
    if(z->m_parent) // puvodne parrent y
    {
        if(z->m_parent->m_right == y)
            z->m_parent->m_right = z;
        else
            z->m_parent->m_left = z;
    }

    z->m_predaccessors += (underYleft + 1);

    y->m_rightDept = -1 * z->m_leftDepth;
    z->m_leftDepth = -1 * ( absMax(y->m_leftDepth, y->m_rightDept) + 1 );
    return;
}

public:
    AVLTree() : m_root(nullptr), m_size(0) {}
    ~AVLTree() { delete m_root; }

    void print() { m_root->print(); }

struct data
{
    data(Node * node, int sig) : m_node(node), m_sig(sig) {}
    Node * m_node;
    int m_sig;
};

    data removeNode(Node * node)
    {
        if(node->m_left == nullptr && node->m_right == nullptr)
        {
            delete node;
            return data(nullptr, 1);
        }

        else if(node->m_left == nullptr)
        {
            Node * tmp = node->m_right;
            node->m_right = nullptr;
            delete node;
            return data(tmp, 1);
        }

        else
        {
            Node * tmp = node->m_left;
            node->m_left = nullptr;
            delete node;
            return data(tmp,1);
        }
    }

///*************************** REMOVE **********************************************************************************
    data BVSRemove(Node * node, int val)
    {
        int sig = 0;

        if(node == nullptr)
            exit (0); // preventivne nikdy se nemuze stat

        if (val == node->m_value) // delete this node
            return removeNode(node);


        /// POSILAME MAZAT DOLEVA - ODPOVED PRIJDE ZLEVA
        else if(val < node->m_value ) // val < node
        {
            node->m_predaccessors--; // ANO, VLEVO UBYDE
            data dat = BVSRemove(node->m_left, val);

            node->m_left = dat.m_node; // MUZE BYT UZ NULL

            if(node->m_left)
                node->m_left->m_parent = node;
            sig = dat.m_sig;

            if(sig)  /// balance code
            {
                ++node->m_leftDepth;

                if( node->m_leftDepth + node->m_rightDept == 1)
                    sig = 0;

                else if(node->m_leftDepth + node->m_rightDept == +2)
                {
                    Node * y =  node->m_right; /// existuje y ? ano mel by

                    if(y->m_leftDepth + y->m_rightDept == +1) // +1 aka. h+1 h h ->right rotation
                    {
                        rotateL(node, y); // ten vys, ten niz !
                        node = y;
                        sig=1;
                    }
                    else if (  y->m_leftDepth + y->m_rightDept == 0 )
                    {
                        rotateL(node, y); // ten vys, ten niz !
                        node = y;
                        sig=0;
                    }
                    else // -1
                    {
                        Node * z = y->m_left; // ANO MELO BY BYT OK
                        rotateR(z, y); // ten niz, ten vys !!
                        rotateL(node, z); // ten vys, ten niz !!
                        node = z;
                        sig=1;
                    }

                }
            }
        }

        else // val > node
        {
            data dat = BVSRemove(node->m_right, val);
            node->m_right = dat.m_node;
            if(node->m_right)
                node->m_right->m_parent = node; // !! todo pozor ten node nemusi existovat uz !!!!
            sig = dat.m_sig;

            if(sig)
            {
                --node->m_rightDept;
                if( node->m_leftDepth + node->m_rightDept == -1)
                    sig = 0;

                else if(node->m_leftDepth + node->m_rightDept == -2) // todo prekontrolovat
                {
                    Node * y =  node->m_left;
                    if(y->m_leftDepth + y->m_rightDept == -1) // -1 aka. h+1 h h ->right rotation
                    {
                        rotateR(y, node); // ten niz, ten vys !!
                        node = y; // important
                        sig=1;
                    }
                    else if( y->m_leftDepth + y->m_rightDept == 0)
                    {
                        rotateR(y, node); // ten niz, ten vys !!
                        node = y; // important
                        sig=0;
                    }
                    else // +1
                    {
                        Node * z = y->m_right;
                        rotateL(y, z); // ten vys ten niz
                        rotateR(z, node); // ten niz ten vys
                        node = z; // important
                        sig=1;
                    }

                }
            }
        }

        return data(node, sig);
    }

    void remove(int x)
    {
        int pos = containsValue(x);

        if(pos == -1 )
            return;

        Node * toDelete = BVScontainsValueRefenernce(m_root, m_root->m_predaccessors,x);

        if(toDelete->m_right && toDelete->m_left)
        {
            int tmpVal = getValueAtPosition(pos+1); // uloz hodnotu naslednika
            remove(tmpVal); // smaz naslednika
            toDelete->m_value = tmpVal; // na hodnotu x uloz hodnotu naslednika
        }

        else
        {
            --m_size;
            m_root = BVSRemove(m_root, x).m_node;
        }

        return;
    }

///*************************** INSERT **********************************************************************************
    data BVSInsert(Node * node, int val) // TODO - velka pravdepodobnost chyby
    {
        if(node == nullptr)
            return data( new Node(val), 1);

        int sig = 0;

        if(val < node->m_value )
        {
            node->m_predaccessors++;
            data dat = BVSInsert(node->m_left, val);
            node->m_left = dat.m_node;
            node->m_left->m_parent = node;
            sig = dat.m_sig;

            if(sig ) /// balance code
            {
                --node->m_leftDepth;
                if( node->m_leftDepth + node->m_rightDept == 0)
                    sig = 0;
                else if(node->m_leftDepth + node->m_rightDept == -2)
                {
                    Node * y =  node->m_left;
                    if(y->m_leftDepth + y->m_rightDept == -1) // -1 aka. h+1 h h ->right rotation
                    {
                        rotateR(y, node); // ten niz, ten vys !!
                        node = y; // important
                    }
                    else // +1
                    {
                        Node * z = y->m_right;
                        rotateL(y, z); // ten vys ten niz
                        rotateR(z, node); // ten niz ten vys
                        node = z; // important
                    }

                    sig=0;
                }
            }
        }

        else if ( val > node->m_value)
        {
            data dat = BVSInsert(node->m_right, val);
            node->m_right = dat.m_node;
            node->m_right->m_parent = node;
            sig = dat.m_sig;

            if(sig ) /// balance code
            {
                ++node->m_rightDept;
                if( node->m_leftDepth + node->m_rightDept == 0)
                    sig = 0;
                else if(node->m_leftDepth + node->m_rightDept == 2)
                {
                    Node * y =  node->m_right;

                    if(y->m_leftDepth + y->m_rightDept == +1) // +1 aka. h+1 h h ->right rotation
                    {
                        rotateL(node, y); // ten vys, ten niz !
                        node = y;
                    }
                    else // -1
                    {
                        Node * z = y->m_left;
                        rotateR(z, y); // ten niz, ten vys !!
                        rotateL(node, z); // ten vys, ten niz !!
                        node = z;
                    }

                    sig=0;
                }
            }
        }

        return data(node, sig );
    }

    void insert(int x)
    {
        if(containsValue(x) != -1 )
            return;

        m_size ++;

        m_root = BVSInsert(m_root, x).m_node;
        return;
    }

///************************* BOUND HELPERS *****************************************************************************
    Node * elemHelper( Node * neverNull, Node * b) { return b == nullptr ? neverNull : b; }// TODO - done

    Node * getElemOrBesideElemWithValue(Node * curr, int value) // TODO - done
    {
        if(curr == nullptr)
            return nullptr;

        if(value == curr->m_value)
            return  curr;

        if(value < curr->m_value)
            return elemHelper(curr, getElemOrBesideElemWithValue(curr->m_left, value));

        else
            return elemHelper(curr, getElemOrBesideElemWithValue(curr->m_right, value));
    }

///*************************** LOWER BOUND *****************************************************************************
    int loverBound(int value) // TODO done
    {
        Node * res = getElemOrBesideElemWithValue(m_root, value);

        if(res == nullptr)
            return -1;

        if(res->m_value == value ||  res->m_value > value) // vetsi rovno
            return containsValue(res->m_value);

        else                                               // pokud mensi -> musis vzit dalsiho, bude vetsi
        {
            int pos = containsValue(res->m_value);
            return pos + 1 == m_size ? -1 : pos + 1;
        }
    }

///************************* LOWER LOWER BOUND *************************************************************************
    int loverLoverBound(int value) // TODO done
    {
        Node * res = getElemOrBesideElemWithValue(m_root, value);

        if(res == nullptr)
            return -1;

        if(res->m_value == value ||  res->m_value < value) // mensi rovno
            return containsValue(res->m_value);

        else                                               // pokud vetsi -> musis vzit predchoziho, bude mensi
            return (containsValue(res->m_value) - 1); // kdyz bude 0, da mi to -1 a to je srpavne :)
    }

///*************************** CONTAINS ********************************************************************************
    int BVScontainsValue(Node *node, int currPos, int value) // TODO done
    {
        if(node == nullptr)
            return -1;

        if(node->m_value == value)
            return currPos;

        if(value < node->m_value && node->m_left != nullptr)
            return BVScontainsValue(node->m_left, currPos - node->m_predaccessors + node->m_left->m_predaccessors, value);

        if(value > node->m_value && node->m_right != nullptr)
            return BVScontainsValue(node->m_right, currPos + 1 + node->m_right->m_predaccessors, value);

        return  -1;
    }

    Node * BVScontainsValueRefenernce(Node *node, int currPos, int value) // TODO done
    {
        if(node == nullptr)
            return nullptr;

        if(node->m_value == value)
            return node;

        if(value < node->m_value && node->m_left != nullptr)
            return BVScontainsValueRefenernce(node->m_left, currPos - node->m_predaccessors + node->m_left->m_predaccessors, value);

        if(value > node->m_value && node->m_right != nullptr)
            return BVScontainsValueRefenernce(node->m_right, currPos + 1 + node->m_right->m_predaccessors, value);

        return nullptr;
    }

    const int containsValue(int value) // returns position of elem; if elem dont exists, return -1
    { return m_root == nullptr ? -1 : BVScontainsValue(m_root, m_root->m_predaccessors, value); }

///*************************** GET VALUE AT POSITION *******************************************************************
    int BVSgetValueAtPosition(Node * node, int currPos, int pos) // TODO done
    {
        if(node == nullptr)
            return -1; // nemelo by nikdy nastat !

        if(currPos == pos)
            return node->m_value;

        if(pos < currPos && node->m_left != nullptr)
            return BVSgetValueAtPosition(node->m_left, currPos - node->m_predaccessors + node->m_left->m_predaccessors  ,pos);

        if(pos > currPos && node->m_right != nullptr)
            return BVSgetValueAtPosition(node->m_right, currPos + 1 + node->m_right->m_predaccessors, pos);

        return -1; // nemelo by nastat
    }

    int getValueAtPosition(int x) // returns -1 if position is out of bound
    { return m_root == nullptr ? -1 : BVSgetValueAtPosition(m_root, m_root->m_predaccessors, x); }
};


///*********************************************************************************************************************
///*********************************************************************************************************************
///*********************************************************************************************************************

int main() {
    int op, val;
    AVLTree mySet;

    while (true) {
        scanf("%d", &op);

        if (op == 1) // add
        {
            scanf("%d", &val); mySet.insert(val);
        }
        else if (op == 2) //remove
        {
            scanf("%d", &val);
            mySet.remove(val);
        }
        else if (op == 3) //median
        {
            int from, to; scanf("%d%d", &from, &to);
            int posFrom = mySet.loverBound(from);
            int posTo = mySet.loverLoverBound(to);

            if (posFrom == -1 || posTo == -1 || posFrom > posTo)
                printf("notfound\n");
            else
                printf("%d\n", mySet.getValueAtPosition(posFrom + (posTo - posFrom) / 2)); // +1 ?
        }

        else
            break;
    }

    return 0;
}