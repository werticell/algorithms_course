#include <iostream>
#include<string>
using std::cin;
using std::cout;


class Stack{
public:

    Stack () : array(new char[1]), array_size(0), max_size(1) {}
    ~Stack() { delete []array; }

    Stack(const Stack&) = delete;
    Stack(Stack&&) = delete;
    Stack& operator=(const Stack&) = delete;
    Stack& operator=(Stack&&) = delete;

    void push(char value);
    void pop();
    bool empty();
    int top();

private:
    char* array;
    int max_size;
    int array_size;

    void enlarge();
    void reduce();
};

void Stack::push(char value) {
    if(array_size == max_size)
        enlarge();
    array[array_size] = value;
    ++array_size;
}

void Stack::pop() {
    assert(array_size >= 1);
    --array_size;
    if(array_size < max_size / 4)
        reduce();
}

bool Stack::empty() {
    return array_size == 0;
}

int Stack::top() {
    return array[array_size - 1];
}

void Stack::enlarge() {
    char* new_array = new char[max_size * 2];
    for(int i = 0; i < max_size; ++i)
        new_array[i] = array[i];

    max_size *= 2;
    delete[] array;
    array = new_array;
}

void Stack::reduce() {
    char* new_array = new char[max_size / 2];
    for(int i = 0; i < max_size / 4; ++i)
        new_array[i] = array[i];

    max_size /= 2;
    delete[] array;
    array = new_array;
}




class String{
public:
    String () : array(new char[8]), array_size(0), max_size(8) {}
    ~String() { delete []array; }

    String(const String&) = delete;
    String(String&&) = delete;
    String& operator=(const String&) = delete;
    String& operator=(String&&) = delete;

    void add_char(char value);
    int size() const;
    char operator[](int ind);

private:
    char* array;
    int max_size;
    int array_size;
    void enlarge();
};

void String::add_char(char value) {
    if( array_size == max_size )
        this->enlarge();
    array[array_size] = value;
    array_size += 1;
}

int String::size() const {
    return array_size;
}

char String::operator[](int ind) {
    return array[ind];
}
void String::enlarge() {
    char* new_array = new char[max_size * 2];
    for(int i = 0; i < max_size; ++i)
        new_array[i] = array[i];

    max_size *= 2;
    delete[] array;
    array = new_array;
}




bool isClosing(char bracket) {
    return bracket == ']' || bracket == '}' || bracket == ')';
}
char get_opposite_bracket(char bracket) {
    if( bracket == '{' )
        return '}';
    if( bracket == '[' )
        return ']';
    if( bracket == '(' )
        return ')';
    if( bracket == '}' )
        return '{';
    if( bracket == ']' )
        return '[';
    if( bracket == ')' )
        return '(';
}

void get_line(String& array, int array_size) {
    for(int i = 0; i < array_size; ++i)
        cout << array[i];
}

bool solve_string(Stack& stack, String& input) {
    for(int i = 0; i < input.size(); ++i) {
        if(!isClosing(input[i]))
            stack.push(input[i]);
        else
        {
            if(stack.top() == get_opposite_bracket(input[i]))
                stack.pop();
            else {
                if(stack.empty() || isClosing(stack.top()))
                    stack.push(input[i]);
                else
                    return false;
            }
        }
    }
    return true;
}

void make_string(String& prefix, String& suffix, Stack& stack) {
    while(!stack.empty()) {
        if (!isClosing(stack.top())) {
            suffix.add_char(get_opposite_bracket(stack.top()));
        } else
            prefix.add_char(get_opposite_bracket(stack.top()));
        stack.pop();
    }
}

int main() {
    String str_input;
    Stack stack;

    char tmp;
    while(cin >> tmp  && tmp != 'a')
        str_input.add_char(tmp);

    String prefix, suffix;
    if(solve_string(stack, str_input)) {
        make_string(prefix, suffix, stack);
        get_line(prefix, prefix.size());
        get_line(str_input, str_input.size());
        get_line(suffix, suffix.size());
    }
    else
        cout << "IMPOSSIBLE";

    return 0;
}


