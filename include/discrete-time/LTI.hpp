#pragma once
#include <cstddef>
#include <memory>
#include <cmath>
#include <vector>
#include <sequences.hpp>

namespace dsp::discrete_time {

// |a-b| <= atol + rtol * |b|
inline bool allclose(const DiscreteTimeSequence& a, const DiscreteTimeSequence& b, float atol = 1e-6, float rtol = 1e-7) {
	std::ptrdiff_t first = std::min(a.start(), b.start());
	std::ptrdiff_t last = std::max(a.end(), b.end());
	for (std::ptrdiff_t t = first; t < last; ++t) {
		if (std::abs(a(t) - b(t)) > atol + rtol * std::abs(b(t)) ) return false;
	}
	return true;
}

template<class System>
inline bool looks_linear(System T, const DiscreteTimeSequence& x1, const DiscreteTimeSequence& x2, float a, float b) {
	return allclose(T(a*x1 + b*x2), a*T(x1) + b*T(x2) );
}

template<class System> 
inline bool time_invariant(System T, const DiscreteTimeSequence& x, std::ptrdiff_t d) {
	return allclose(T(delayed_copy(x,d)), delayed_copy(T(x),d));
}

template<class System> 
inline bool is_LTI(System T, const DiscreteTimeSequence& x1, const DiscreteTimeSequence& x2, float a, float b, std::ptrdiff_t d) {
	return looks_linear(x1,x2,a,b) && time_invariant(T,x,d);
} 

inline double finite_difference(const std::vector<double>& b , const std::vector<double>& a, const std::vector<double>& x) { //x is n-dimensional flattened to 1d, b and a are 1d
	double sum =0.0;
	const std::size_t n = x.size();
	for (std::size_t i = 0; i < b.size(); ++i) {
		sum += b[i] * x[n-i];
	}
	for (std::size_t i = 0; i < a.size(); ++i) {
		sum -= a[i] * x[n-i];
	}
	return sum / a[0];
}

}