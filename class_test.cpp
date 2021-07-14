#include <iostream>

class C2;

class C1 {

private:
    C2 c2_;
};

class C2 {
    C2(C1 c1) : c1_(c1) {}
private:
    C1 c1_;
};

int main() {
    C1 c1;
    C2 c2(c1);
}