#include <iostream>
#include <vector>
#include <ctime>
#include <list>

using namespace std;

// ��������� ��� ��������� �������
class TurnOnStrategy {
public:
    virtual void TurnOn(const string& name) = 0;
    virtual ~TurnOnStrategy() {}
};

class NormalStart : public TurnOnStrategy {
public:
    void TurnOn(const string& name) override {
        cout << name << " ������� � ������� ������." << endl;
    }
};

class EcoStart : public TurnOnStrategy {
public:
    void TurnOn(const string& name) override {
        cout << name << " ������� � ����������������� ������." << endl;
    }
};

class PreheatStart : public TurnOnStrategy {
public:
    void TurnOn(const string& name) override {
        cout << name << " ������� � ��������������� ��������." << endl;
    }
};

class KitchenAppliance
{
protected:
    string name;
    int power;
    string manufacturer;
    string material;
    TurnOnStrategy* strategy;
public:
    KitchenAppliance(string n, int p, string m, string mat) :
        name(n), power(p), manufacturer(m), material(mat), strategy(nullptr) {
    }

    void setStrategy(TurnOnStrategy* s) {
        strategy = s;
    }

    virtual void turn_on() = 0;

    virtual void turn_off() {
        cout << name << " ��������." << endl;
    }

    virtual void get_info() {
        cout << "��������: " << name << "\n��������: " << power << " ��\n�������������: " << manufacturer << "\n�������� �������: " << material << endl;
    }

    virtual void use() { // ��������� �����
        turn_on();
        get_info();
        turn_off();
    }

    virtual ~KitchenAppliance() {
        if (strategy) delete strategy;
    }
};


class Blender : public KitchenAppliance
{
private:
    int speedLevels;
    double cupVolume;
public:
    Blender(string man, int p, string mat, int s, double v)
        : KitchenAppliance("�������", p, man, mat), speedLevels(s), cupVolume(v) {
    }

    void turn_on() override {
        if (strategy)
            strategy->TurnOn(name);
        else
            cout << name << " �������. �������� ����� �� " << speedLevels << " ���������." << endl;
    }
    void get_info() override {
        KitchenAppliance::get_info();
        cout << "���������� ���������: " << speedLevels << "\n����� ����: " << cupVolume << " �\n" << endl;
    }
};

class Microwave : public KitchenAppliance
{
private:
    double volume;
    bool hasGrill;
public:
    Microwave(string man, int p, string mat, double v, bool g)
        : KitchenAppliance("�������������", p, man, mat), volume(v), hasGrill(g) {
    }

    void turn_on() override {
        if (strategy)
            strategy->TurnOn(name);
        else
            cout << name << " ��������. ������ " << (hasGrill ? "� ������" : "��� �����") << "." << endl;
    }

    void get_info() override {
        KitchenAppliance::get_info();
        cout << "����� ������: " << volume << " �\n�����: " << (hasGrill ? "��" : "���") << "\n" << endl;
    }
};

class CoffeeMachine : public KitchenAppliance
{
private:
    string coffeeType;
    int pressure;
public:
    CoffeeMachine(string man, int p, string mat, string type, int press)
        : KitchenAppliance("����������", p, man, mat), coffeeType(type), pressure(press) {
    }

    void turn_on() override {
        if (strategy)
            strategy->TurnOn(name);
        else
            cout << name << " ��������. ����������� " << coffeeType << " ���� ��� ��������� " << pressure << " ���." << endl;
    }

    void get_info() override {
        KitchenAppliance::get_info();
        cout << "��� ����: " << coffeeType << "\n�������� �����: " << pressure << " ���\n" << endl;
    }
};


// ��������� ����� ��� �������� ���������� ������� �������� �������
typedef KitchenAppliance* KitchenAppliancePtr;

KitchenAppliancePtr  createRandomAppliance() {
    int choice = rand() % 3;
    switch (choice) {
    case 0: return new Blender("Philips", 600, "�������", 5, 1.5);
    case 1: return new Microwave("Samsung", 1000, "������", 20, true);
    case 2: return new CoffeeMachine("DeLonghi", 1200, "�����", "��������", 15);
    default: return nullptr;
    }
}


//��������
template <typename T>
class Iterator {
public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T GetCurrent() const = 0;
    virtual ~Iterator() {}
};

//�������� ��� ���������� �� ������ �������
class VectorIterator : public Iterator<KitchenAppliancePtr>
{
private:
    const vector<KitchenAppliancePtr>* appliances;
    vector<KitchenAppliancePtr>::const_iterator it;
public:
    VectorIterator(const vector<KitchenAppliancePtr>* a) :
        appliances(a), it(a->begin()) {}
    void First() override { it = appliances->begin(); }
    void Next() override { it++; }
    bool IsDone() const override { return it == appliances->end(); }
    KitchenAppliancePtr GetCurrent() const override { return *it; }
};

// �������� ��� ���������� �� ������ ������������� �������
class ArrayIterator : public Iterator<KitchenAppliancePtr>
{
private:
    KitchenAppliancePtr* appliances;
    int size;
    int index;
public:
    ArrayIterator(KitchenAppliancePtr* a, int s) : appliances(a), size(s), index(0) {}
    void First() override { index = 0; }
    void Next() override { index++; }
    bool IsDone() const override { return index >= size; }
    KitchenAppliancePtr GetCurrent() const override
    {
        if (index < size)
            return appliances[index];
        else //�� ������� ���������a
            return nullptr;
    }
};

class ApplianceContainer {
public:
    virtual ~ApplianceContainer() = default;
    virtual void Add(KitchenAppliancePtr) = 0;
    virtual int GetCount() = 0;
    virtual KitchenAppliancePtr GetByIndex(int index) const = 0;
    virtual Iterator<KitchenAppliancePtr>* GetIterator() = 0;
};

//��������� �� ������ �������
class VectorContainer : public ApplianceContainer
{
private:
    vector<KitchenAppliancePtr> appliances;
public:
    ~VectorContainer() {
        for (auto appliance : appliances)
            delete appliance;
    }
    void Add(KitchenAppliancePtr appliance) override { appliances.push_back(appliance); }
    int GetCount() override { return appliances.size(); }
    Iterator<KitchenAppliancePtr>* GetIterator() override { return new VectorIterator(&appliances); }
    KitchenAppliancePtr GetByIndex(int index) const override {
        if (index >= 0 && index < appliances.size())
            return appliances[index];
        return nullptr;
    }
};

// ��������� �� ������ ������������� �������
class ArrayContainer : public ApplianceContainer {
private:
    KitchenAppliancePtr* appliances;
    int size;
    int capacity;
public:
    ArrayContainer() : size(0), capacity(4) {
        appliances = new KitchenAppliancePtr[capacity];
    }

    ~ArrayContainer() {
        for (int i = 0; i < size; i++)
            delete appliances[i];
        delete[] appliances;
    }

    void Add(KitchenAppliancePtr appliance) override {
        if (size >= capacity) {
            capacity *= 2;
            KitchenAppliancePtr* newAppliances = new KitchenAppliancePtr[capacity];
            for (int i = 0; i < size; i++)
                newAppliances[i] = appliances[i];
            delete[] appliances;
            appliances = newAppliances;
        }
        if (size < capacity) {
            appliances[size++] = appliance;
        }
    }

    int GetCount() override { return size; }

    Iterator<KitchenAppliancePtr>* GetIterator() override {
        return new ArrayIterator(appliances, size);
    }

    KitchenAppliancePtr GetByIndex(int index) const override {
        if (index >= 0 && index < size)
            return appliances[index];
        return nullptr;
    }
};

// ������� ��������� ���������
template <typename T>
class IteratorDecorator : public Iterator<T> {
protected:
    Iterator<T>* wrapped;
public:
    IteratorDecorator(Iterator<T>* it) : wrapped(it) {}
    void First() override { wrapped->First(); }
    void Next() override { wrapped->Next(); }
    bool IsDone() const override { return wrapped->IsDone(); }
    T GetCurrent() const override { return wrapped->GetCurrent(); }
    virtual ~IteratorDecorator() { delete wrapped; }
};

// ��������� ��� �������� ���������
template <typename T>
class CountingIterator : public IteratorDecorator<T> {
private:
    int counter;
public:
    CountingIterator(Iterator<T>* it) : IteratorDecorator<T>(it) {}
    void Next() override {
        IteratorDecorator<T>::Next();
        counter++;
    }
    int GetCount() const { return counter; }
};

// ���������, ������������ ������ ������ �������
template <typename T>
class SkipIterator : public IteratorDecorator<T> {
public:
    SkipIterator(Iterator<T>* it) : IteratorDecorator<T>(it) {}
    void Next() override {
        IteratorDecorator<T>::Next();
        if (!this->IsDone()) IteratorDecorator<T>::Next();
    }
};


// ���������, ����������� �� ���� ������� (��������, ������ ��������)
template <typename T>
class FilterBlenderIterator : public IteratorDecorator<T> {
public:
    FilterBlenderIterator(Iterator<T>* it) : IteratorDecorator<T>(it) {}

    void First() override {
        IteratorDecorator<T>::First();
        SkipNonBlender();
    }

    void Next() override {
        IteratorDecorator<T>::Next();
        SkipNonBlender();
    }

private:
    void SkipNonBlender() {
        while (!this->IsDone()) {
            T current = this->GetCurrent();
            if (dynamic_cast<Blender*>(current))
                break;
            IteratorDecorator<T>::Next();
        }
    }
};

// ������� ��� ����������� STL
template<typename ContainerType, typename ItemType>
class ConstIteratorAdapter : public Iterator<ItemType>
{
protected:
    ContainerType* Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType* container)
        : Container(container)
    {
        It = Container->begin();
    }

    virtual void First() override { It = Container->begin(); }
    virtual void Next() override { ++It; }
    virtual bool IsDone() const override { return It == Container->end(); }
    virtual ItemType GetCurrent() const override { return *It; }
};

//������� ������ ����������
void processContainer(Iterator<KitchenAppliancePtr>* iterator) {
    for (iterator->First(); !iterator->IsDone(); iterator->Next()) {
        KitchenAppliancePtr appliance = iterator->GetCurrent();
        if (appliance) {
            cout << "=====================================\n\n";
            appliance->get_info();
            cout << "-----------------------------------\n";
            appliance->turn_on();
            appliance->turn_off();
            cout << "=====================================\n\n";
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(0)));

    vector<KitchenAppliance*> appliances;

    // ������� ��������� ��������
    appliances.push_back(new Blender("Philips", 600, "�������", 5, 1.5));
    appliances.push_back(new Microwave("Samsung", 1000, "������", 20, true));
    appliances.push_back(new CoffeeMachine("DeLonghi", 1200, "�����", "��������", 15));
    appliances.push_back(new Blender("Bosch", 700, "�����", 3, 2.0));
    appliances.push_back(new Microwave("LG", 900, "�������", 25, false));

    // ������������ ������ � ������� �����������
    for (auto appliance : appliances) {
        int strategy_choice = rand() % 3;
        switch (strategy_choice) {
        case 0:
            appliance->setStrategy(new NormalStart());
            break;
        case 1:
            appliance->setStrategy(new EcoStart());
            break;
        case 2:
            appliance->setStrategy(new PreheatStart());
            break;
        }

        appliance->use(); // ��������� �����: turn_on + get_info + turn_off
        cout << "---------------------------------" << endl;
    }

    // ������������ ������
    for (auto appliance : appliances) {
        delete appliance;
    }

    return 0;
}
