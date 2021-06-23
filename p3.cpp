#include <iostream>
using namespace std;

class Array;
class Strategy {
public:
    virtual void execute(Array&) = 0;  
    virtual ~Strategy() {};
};


class Array {
public:
    Array(int length) {
        lenght_ = length;
        arr = new int[lenght_];
    }
    Array(int length, Strategy* strategy) {
        lenght_ = length;
        arr = new int[length];
        strategy_ = strategy;
    }
    Array& fillArray() {
        strategy_->execute(*this);
        return *this;
    }
   

    void In(int* mass) {
        for (int i = 0; i < lenght_; ++i) {
            arr[i] = mass[i];
        }
    }

    int& operator[](int i) {
        return arr[i];
    }

    Array& chStrategy(Strategy* strategy) {
        delete strategy_;
        strategy_ = strategy;
        return *this;
    }

    int set_lenght() {
        return lenght_;
    }

    ~Array() {
        delete[] arr;
        delete strategy_;
    }
private:
    Strategy* strategy_ = nullptr;
    int lenght_ = 0;
    int* arr = nullptr;
};

class Strategy1 : public Strategy {
public:
    Strategy1() {};
    void execute(Array& arr) override {
        int n = arr.set_lenght();
        for (int i = 0; i < n; i++)
        {
            std::cout << arr[i] << std::endl;
        }
    };
};
class Strategy2 : public Strategy {
public:
    Strategy2() {};
    void execute(Array& arr) override {
        int n = arr.set_lenght();
        for (int i = 0; i < n; i++)
        {
            std::cout << arr[i] << ' ';

        }
    };

};


int main() {
    Array arr(6);
    arr.chStrategy(new Strategy1());
    int mas[6] = { 1,2,3,4,5,6 };
    arr.In(mas);
    arr.fillArray();
    arr.chStrategy(new Strategy2());
    arr.fillArray();
}

