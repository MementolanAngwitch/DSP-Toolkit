#pragma once
#include <cstddef>
#include <memory>
#include <cmath>
#include <cassert>
#include <span>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <complex>
#include <sequences.hpp>

namespace dsp::discrete_time {

// |a-b| <= atol + rtol * |b|
inline bool allclose(const DiscreteTimeSequence& a, const DiscreteTimeSequence& b, float atol = 1e-6, float rtol = 1e-7) {
	std::ptrdiff_t first = std::min(a.start(), b.start());
	std::ptrdiff_t last = std::max(a.end(), b.end());
}

}