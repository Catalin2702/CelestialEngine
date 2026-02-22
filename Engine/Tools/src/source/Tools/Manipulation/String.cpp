//
// Created by Catalin Chirosca on 2026-02-21.
//

#include "Tools/Manipulation/String.hpp"

#include <algorithm>

namespace CE::Tools::Manipulation {

std::string ToLowerCase(const std::string& source) {
	std::string result = source;
	std::ranges::transform(result, result.begin(), [](const unsigned char c) {
		return std::tolower(c);
	});
	return result;
}

}