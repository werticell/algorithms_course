#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::string;

int Hash(const string& key, int& bufferSize) {
    int resultHash = 0;
    for (const auto& x : key)
        resultHash = (resultHash * 127 + x) % bufferSize;

    return resultHash;
}

int SecondHash(const string& key, int& bufferSize) {
    int resultHash = 0;
    for (const auto& x : key)
        resultHash = (resultHash * 3 + x) % bufferSize;

    if (resultHash % 2 == 0)
        ++resultHash;
    if (resultHash == 0)
        resultHash = 5 % bufferSize;
    return resultHash;
}


template <typename T, int(*f1)(const T&, int& size),int(*f2)(const T&, int& size)>
class HashTable
{
private:
    int (*firstHash)(const T&, int& size);
    int (*secondHash)(const T&, int& size);

    /*
     * EMPTY - ячейка пуста,
     * FILLED - не пуста и элемент присутствует,
     * DELETED - когда-то было не пуста, но сейчас тот элемент удален
    */
    enum type {
        EMPTY,
        FILLED,
        DELETED
    };

    struct Node {
        T key;
        type status; //
        explicit Node(const T& key) : status(1), key(key) {}
        Node() : status(EMPTY) {}
    };

    Node* buffer;
    int keyCount;
    int bufferSize;
    int deletedCount;

public:

    HashTable() {
        firstHash = *f1;
        secondHash = *f2;

        keyCount = 0;
        deletedCount = 0;
        bufferSize = 8;
        buffer = new Node[bufferSize];

        for(int i = 0; i < bufferSize; ++i)
            buffer[i].status = EMPTY;
    }

    ~HashTable() { delete []buffer; }

    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    bool has(const T& key);
    bool add(const T& key);
    bool erase(const T& key);

private:

    void enlarge() {
        Node* reserve = buffer;
        bufferSize *= 2;
        keyCount = 0;
        deletedCount = 0;
        buffer = new Node[bufferSize];


        for (int i = 0; i < bufferSize; ++i) // обнулим
            buffer[i].status = EMPTY;

        for (int i = 0; i < bufferSize / 2; ++i) {
            if(reserve[i].status != 0 ) { // если указатель не пустой и элемент не был удален ранее
                if(reserve[i].status == 1 ) // если элемент присутствует
                    add(reserve[i].key);

            }
        }
        delete [] reserve;

    }

    void rehash() {
        Node* reserve = buffer;

        keyCount = 0;
        deletedCount = 0;
        buffer = new Node[bufferSize];

        for (int i = 0; i < bufferSize; ++i)
            buffer[i].status = EMPTY;

        for (int i = 0; i < bufferSize; ++i) {
            if(reserve[i].status != 0) { // если ячейка не пуста и элемент не был удален ранее
                if(reserve[i].status == 1) // если элемент присутствует
                    add(reserve[i].key);
            }
        }
        delete [] reserve;
    }
};


template<typename T, int(*f1)(const T&, int& size),int(*f2)(const T&, int& size)>
bool HashTable<T, f1, f2>::has(const T& key)
{
    int currentHash = firstHash(key, bufferSize);
    int step = secondHash(key, bufferSize);

    for (int probe = 0; probe < bufferSize; ++probe) {
        currentHash = (currentHash + probe * step) % bufferSize;
        if (buffer[currentHash].status != 0) {
            if (buffer[currentHash].key == key && buffer[currentHash].status == 1) // если этот элемент не был удален раннее и при этом равен по ключу
                return true;
        }
        else
            return false;
    }
    return false;
}

template<typename T, int(*f1)(const T&, int& size),int(*f2)(const T&, int& size)>
bool HashTable<T, f1, f2>::erase(const T& key)
{
    int currentHash = firstHash(key, bufferSize);
    int step = secondHash(key, bufferSize);

    for (int probe = 0; probe < bufferSize; ++probe) {
        currentHash = (currentHash + probe * step) % bufferSize;
        if (buffer[currentHash].status != EMPTY) {
            if (buffer[currentHash].key == key && buffer[currentHash].status == FILLED) { // если ячейка не пуста и элемент не удален, то удалим
                buffer[currentHash].status = DELETED;
                --keyCount;
                ++deletedCount;
                return true;
            }
        }
        else
            return false;

    }
    return false;
}



template<typename T, int(*f1)(const T&, int& size),int(*f2)(const T&, int& size)>
bool HashTable<T, f1, f2>::add(const T& key)
{
    if(4 * (keyCount + 1) >=  3 * bufferSize)
        enlarge();

    if(4 * deletedCount >= 3 * bufferSize)
        rehash();

    int currentHash = firstHash(key, bufferSize);
    int step = secondHash(key, bufferSize);
    bool deleted_found = false;
    int deleted_ind = 0;

    for (int probe = 0; probe < bufferSize; ++probe) {
        currentHash = (currentHash + probe * step) % bufferSize;
        if (buffer[currentHash].status == EMPTY) {
            if(!deleted_found) { // если и до этого не было ни одного удаленного, то добавляем
                buffer[currentHash].key = key;
                buffer[currentHash].status = FILLED;
                ++keyCount;
                return true;
            }
            else {
                buffer[deleted_ind].key = key;
                buffer[deleted_ind].status = FILLED;
                ++keyCount;
                --deletedCount;
                return true;
            }

        }
        else {
            if(buffer[currentHash].key == key && buffer[currentHash].status == FILLED) // если нашли ключ который хотели добавить, то вернуть false
                return false;
            if(buffer[currentHash].status == DELETED && !deleted_found) { // если нашли удаленный элемент и он первый, то запомним его
                deleted_found = true;
                deleted_ind = currentHash;
            }
        }
    }
    if(deleted_found) { // если обошли все ячейки и пришли сюда, значит нашего ключа в таблице нет, и можно добавить в первый найденный deleted
        buffer[deleted_ind].key = key;
        buffer[deleted_ind].status = FILLED;
        ++keyCount;
        --deletedCount;
        return true;
    }
    else
        return false;
}


int main() {
    HashTable<string, Hash, SecondHash> myHashTable;
    char command;
    string word;
    bool resultOfCommand = false;

    while (cin >> command >> word) {
        if (command == '+')
        {
            resultOfCommand = myHashTable.add(word);
        }
        if (command == '-')
        {
            resultOfCommand = myHashTable.erase(word);
        }
        if (command == '?')
        {
            resultOfCommand = myHashTable.has(word);
        }
        if (resultOfCommand)
            cout << "OK" << std::endl;
        else
            cout << "FAIL" << std::endl;
    }
    return 0;
}

