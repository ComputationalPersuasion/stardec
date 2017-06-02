#ifndef STARDEC_VALUES_H
#define STARDEC_VALUES_H

#include <vector>
#include <array>
#include <string>
#include <sstream>
#include "beta_mixture.hpp"

namespace stardec {
    inline int comp3(double val1, double val2) {
        if(val1 > val2)
            return -1;
        else if(val1 == val2)
            return 0;
        else
            return 1;
    }

    /*************** base_value *******************/
    template <typename T>
    class base_value {
    public:
        base_value(T val) : _value(val) {}
        base_value(const base_value<T> &other) : _value(other._value) {}
        ~base_value() {}

        virtual T &value() {return _value;}
        virtual T const &value() const {return _value;}

        virtual int comp(const T &other) const = 0;
        virtual int comp(const base_value<T> &other) const {return comp(other._value);};

        virtual bool operator< (const base_value<T> &other) const {return comp(other) == -1;}
        virtual bool operator> (const base_value<T> &other) const {return comp(other) == 1;}
        virtual bool operator== (const base_value<T> &other) const {return comp(other) == 0;}
        virtual bool operator<= (const base_value<T> &other) const {return this->operator<(other) || this->operator==(other);};
        virtual bool operator>= (const base_value<T> &other) const {return this->operator>(other) || this->operator==(other);};

        virtual std::string str() const = 0;

    protected:
        T _value;
    };

    /*************** belief *******************/
    class belief : public base_value<double> {
    public:
        belief() : base_value<double>(0.5) {}
        belief(double val) : base_value<double>(val) {}

        virtual int comp(const double &other) const override {return comp3(_value, other);}
        virtual int comp(const belief &other) const {return comp(other._value);}

        virtual std::string str() const override {return std::to_string(_value);}
    };

    /*************** affective_norm *******************/
    class affective_norm : public base_value<std::array<double, 3>> {
    public:
        affective_norm() : base_value<std::array<double,3>>({-1.0,-1.0,-1.0}) {}
        affective_norm(const std::array<double, 3> &norms) : base_value<std::array<double,3>>(norms) {}

        virtual int comp(const std::array<double, 3> &other) const override {
            int c0 = comp3(_value[0], other[0]);
            int c1 = comp3(_value[1], other[1]);

            if(c0 == -c1) return -2;
            int ret = c0 + c1;
            if(ret < -1) ret = -1;
            else if(ret > 1) ret = 1;

            int c2 = comp3(_value[2], other[2]);
            if(ret == -c2) return -2;
            ret += c2;
            if(ret < -1) ret = -1;
            else if(ret > 1) ret = 1;

            return ret;
        }
        virtual int comp(const affective_norm &other) const {return comp(other._value);}

        virtual std::string str() const override {
            std::stringstream s;
            s << "[" << _value[0] << "," << _value[1] << "," << _value[2] << "]";
            return s.str();
        }
    };

    /******************* imprecise_belief ******************/
    class imprecise_belief : public base_value<std::array<double, 2>> {
    public:
        imprecise_belief() : base_value<std::array<double,2>>({0,1}) {}
        imprecise_belief(const std::array<double,2> &bounds) : base_value<std::array<double,2>>(bounds) {}

        virtual int comp(const std::array<double, 2> &other) const override {
            int c0 = comp3(_value[0], other[0]);
            int c1 = comp3(_value[1], other[1]);

            if(c0 == -c1) return -2;
            int ret = c0 + c1;
            if(ret < -1) ret = -1;
            else if(ret > 1) ret = 1;
            return ret;
        }
        virtual int comp(const imprecise_belief &other) const {return comp(other._value);}

        virtual std::string str() const override {
            std::stringstream s;
            s << "[" << _value[0] << "," << _value[1] << "]";
            return s.str();
        }
    };

    /******************* beta_belief ******************/
    class beta_belief : public base_value<beta_mixture> {
    public:
        beta_belief() : base_value<beta_mixture>(beta_mixture()) {}
        beta_belief(const beta_mixture &mixture) : base_value<beta_mixture>(mixture) {}

        virtual int comp(const beta_mixture &) const override {
            return 0;
        }
        virtual int comp(const beta_belief &other) const {return comp(other._value);}

        virtual std::string str() const override {
            std::stringstream s;
            for(const auto &p : _value.mixture())
                s << "[" << p.first << ": (" << p.second.alpha() << "," << p.second.beta() << ")] ";
            return s.str();
        }
    };
}

#endif //STARDEC_VALUES_H
