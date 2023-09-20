#include <concepts>
#include <string>
#include <iostream>

//концепт, который накладывает следующие ограничения на тип Т:
template <typename T> concept ComplexConcept = requires(const T v) { 
    {v.hash()}->std::convertible_to<long>;      //наличие у типа T метода hash(), который возвращает тип, конвертируемый в long
    {v.toString()}->std::same_as<std::string>;  //наличие у типа T метода toString(), который возвращает std::string
}
&& !std::has_virtual_destructor<T>::value;      //отсутствие у типа T виртуального деструктора

//печать результатов
template <ComplexConcept T> void printingResults(T val) {
    std::cout << "\nValue " << val._value << ":" << std::endl;
    std::cout << "hash()     -\t" << val.hash() << std::endl;
    std::cout << "toString() -\t" << val.toString() << std::endl;
    std::cout << "virtual ~  -\t" << std::has_virtual_destructor<T>::value << std::endl;
}

//структура1, соответствующая концепту
struct Struct1 {
    int _value = 0;
    Struct1(auto const val) :_value(val) {};
    ~Struct1() = default;
    auto hash() const {
        return _value << 5;
    }
    auto toString() const {
        return "string " + std::to_string(_value);
    }
};

//структура2, не соответствующая концепту: метод hash() возвращает тип string, не конвертируемый в long;
struct Struct2 {
    std::string _value = "";
    Struct2(auto const val) :_value(val) {};
    ~Struct2() = default;
    std::string hash() const {
        return "hash" + _value;
    }
    std::string toString() const {
        return "string " + _value;
    }
};

//структура3, не соответствующая концепту: метод toString() возвращает int
struct Struct3 {
    int _value = 0;
    Struct3(auto const val) :_value(val) {};
    ~Struct3() = default;
    long hash() const {
        return static_cast<long>(_value << 5);
    }
    int toString() const {
        return _value;
    }
};

//структура4, не соответствующая концепту: есть виртуальный деструктор
struct Struct4 {
    int _value = 0;
    Struct4(auto const val) :_value(val) {};
    virtual ~Struct4() = default;
    auto hash() const {
        return _value << 5;
    }
    auto toString() const {
        return "string" + std::to_string(_value);
    }
};


int main() {
    Struct1 s11(1234567);
    Struct1 s12(1.23);
    Struct1 s13(-1234567);

    Struct2 s2("22222");
    Struct3 s3(33333);
    Struct4 s4(44444);

    printingResults<Struct1>(s11);
    printingResults<Struct1>(s12); 
    printingResults<Struct1>(s13);

    //printingResults<Struct2>(s2);         //ошибка: метод hash() возвращает тип, не конвертируемый в long;
    //printingResults<Struct3>(s3);         //ошибка: метод toString() возвращает int
    //printingResults<Struct4>(s4);         //ошибка: есть виртуальный деструктор

    return 0;
}