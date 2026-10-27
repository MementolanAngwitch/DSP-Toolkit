#pragma once
#include <cstddef>
#include <memory>

struct DiscreteTimeSequence {
    std::size_t n;
    std::unique_ptr<float[]> sequence;

    explicit DiscreteTimeSequence(std::size_t n)
        : n(n), sequence(std::make_unique<float[]>(n)) {}

    float& operator[](std::size_t i) { return sequence[i]; }
    const float& operator[](std::size_t i) const { return sequence[i]; }

    //Deep copy
    DiscreteTimeSequence(const DiscreteTimeSequence& rhs) : n(rhs.n), sequence(std::make_unique<float[]>(rhs.n)){
    	std::copy(rhs.sequence.get(), rhs.sequence.get()+n, this->sequence.get());
    }

    DiscreteTimeSequence& operator=(const DiscreteTimeSequence& rhs) {
    	DiscreteTimeSequence tmp(rhs);
    	std::swap(n,tmp.n);
    	std::swap(this->sequence,tmp.sequence);
    	return *this;
    }
    // std::move
    DiscreteTimeSequence(DiscreteTimeSequence&&) noexcept = default;
    DiscreteTimeSequence& operator=(DiscreteTimeSequence&&) noexcept = default;
};

DiscreteTimeSequence delayed_copy(const DiscreteTimeSequence& rhs, std::size_t delay) {
	DiscreteTimeSequence out(rhs.n + delay);
	std::copy(rhs.sequence.get(), rhs.sequence.get()+rhs.n, out.sequence.get() + delay);
	return out;
}

DiscreteTimeSequence unit_sample_sequence(std::size_t n) {
	DiscreteTimeSequence out(n);
	out[0] = 1;
	return out;
}