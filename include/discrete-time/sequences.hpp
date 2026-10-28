#pragma once
#include <cstddef>
#include <memory>
#include <cmath>
#include <cassert>
#include <span>
#include <iostream>
#include <iomanip>
#include <algorithm>
namespace dsp::discrete_time{


class DiscreteTimeSequence {
public:
    explicit DiscreteTimeSequence(std::size_t n, std::ptrdiff_t start = 0)
        : start_(start), n_(n), sequence_(std::make_unique<float[]>(n)) {}

    std::ptrdiff_t end() const { return start_ + static_cast<ptrdiff_t>(n_); }
    std::ptrdiff_t start() const { return start_;}
    std::size_t size() const { return n_; }
    void set_start(std::ptrdiff_t s) { start_ = s; }
    // void resize()

    std::span<float> samples() {return {sequence_.get(), n_}; }
    std::span<const float> samples() const { return {sequence_.get(), n_}; }

    // Access by t
    float operator()(std::ptrdiff_t t) const{
        std::ptrdiff_t i = t - start_;
        if (i < 0 || i >= static_cast<std::ptrdiff_t>(n_)) return 0.0;
        return sequence_[i];
    }
    // Array access
    float& operator[](std::size_t i) { 
        assert(i < n_);
        return sequence_[i];
    }

    const float& operator[](std::size_t i) const { 
        assert(i < n_);
        return sequence_[i]; 
    }
    

    //Deep copy
    DiscreteTimeSequence(const DiscreteTimeSequence& rhs) 
        : start_(rhs.start_), n_(rhs.n_), sequence_(std::make_unique<float[]>(rhs.n_)){
    	std::copy(rhs.sequence_.get(), rhs.sequence_.get()+n_, this->sequence_.get());
    }

    DiscreteTimeSequence& operator=(const DiscreteTimeSequence& rhs) {
    	DiscreteTimeSequence tmp(rhs);
        std::swap(this->start_, tmp.start_);
    	std::swap(this->n_,tmp.n_);
    	std::swap(this->sequence_,tmp.sequence_);
    	return *this;
    }

    DiscreteTimeSequence& operator*=(const float a) {
        for (std::size_t i=0; i < n_; ++i) {
            sequence_[i] *= a;
        }
        return *this;
    }

    // std::move
    DiscreteTimeSequence(DiscreteTimeSequence&&) noexcept = default;
    DiscreteTimeSequence& operator=(DiscreteTimeSequence&&) noexcept = default;

    friend std::ostream& operator<<(std::ostream& os, const DiscreteTimeSequence& x)  { 
        os << "   i |";
        for (std::size_t i = 0; i < x.n_; ++i) os << std::setw(6) << i;
        os << "\n   t |";
        for (std::size_t i = 0; i < x.n_; ++i) os << std::setw(6) << x.start_ + static_cast<std::ptrdiff_t>(i);
        os << "\nx(t) |";
        for (std::size_t i =0; i<x.n_; ++i) os << std::setw(6) << x.sequence_[i];
        return os << '\n';
    }

    void print(std::ostream& os = std::cout) const { os << *this;}

private:
    std::ptrdiff_t start_;
    std::size_t n_;
    std::unique_ptr<float[]> sequence_;
};


inline DiscreteTimeSequence operator+(const DiscreteTimeSequence& x1,
                                      const DiscreteTimeSequence& x2) {
    std::ptrdiff_t first = std::min(x1.start(), x2.start());
    std::ptrdiff_t last = std::max(x1.end(),x2.end());
    DiscreteTimeSequence out(static_cast<size_t>(last - first), first);
    for (std::size_t i = 0; i < out.size(); ++i) {
        std::ptrdiff_t t = first + static_cast<std::ptrdiff_t>(i);
        out[i] = x1(t) + x2(t);
    }
    return out;
}

inline DiscreteTimeSequence operator*(float a, DiscreteTimeSequence x) {
    x*=a; return x;
}

inline DiscreteTimeSequence operator*(DiscreteTimeSequence x, float a) {
    x*=a; return x;
}
inline DiscreteTimeSequence delayed_copy(DiscreteTimeSequence x, std::ptrdiff_t d) {
	x.set_start(x.start() + d);
	return x;
}

inline DiscreteTimeSequence uniform_sequence(float val, std::size_t n, std::ptrdiff_t start =0) {
    DiscreteTimeSequence out(n,start);
    for (std::size_t i =0; i < n; ++i) {
        out[i] = val;
    }
    return out;
}

inline DiscreteTimeSequence unit_sample_sequence(std::size_t n, std::ptrdiff_t start=0) {
    DiscreteTimeSequence out(n,start);
    if (start <= 0) {
        out[-start] = 1.0f;
    }
	return out;
}

inline DiscreteTimeSequence unit_step_sequence(std::size_t n, std::ptrdiff_t start=0) {
    DiscreteTimeSequence out(n,start);
    if (start < 0) {
        out = out + uniform_sequence(1,n-start,0);
    } 
    else{ // all ones
        for (std::size_t i = 0; i < n; ++i) {
            out = uniform_sequence(1,n,start);
        }
    }
    return out;
}

inline DiscreteTimeSequence exponential_sequence(float A, float alpha, std::size_t n, std::ptrdiff_t start = 0) {
    DiscreteTimeSequence out(n,start);
    for (std::size_t i = 0; i < n; ++i) {
        std::ptrdiff_t t = start + static_cast<std::ptrdiff_t>(i);
        out[i] = A * std::pow(alpha,t);
    }
    return out;
}

inline DiscreteTimeSequence sinusoidal_sequence(double A, double omega, double phi, std::size_t n, std::ptrdiff_t start = 0) {
    DiscreteTimeSequence out(n,start);
    for (std::size_t i = 0; i < n; ++i) {
        std::ptrdiff_t t = start+ static_cast<std::ptrdiff_t>(i);
        out[i] = static_cast<float>(A * std::cos(omega*t + phi));
    }
    return out;
}

}