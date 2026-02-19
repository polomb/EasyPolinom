#ifndef __Polinoms__
#define __Polinoms__

#include "List.h"

#include <cstdint>
#include <string>

#define ZERO 1e-12

class Polinom
{
    struct Monom //a * x^i * y^j * z^k
    {
        double coeff;
        uint32_t deg_of_x;
        uint32_t deg_of_y;
        uint32_t deg_of_z;
        //
        Monom(double coef = 0.0, uint32_t degree_of_x = 0, uint32_t degree_of_y = 0,\
            uint32_t degree_of_z = 0) : coeff(coef), deg_of_x(degree_of_x),\
            deg_of_y(degree_of_y), deg_of_z(degree_of_z)
        {
            if (std::abs(coef) < ZERO && (degree_of_x != 0 || degree_of_y != 0 || degree_of_z != 0))
            {
                coeff = 0.0;
            }
            if (degree_of_x > 1024 || degree_of_y > 1024 || degree_of_z > 1024)
            {
                throw std::out_of_range("Degree > 1024");
            }
        }
        bool is_similar(const Monom& other) const
        {
            return (deg_of_x == other.deg_of_x &&\
                deg_of_y == other.deg_of_y &&\
                deg_of_z == other.deg_of_z);
        }
        bool operator==(const Monom& other) const
        {
            return (is_similar(other) && std::abs(coeff - other.coeff) < ZERO);
        }
        bool operator<(const Monom& other) const // 1 -> x -> y -> z
        {
            if (deg_of_x != other.deg_of_x)
                return deg_of_x < other.deg_of_x;
            if (deg_of_y != other.deg_of_y)
                return deg_of_y < other.deg_of_y;
            return deg_of_z < other.deg_of_z;
        }
    };
    ForwardList<Monom> monoms;

    ForwardList<Monom>::Iterator insert_sorted(const Monom& monom, ForwardList<Monom>::Iterator iter = nullptr)
    {
        if (std::abs(monom.coeff) < ZERO) return iter;
        if (monoms.empty())
        {
            monoms.push_front(monom);
            return monoms.begin();
        }

        auto it = monoms.begin();
        if (iter != nullptr)
        {
            it = iter;
        }
        auto prev = monoms.end();

        if (iter != nullptr)
        {
            if (it != monoms.end() && it.iter->data < monom)
            {
                prev = it;
                ++it;
            }
        }
        else
        {
            while (it != monoms.end() && it.iter->data < monom)
            {
                prev = it;
                ++it;
            }
        }

        if (it != monoms.end() && it.iter->data.is_similar(monom))
        {
            Monom& existing = it.iter->data;
            existing.coeff += monom.coeff;

            if (std::abs(existing.coeff) < ZERO)
            {
                monoms.erase_after(prev);
            }
            return it;
        }
        else
        {
            monoms.insert_after(prev, monom);
        }
    }
public:
    //Ќужны +, -, * с полиномами, * и деление на константу
    Polinom() = default;
    Polinom(const Polinom& other) : monoms(other.monoms) {}
    Polinom(Polinom&& other) noexcept : monoms(std::move(other.monoms)) {}
    Polinom& operator=(const Polinom& other)
    {
        if (this != &other)
        {
            monoms = other.monoms;
        }
        return *this;
    }
    Polinom& operator=(Polinom&& other) noexcept
    {
        if (this != &other)
        {
            monoms = std::move(other.monoms);
        }
        return *this;
    }
    //
    Polinom operator+(const Polinom& other) const
    {
        Polinom result;
        auto it1 = monoms.begin();
        auto it2 = other.monoms.begin();
        auto it3 = result.monoms.begin();

        while (it1 != monoms.end() && it2 != other.monoms.end())
        {
            if (it1.iter->data < it2.iter->data)
            {
                it3 = result.insert_sorted(it1.iter->data, it3);
                ++it1;
                if (it3 != nullptr)
                    ++it3;
            }
            else if (it2.iter->data < it1.iter->data)
            {
                it3 = result.insert_sorted(it2.iter->data, it3);
                ++it2;
                if (it3 != nullptr)
                    ++it3;
            }
            else
            {
                Monom sum = it1.iter->data;
                sum.coeff += it2.iter->data.coeff;
                it3 = result.insert_sorted(sum, it3);
                ++it1;
                ++it2;
                if (it3 != nullptr)
                    ++it3;
            }
        }
        while (it1 != monoms.end())
        {
            it3 = result.insert_sorted(it1.iter->data, it3);
            ++it1;
            if (it3 != nullptr)
                ++it3;
        }
        while (it2 != other.monoms.end())
        {
            it3 = result.insert_sorted(it2.iter->data, it3);
            ++it2;
            if (it3 != nullptr)
                ++it3;
        }
        return result;
    }
    Polinom operator-(const Polinom& other) const
    {
        Polinom result;
        auto it1 = monoms.begin();
        auto it2 = other.monoms.begin();
        auto it3 = result.monoms.begin();

        while (it1 != monoms.end() && it2 != other.monoms.end())
        {
            if (it1.iter->data < it2.iter->data)
            {
                it3 = result.insert_sorted(it1.iter->data, it3);
                ++it1;
                if(it3 != nullptr)
                    ++it3;
            }
            else if (it2.iter->data < it1.iter->data)
            {
                it3 = result.insert_sorted(it2.iter->data, it3);
                ++it2;
                if (it3 != nullptr)
                    ++it3;
            }
            else
            {
                Monom sum = it1.iter->data;
                sum.coeff -= it2.iter->data.coeff;
                it3 = result.insert_sorted(sum, it3);
                ++it1;
                ++it2;
                if (it3 != nullptr)
                    ++it3;
            }
        }
        while (it1 != monoms.end())
        {
            it3 = result.insert_sorted(it1.iter->data, it3);
            ++it1;
            if (it3 != nullptr)
                ++it3;
        }
        while (it2 != other.monoms.end())
        {
            it3 = result.insert_sorted(it2.iter->data, it3);
            ++it2;
            if (it3 != nullptr)
                ++it3;
        }
        return result;
    }
    Polinom operator*(const Polinom& other) const
    {
        Polinom result;
        auto it1 = monoms.begin();
        auto it2 = other.monoms.begin();
        auto it3 = result.monoms.begin();

        while (it1 != monoms.end())
        {
            while (it2 != other.monoms.end())
            {
                Monom mul = it1.iter->data;
                mul.coeff *= it2.iter->data.coeff;
                mul.deg_of_x += it2.iter->data.deg_of_x;
                mul.deg_of_y += it2.iter->data.deg_of_y;
                mul.deg_of_z += it2.iter->data.deg_of_z;
                it3 = result.insert_sorted(mul, it3);
                ++it2;
                if (it3 != nullptr)
                    ++it3;
            }
            it2 = other.monoms.begin();
            ++it1;
        }
        return result;
    }
    Polinom operator*(const double con) const
    {
        if (std::abs(con) < ZERO)
        {
            return Polinom();
        }

        Polinom result;
        auto it1 = monoms.begin();
        auto it2 = result.monoms.begin();

        while (it1 != monoms.end())
        {
            Monom sum = it1.iter->data;
            sum.coeff *= con;
            it2 = result.insert_sorted(sum, it2);
            ++it1;
            if (it2 != nullptr)
                ++it2;
        }
        return result;
    }
    Polinom operator/(const double con) const
    {
        if (std::abs(con) < ZERO)
        {
            throw std::runtime_error("Division by zero");
        }

        Polinom result;
        auto it1 = monoms.begin();
        auto it2 = result.monoms.begin();

        while (it1 != monoms.end())
        {
            Monom sum = it1.iter->data;
            sum.coeff /= con;
            it2 = result.insert_sorted(sum, it2);
            ++it1;
            if (it2 != nullptr)
                ++it2;
        }
        return result;
    }
    Polinom(const std::string& input)
    {
        this->parse(input);
    }
    Polinom& operator=(const std::string& input) {
        monoms.clear();
        this->parse(input);
        return *this;
    }
private:
    void parse(const std::string& input)
    {
        std::string polin = input;
        polin.erase(std::remove_if(polin.begin(), polin.end(), ::isspace), polin.end());
        if (polin.empty()) return;

        size_t pos = 0;
        char sign = '+';

        while (pos < polin.length())
        {
            if (polin[pos] == '+' || polin[pos] == '-')
            {
                sign = polin[pos];
                pos++;
            }
            if (pos >= polin.length()) break;

            double coeff = 1.0;
            uint32_t deg_x = 0, deg_y = 0, deg_z = 0;

            size_t start = pos;
            while (pos < polin.length() && (isdigit(polin[pos]) || polin[pos] == '.'))
            {
                pos++;
            }

            if (start != pos)
            {
                coeff = std::stod(polin.substr(start, pos - start));
            }

            if (sign == '-') coeff = -coeff;

            while (pos < polin.length() && (polin[pos] == 'x' || polin[pos] == 'y' || polin[pos] == 'z'))
            {
                char var = polin[pos];
                pos++;

                uint32_t degree = 1;

                if (pos < polin.length() && polin[pos] == '^')
                {
                    pos++;
                    start = pos;
                    while (pos < polin.length() && isdigit(polin[pos]))
                    {
                        pos++;
                    }
                    if (start != pos)
                    {
                        degree = std::stoul(polin.substr(start, pos - start));
                    }
                }

                switch (var)
                {
                case 'x': deg_x = degree; break;
                case 'y': deg_y = degree; break;
                case 'z': deg_z = degree; break;
                }
            }
            insert_sorted(Monom(coeff, deg_x, deg_y, deg_z));
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const Polinom& poly)
    {
        if (poly.monoms.empty())
        {
            os << "0";
            return os;
        }

        auto it = poly.monoms.begin();
        bool first = true;

        while (it != poly.monoms.end())
        {
            const Monom& m = it.iter->data;

            if (first)
            {
                if (m.coeff < 0)
                    os << "-";
                first = false;
            }
            else
            {
                if (m.coeff > 0)
                    os << " + ";
                else
                    os << " - ";
            }
            double abs_coeff = std::abs(m.coeff);
            bool has_vars = (m.deg_of_x > 0 || m.deg_of_y > 0 || m.deg_of_z > 0);

            if (!has_vars || std::abs(abs_coeff - 1.0) > ZERO)
            {
                std::string coeff_str = std::to_string(abs_coeff);
                os << coeff_str;
            }

            if (m.deg_of_x > 0)
            {
                os << "x";
                if (m.deg_of_x > 1)
                    os << "^" << m.deg_of_x;
            }
            if (m.deg_of_y > 0)
            {
                os << "y";
                if (m.deg_of_y > 1)
                    os << "^" << m.deg_of_y;
            }
            if (m.deg_of_z > 0)
            {
                os << "z";
                if (m.deg_of_z > 1)
                    os << "^" << m.deg_of_z;
            }

            ++it;
        }

        return os;
    }
};

#endif

