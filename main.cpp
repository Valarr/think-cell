#include <map>
#include <iostream>


template<typename K, typename V>
class interval_map {
    friend void IntervalMapTest();

    V m_valBegin;
    std::map<K, V> m_map;

public:
// constructor associates whole range of K with val
    explicit interval_map(V const &val)
            : m_valBegin(val) {}

    V const &operator[](K const &key) const {
        auto it = m_map.upper_bound(key);
        if (it == m_map.begin()) {
            return m_valBegin;
        } else {
            return (--it)->second;
        }
    }

    void PrintMap() {
        for (auto const &pair: m_map) {
            std::cout << "{" << pair.first << ": " << pair.second << "}\n";
        }
    }

    void PrintValBegin() {
        std::cout << m_valBegin;
    }
    //endregion

    void assign(K const &keyBegin, K const &keyEnd, V const &val) {

        // if invalid range do nothing
        if (keyBegin >= keyEnd) {
            return;
        }

        // store previous value at keyBegin
        V aux = operator[](keyBegin);

        // insert value at keyBegin, if not already the same
        if (aux != val) {
            m_map[keyBegin] = val;
        }

        auto it = m_map.upper_bound(keyBegin);
        auto next_it = it;
        while (it != m_map.cend()) {
            if (keyEnd < it->first) break;
            ++next_it;
            m_map.erase(it);
            aux = it->second;
          it = next_it;
        }

        // put previous value at keyEnd
        if ((val != aux)) {
            m_map[keyEnd] = aux;
        }
    }
};

//region Help Functions
void PrintRange(int start, int end, interval_map<int, char> m) {
    for (int i = start; i <= end; i++) {
        std::cout << "Key: " << i << " Value: " << m[i] << std::endl;
    }
}

void Print(interval_map<int, char> m) {
    std::cout << std::endl << "Range" << std::endl;
    PrintRange(-5, 30, m);
    std::cout << std::endl << "Map" << std::endl;
    std::cout << "{ValBegin: ";
    m.PrintValBegin();
    std::cout << "}" << std::endl;
    m.PrintMap();
}
//endregion



void IntervalMapTest() {
    //Initialise m_map
    //interval_map m = interval_map<int, char>('A');
interval_map<int, char> m('A');

int passed = 0;
int failed = 0;

#pragma region EmptyRange
passed = 0;
failed = 3;

m.assign(3, 3, 'B');
if (m.m_map.count(3) == 0) {
  passed++;
  failed--;
}

m.assign(3, 2, 'B');
if (m.m_map.count(2) == 0) {
  passed++;
  failed--;
}
if (m.m_map.count(3) == 0) {
  passed++;
  failed--;
}

std::cout << "EmptyRange | " << passed << " | " << failed << "\n";
#pragma endregion

#pragma region TrivialRange
passed = 0;
failed = 11;

m.assign(1, 10, 'B');
if (m[0] == 'A') {
  passed++;
  failed--;
}

for (int i = 1; i < 10; i++)
{
  if (m[i] == 'B') {
    passed++;
    failed--;
  }
}

if (m[10] == 'A') {
  passed++;
  failed--;
}

std::cout << "TrivialRange | " << passed << " | " << failed << "\n";
#pragma endregion

#pragma region TrivialTwoRange
passed = 0;
failed = 9;
for(int i=0;i<10;i++){
   std::cout << i << "\t" << m[i] << "\n";
}
m.assign(1, 3, 'B');
m.assign(6, 8, 'C');

if (m[0] == 'A'){
  std::cout << "0\n";
  passed++;
  failed--;
}
if (m[1] == 'B'){
  std::cout << "1\n";
  passed++;
  failed--;
}
if (m[2] == 'B'){
  std::cout << "2\n";
  passed++;
  failed--;
}
if (m[3] == 'A'){
   std::cout << "3\n";
  passed++;
  failed--;
}
if (m[4] == 'A'){
  std::cout << "4\n";
  passed++;
  failed--;
}
if (m[5] == 'A'){
  std::cout << "5\n";
  passed++;
  failed--;
}
if (m[6] == 'C'){
  std::cout << "6\n";
  passed++;
  failed--;
}
if (m[7] == 'C'){
  std::cout << "7\n";
  passed++;
  failed--;
}
if (m[8] == 'A'){
  std::cout << "8\n";
  passed++;
  failed--;
}
for(int i=0;i<10;i++){
   std::cout << i << "\t" << m[i] << "\n";
}
std::cout << "TrivialTwoRange | " << passed << " | " << failed << "\n";
#pragma endregion

#pragma region OverwriteLowest
passed = 0;
failed = 3;

m.assign(std::numeric_limits<int>::lowest(), 10000, 'B');

if (m[0] == 'B') {
  passed++;
  failed--;
}
if (m[9999] == 'B') {
  passed++;
  failed--;
}
if (m[10000] == 'A') {
  passed++;
  failed--;
}

std::cout << "OverwriteLowest | " << passed << " | " << failed << "\n";
#pragma endregion

#pragma region Merge
passed = 0;
failed = 4;

m.assign(std::numeric_limits<int>::lowest(), 10, 'B');
m.assign(10, 20, 'B');

if (m[0] == 'B') {
  std::cout << "0\n";
  passed++;
  failed--;
}
if (m[10] == 'B') {
  std::cout << "10\n";
  passed++;
  failed--;
}
if (m[19] == 'B') {
  std::cout << "19\n";
  passed++;
  failed--;
}
if (m[20] == 'A') {
  std::cout << "20\n";
  passed++;
  failed--;
}

std::cout << "Merge | " << passed << " | " << failed << "\n";
#pragma endregion


}


int main() {

  IntervalMapTest();
  /*
  interval_map<int, char> test = interval_map<int, char>('A');

 

  test.assign(1, 3, 'B');
  for (int i = -1; i < 7; i++) {
    std::cout << "Valor " << i << ": " << test.operator[](i) << "\n";
  }

  std::cout << "-------------------------\n";

  test.assign(4, 6, 'B');
  for (int i = -1; i < 7; i++) {
    std::cout << "Valor " << i << ": " << test.operator[](i) << "\n";
  }

   test.assign(3, 3, 'B');
   if(test.operator[](3) != 'B'){
	   std::cout << test.operator[](3) << " EmptyRange1\n";
   } else std::cout << test.operator[](3) << " NotEmptyRange1\n";
   if(test.operator[](3) != 'B'){
	   std::cout << test.operator[](3) << " EmptyRange2\n";
   } else std::cout << test.operator[](3) << " NotEmptyRange2\n";

  test.assign(1, 10, 'B');
   if(test.operator[](0) == 'A') std::cout << "TrivialRange1\n";
   else std::cout << "NotTrivialRange1\n";

   for (int i = 1; i < 11; i++){
   if(test.operator[](i) == 'B')	std::cout << i << test.operator[](i) << " TrivialRange2\n";
   else std::cout << i << test.operator[](i) << " NotTrivialRange2\n";
   }*/
  
}

//Type requirements are met: You must adhere to the specification of the key and value type given above. For example, many solutions we receive use operations other than those that are explicitly stated in the task description. We have to reject many solutions because they assume that V is default-constructible, e.g., by using std::map::operator[].
