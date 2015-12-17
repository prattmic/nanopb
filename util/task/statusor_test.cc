#include <utility>
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "util/task/statusor.h"

using util::StatusOr;

class Movable {
 public:
    Movable(int n) : n_(n) {}

    ~Movable() {
        n_ = -2;
    }

    Movable(const Movable& m) = delete;

    // Move constructor.
    Movable(Movable&& other) : n_(other.n_) {
        other.n_ = -1;
    }

    int n_;
};

TEST(MovableTest, Move) {
    Movable m(1);

    Movable n = std::move(m);

    EXPECT_EQ(1, n.n_);
    EXPECT_EQ(-1, m.n_);
}

StatusOr<Movable> GetMovable(int n) {
    return Movable(1);
}

TEST(StatusOrTest, Move) {
    auto statusor = GetMovable(1);

    ASSERT_TRUE(statusor.ok());

    auto m = statusor.Move();
    EXPECT_EQ(1, m);
}
