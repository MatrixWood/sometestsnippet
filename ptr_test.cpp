#include <memory>
#include <vector>
#include <iostream>
#include <list>

int main() {
    std::list<std::unique_ptr<int>> vec_ptr;
    for (int i = 0; i < 5; ++i) {
        vec_ptr.push_back(std::make_unique<int>(i));
    }

    std::cout << "before vec_ptr size: " << vec_ptr.size() << std::endl;
    std::unique_ptr<int> one_ptr = std::move(vec_ptr.back());
    std::cout << "after vec_ptr size: " << vec_ptr.size() << std::endl;


    int* i = new int(22);
    std::unique_ptr<int> uptr(i);

    int* j = uptr.get();

    int ii = 33;
    j = &ii;

    return 0;
}