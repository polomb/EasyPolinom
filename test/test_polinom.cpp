#include "Polinom.h"

#include <gtest.h>

TEST(Polinoms, DefaultConstructor)
{
    Polinom p;
    std::ostringstream oss;
    oss << p;

    EXPECT_EQ(oss.str(), "0");
}

TEST(Polinoms, StringConstructor)
{
    Polinom p("2x^2y + 3z");
    std::ostringstream oss;
    oss << p;

    EXPECT_NE(oss.str(), "0");
}

TEST(Polinoms, CopyConstructor)
{
    Polinom original("x + y");
    Polinom copy(original);

    std::ostringstream oss1, oss2;
    oss1 << original;
    oss2 << copy;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, MoveConstructor)
{
    Polinom original("x + y");
    Polinom moved(std::move(original));

    std::ostringstream oss;
    oss << moved;
    EXPECT_FALSE(oss.str().empty());
}

TEST(Polinoms, AssignmentOperator)
{
    Polinom p1("x^2 + y^2");
    Polinom p2;
    p2 = p1;

    std::ostringstream oss1, oss2;
    oss1 << p1;
    oss2 << p2;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, MoveAssignmentOperator)
{
    Polinom p1("x^2 + y^2");
    Polinom p2;
    p2 = std::move(p1);

    std::ostringstream oss;
    oss << p2;
    EXPECT_FALSE(oss.str().empty());
}

TEST(Polinoms, StringAssignmentOperator)
{
    Polinom p;
    p = "3x^2 + 4y";

    std::ostringstream oss;
    oss << p;
    EXPECT_FALSE(oss.str().empty());
}

TEST(Polinoms, Addition)
{
    Polinom p1("x + y + z");
    Polinom p2("2x + 3y + 4z");
    Polinom result = p1 + p2;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("3") != std::string::npos);
    EXPECT_TRUE(oss.str().find("4") != std::string::npos);
    EXPECT_TRUE(oss.str().find("5") != std::string::npos);
}

TEST(Polinoms, AdditionWithZero)
{
    Polinom p1("x + y + z");
    Polinom p2("0");
    Polinom result = p1 + p2;
    std::ostringstream oss1, oss2;
    oss1 << p1;
    oss2 << result;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, AdditionLikeTerms)
{
    Polinom p("x + x");
    std::ostringstream oss;
    oss << p;
    EXPECT_TRUE(oss.str().find("2.000000x") != std::string::npos);
}

TEST(Polinoms, Subtraction)
{
    Polinom p1("x + y + z");
    Polinom p2("2x + 3y + 4z");
    Polinom result = p2 - p1;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("x") != std::string::npos);
    EXPECT_TRUE(oss.str().find("2.000000y") != std::string::npos);
    EXPECT_TRUE(oss.str().find("3.000000z") != std::string::npos);
}

TEST(Polinoms, SubtractionToZero)
{
    Polinom p1("x + y + z");
    Polinom result = p1 - p1;
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "0");
}

TEST(Polinoms, SubtractionWithZero)
{
    Polinom p1("x + y + z");
    Polinom p2("0");
    Polinom result = p1 - p2;
    std::ostringstream oss1, oss2;
    oss1 << p1;
    oss2 << result;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, Multiplication)
{
    Polinom p1("x + y + z");
    Polinom p2("x + 1");
    Polinom result = p1 * p2;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("x^2") != std::string::npos);
    EXPECT_TRUE(oss.str().find("xy") != std::string::npos);
    EXPECT_TRUE(oss.str().find("xz") != std::string::npos);
}

TEST(Polinoms, MultiplicationByZero)
{
    Polinom p1("x + y + z");
    Polinom p2("0");
    Polinom result = p1 * p2;
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "0");
}

TEST(Polinoms, MultiplicationSquare)
{
    Polinom p1("x^2 + 2xy + y^2");
    Polinom result = p1 * p1;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("x^4") != std::string::npos);
}

TEST(Polinoms, MultiplicationByConstant)
{
    Polinom p1("x + y + z");
    Polinom result = p1 * 2.0;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("2.000000x") != std::string::npos);
    EXPECT_TRUE(oss.str().find("2.000000y") != std::string::npos);
    EXPECT_TRUE(oss.str().find("2.000000z") != std::string::npos);
}

TEST(Polinoms, MultiplicationByZeroConstant)
{
    Polinom p1("x + y + z");
    Polinom result = p1 * 0.0;
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "0");
}

TEST(Polinoms, DivisionByConstant)
{
    Polinom p1("2x + 3y + 4z");
    Polinom result = p1 / 2.0;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("x") != std::string::npos);
    EXPECT_TRUE(oss.str().find("1.500000y") != std::string::npos ||
        oss.str().find("1.500000y") != std::string::npos);
}

TEST(Polinoms, DivisionByZero)
{
    Polinom p1("x + y + z");
    EXPECT_ANY_THROW(p1 / 0.0);
}

TEST(Polinoms, SimpleMonom)
{
    Polinom p("5x^2y^3z^4");
    std::ostringstream oss;
    oss << p;
    EXPECT_TRUE(oss.str().find("5") != std::string::npos);
    EXPECT_TRUE(oss.str().find("x^2") != std::string::npos);
    EXPECT_TRUE(oss.str().find("y^3") != std::string::npos);
    EXPECT_TRUE(oss.str().find("z^4") != std::string::npos);
}

TEST(Polinoms, MultipleTerms)
{
    Polinom p("5x^5y^3z^2 + 5x^2yz + 12");
    std::ostringstream oss;
    oss << p;
    EXPECT_TRUE(oss.str().find("5.000000x^5y^3z^2") != std::string::npos);
    EXPECT_TRUE(oss.str().find("5.000000x^2yz") != std::string::npos);
    EXPECT_TRUE(oss.str().find("12") != std::string::npos);
}

TEST(Polinoms, NegativeCoefficients)
{
    Polinom p("-2x - 3y + 4z");
    std::ostringstream oss;
    oss << p;
    EXPECT_TRUE(oss.str().find("- 2.000000x") != std::string::npos);
    EXPECT_TRUE(oss.str().find("- 3.000000y") != std::string::npos);
}

TEST(Polinoms, NoCoefficient)
{
    Polinom p("x + y - z");
    std::ostringstream oss;
    oss << p;
    std::cout << p;
    EXPECT_TRUE(oss.str().find(" x") != std::string::npos);
    EXPECT_TRUE(oss.str().find(" y") != std::string::npos);
    EXPECT_TRUE(oss.str().find("z") != std::string::npos);
}

TEST(Polinoms, DecimalCoefficients)
{
    Polinom p("1.5x");
    std::ostringstream oss;
    oss << p;
    EXPECT_TRUE(oss.str().find("1.500000x") != std::string::npos);
}

TEST(Polinoms, WhitespaceHandling)
{
    Polinom p1("x+y+z");
    Polinom p2("x + y + z");

    std::ostringstream oss1, oss2;
    oss1 << p1;
    oss2 << p2;

    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, ComplexExpression)
{
    Polinom p1("x + y + z");
    Polinom p2("2x + 3y + 4z");
    Polinom p3("x^2 + 2xy + y^2");
    Polinom p4("x + 1");
    Polinom result = (p1 + p2) * p3 - p4;

    std::ostringstream oss;
    oss << result;
    EXPECT_FALSE(oss.str().empty());
}

TEST(Polinoms, Associativity)
{
    Polinom p1("x + y + z");
    Polinom p2("2x + 3y + 4z");
    Polinom p3("x^2 + 2xy + y^2");
    Polinom r1 = (p1 + p2) + p3;
    Polinom r2 = p1 + (p2 + p3);

    std::ostringstream oss1, oss2;
    oss1 << r1;
    oss2 << r2;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, Commutativity)
{
    Polinom p1("x + y + z");
    Polinom p2("2x + 3y + 4z");
    Polinom r1 = p1 * p2;
    Polinom r2 = p2 * p1;

    std::ostringstream oss1, oss2;
    oss1 << r1;
    oss2 << r2;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, ZeroPolinomOperations)
{
    Polinom zero;
    Polinom p("x + y");

    Polinom sum = zero + p;
    Polinom diff = p - zero;
    Polinom prod = zero * p;

    std::ostringstream oss_sum, oss_diff, oss_prod, oss_p;
    oss_sum << sum;
    oss_diff << diff;
    oss_prod << prod;
    oss_p << p;

    EXPECT_EQ(oss_sum.str(), oss_p.str());
    EXPECT_EQ(oss_diff.str(), oss_p.str());
    EXPECT_EQ(oss_prod.str(), "0");
}

TEST(Polinoms, Cancellation)
{
    Polinom p("x + y - x");
    std::ostringstream oss;
    oss << p;
    EXPECT_TRUE(oss.str().find("y") != std::string::npos);
    EXPECT_FALSE(oss.str().find("x") != std::string::npos);
}

TEST(Polinoms, HardQuestion)
{
    Polinom p("x - x + y - y");
    std::ostringstream oss;
    oss << p;
    std::cout << p;
    EXPECT_TRUE(oss.str().find("0") != std::string::npos);
}