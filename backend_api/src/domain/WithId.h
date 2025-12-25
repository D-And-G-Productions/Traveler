#include <cstdint>
template <typename T>
class WithId {
public:
  int64_t id;
  T value;
};
