// Saber_Programming.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <cmath>
#include <random>
#include <map>
#include <sstream>

using namespace std;


struct ListNode {
    ListNode* prev;
    ListNode* next;
    ListNode* rand; // указатель на произвольный элемент или null;
    string data;
};

class List {
private:
    ListNode* head;
    ListNode* tail;
    int count =0;

public:
    ListNode* Add(string data) {
        ListNode *node = new ListNode;
        if (count== 0) {
            head = node;
            (*node).prev = nullptr;
        }
        else {
            tail->next = node;
              (*node).prev = tail;
        }
                    
        tail = node;
        count++;
        (*node).data = data;
        (*node).next = nullptr;
        (*node).rand = nullptr;
        return node;
    }

    void Display(); // Displays the list
    void GetRandomNodeForAll(); // Gets a random or nullptr for all nodes in the list
    void Serialize(FILE* file);  // Сохранение в файл (файл открыт с помощью  fopen(path,"wb"));
    void Deserialize(FILE* file);// Загрузка из файла (файл открыт с помощью fopen(path,"rb"));

    template<typename T>
    bool Write(FILE* file, T value); //Writes the value in a file;
    template<typename T>
    bool Read(FILE* file, T* value); // Reads the value from a file;
};

void List::Display() {
    for (ListNode* i = head; i != NULL; i = i->next) {
        cout << i->data;
        if (i->rand != nullptr)
            cout << " rand->" << i->rand->data << endl;
        else
            cout << endl;
    }
}

template<typename T>
bool List::Write(FILE* file, T value) {
    if (!file)
        return false;

    fwrite(&value, sizeof(value), 1, file);
    size_t t = sizeof(value);
    return true;
}
template<>
bool List::Write(FILE* file, string value)
{
    if (!file)
        return false;

    uint32_t size = static_cast<std::uint32_t>(value.size());
    Write(file, size);
    fwrite(value.c_str(), size, 1, file);
    return true;
}

template<typename T>
bool List::Read(FILE* file, T* value)
{
    if (!file)
        return false;

    fread(value, 1, sizeof(T), file);
    return true;
}
template<>
bool List::Read(FILE* file, string* s) {
    if (!file)
        return false;

    uint32_t size;
    fread(&size, 1, sizeof(int32_t), file);

    char* buf = new char[size + 1];
    fread(buf, 1, size, file);
    buf[size] = '\0';
    *s = string(buf);
    return true;
}

void List::GetRandomNodeForAll() {
    random_device rd;
    mt19937 gen;
    gen.seed(rd());
    uniform_int_distribution<> rand(0, count);
    for (ListNode* i = head; i != NULL; i = i->next)
    {
        ListNode* j = head;
        for (int r = rand(gen); r != 0; r--) {
            if (r == count) {
                j = nullptr;
                break;
            }
            j = j->next;
        }
        i->rand = j;

    }
}

void List::Serialize(FILE* file)
{
    Write(file, count);
    for (ListNode* node = head; node != NULL; node = node->next) {
        Write(file, node);
        Write(file, node->rand);
        Write(file, node->data);
    }

}

void List::Deserialize(FILE* file)
{
    int num;
    vector<vector<string> > ivec;

    Read(file, &num);
    ivec.resize(num);
    ListNode* one;
    ListNode* two;

    for (int i = 0; i < num; i++) {
        ListNode* node = new ListNode;

        Read(file, &one);
        Read(file, &two);
        Read(file, &(node->data));

        ostringstream first, second; //Избыточность для читаемости
        first << one;
        second << two;

        ivec[i].push_back(first.str());
        ivec[i].push_back(second.str());
        this->Add(node->data);

    }

    //Algorithm for linkinig of random nodes;
    int i = 0;
    for (ListNode* n = head; n != NULL; n = n->next) {
        int nodenumber = -1;
        n->rand = nullptr;
        for (int j = 0; j < ivec.size(); j++)
            if (ivec[i][1] == ivec[j][0])
            {
                nodenumber = j;
                break;
            }

        ListNode* k = head;
        if (nodenumber == -1)
            n->rand = nullptr;
        else {
            for (int g = nodenumber; g != 0; g--)
                k = k->next;
            n->rand = k;
        }
        i++;
    }

}


void own_int_to_binary(int n);
void RemovePups(char* str);

int main()
{
    List F,S,T;
    FILE* ptrFile;

    cout << "----first -----" << endl;
    F.Add("First");
    F.Add("Second");
    F.Add("Third");
    F.Add("Fourth");
    F.GetRandomNodeForAll();
    F.Display();


    fopen_s(&ptrFile, "file.txt", "wb");
    F.Serialize(ptrFile);
    fclose(ptrFile);

    cout << "-----Copy of the first ------" << endl;
    fopen_s(&ptrFile, "file.txt", "rb");
    S.Deserialize(ptrFile);
    fclose(ptrFile);
    S.Display();
   
    cout << "-----Added one more line and changed all random nodes ------" << endl;
    fopen_s(&ptrFile, "file.txt", "wb");
    S.Add("Fifth");
    S.GetRandomNodeForAll();
    S.Display();
    S.Serialize(ptrFile);
    fclose(ptrFile);

    
    cout << "-----Copy of the second ------" << endl;
    fopen_s(&ptrFile, "file.txt", "rb");
    T.Deserialize(ptrFile);
    fclose(ptrFile);
    T.Display();

    
    cout << "\n \n \n";
    char data[] = "ABAA BBCASB ASDFVSSDVWWRV SD";
    cout << "---data before---" << endl << data << endl;
    RemovePups(data);
    cout << "---data after ---" << endl << data << "\n \n \n";


    int a = -1248124;
    cout << "Number: " << a << endl;
    own_int_to_binary(a);
}



void own_int_to_binary(int n) // first bit is a sign then binary numbers
{
    int size = (sizeof(int) - 1) * 8;
    char* a = new char[size];

    //Sign
    if (n < 0) {
        cout << "1 | ";
        n = -n;
    }
    else cout << "0 | ";

    // Zero
    if (n == 0) {
        cout << '0' << endl;
        delete[]a;
        return;
    }

    //Algorithm
    for (int i = size - 1; i >= 0; i--)
    {
        if (n / pow(2, i) >= 1) {
            n -= pow(2, i);
            a[size - 1 - i] = '1';
        }
        else
            a[size - 1 - i] = '0';
    }

    //Display;
    int i = 0;
    while (a[i] != '1')
        i++;

    for (i; i < size; i++)
        cout << a[i] << " ";

    cout << endl;
    delete[]a;
}

void RemovePups(char* str)
{
    for (int i = 1; i < strlen(str); i++)
        if (str[i - 1] == str[i]) {
            for (int j = i; str[j] != '\0'; j++)
                str[j] = str[j + 1];
            i--;
        }
}


