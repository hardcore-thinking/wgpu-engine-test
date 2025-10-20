#include <iostream>
#include <sstream>

#include <snitch/snitch.hpp>

#include <Math/Vector2.hpp>

// MARK: Size
TEST_CASE("Math::Vector2 is always 8 bytes in size", "[vector-size]") {
	REQUIRE(sizeof(Math::Vector2) == 8);
}

// MARK: Constructors
TEST_CASE("Constructing a vector", "[vector-construction]") {
	SECTION("Default constructing", "[construction-default]") {
		Math::Vector2 vec;
		REQUIRE(vec.x == 0.0f && vec.y == 0.0f);
	};

	SECTION("Constructing with common component", "[construction-common-component]") {
		Math::Vector2 vec(2.0f);
		INFO("c = 2.0");
		REQUIRE(vec.x == 2.0f && vec.y == 2.0f);
	}

	SECTION("Constructing with different component", "[construction-different-components]") {
		Math::Vector2 vec(3.0f, -2.0f);
		INFO("x =  3.0");
		INFO("y = -2.0");
		REQUIRE(vec.x == 3.0f && vec.y == -2.0f);
	}
}

// MARK: Comparisons
TEST_CASE("Comparison operations of a vector", "[vector-comparison]") {
	Math::Vector2 vec1(-3.0f, 1.0f);
	Math::Vector2 vec2(4.0f, 4.0f);

	INFO("vec1 = [-3.0,  1.0]");
	INFO("vec2 = [ 4.0,  4.0]");

	SECTION("Check equality of two vectors", "[comparison-equality]") {
		REQUIRE(vec1 == Math::Vector2(-3.0f, 1.0));
	}

	SECTION("Check difference of two vectors", "[comparison-difference]") {
		REQUIRE(vec1 != vec2);
	}
}

// MARK: Arithmetic
TEST_CASE("Arithmetic operations of a vector", "[vector-arithmetic]") {
	float scalar = 2.0f;
	Math::Vector2 vec1(2.0f, -7.0f);
	Math::Vector2 vec2(-5.0f, 3.0f);

	INFO("scalar = 2.0");
	INFO("vec1 = [ 2.0, -7.0]");
	INFO("vec2 = [-5.0,  3.0]");

	SECTION("Summing two vectors", "[arithmetic-sum]") {
		Math::Vector2 result = vec1 + vec2;
		INFO("result = vec1 + vec2 = [-3.0, -4.0]");
		REQUIRE(result.x == -3.0f && result.y == -4.0f);
	}

	SECTION("Negating a vector", "[arithmetic-negate]") {
		Math::Vector2 result = -vec1;
		INFO("result = -vec1 = [-2.0, 7.0]");
		REQUIRE(result.x == -2.0f && result.y == 7.0f);
	}

	SECTION("Subtracting two vectors", "[arithmetic-subtract]") {
		Math::Vector2 result = vec1 - vec2;
		INFO("result = vec1 - vec2 = [7.0, -10.0]");
		REQUIRE(result.x == 7.0f && result.y == -10.0f);
	}

	SECTION("Multiply scalar by vector", "[arithmetic-multiply-scalar-vector]") {
		Math::Vector2 result = scalar * vec1;
		INFO("order scalar by vector");
		INFO("result = scalar * vec1 = [4.0, -14.0]");
		REQUIRE(result.x == 4.0f && result.y == -14.0f);
	}

	SECTION("Multiply vector by scalar", "[arithmetic-multiply-vector-scalar]") {
		Math::Vector2 result = vec1 * scalar;
		INFO("order vector by scalar");
		INFO("result = vec1 * scalar = [4.0, -14.0]");
		REQUIRE(result.x == 4.0f && result.y == -14.0f);
	}

	SECTION("Divide vector by scalar", "[arithmetic-divide]") {
		Math::Vector2 result = vec1 / scalar;
		INFO("result = vec1 / scalar = [1.0, -3.5]");
		REQUIRE(result.x == 1.0f && result.y == -3.5f);
	}
}

// MARK: Assignment
TEST_CASE("Assignment operations of a vector", "[vector-assignment]") {
	float scalar = 2.0f;
	Math::Vector2 vec1(2.0f, -7.0f);
	Math::Vector2 vec2(-5.0f, 3.0f);

	INFO("scalar = 2.0");
	INFO("vec1 = [ 2.0, -7.0]");
	INFO("vec2 = [-5.0,  3.0]");

	SECTION("Assigning sum of two vectors", "[assignment-sum]") {
		Math::Vector2 result = vec1;
		result += vec2;
		INFO("result = vec1 + vec2 = [-3.0, -4.0]");
		REQUIRE(result.x == -3.0f && result.y == -4.0f);
	}

	SECTION("Assigning subtraction of two vectors", "[assignment-subtract]") {
		Math::Vector2 result = vec1;
		result -= vec2;
		INFO("result = vec1 - vec2 = [7.0, -10.0]");
		REQUIRE(result.x == 7.0f && result.y == -10.0f);
	}

	SECTION("Assigning multiplication of a scalar to a vector", "[assignment-multiply-scalar-vector]") {
		Math::Vector2 result = vec1;
		result *= scalar;
		INFO("result = scalar * vec1 = [4.0, -14.0]");
		REQUIRE(result.x == 4.0f && result.y == -14.0f);
	}

	SECTION("Assigning division of vector by a scalar", "[assignment-divide]") {
		Math::Vector2 result = vec1;
		result /= scalar;
		INFO("result = vec1 / scalar = [1.0, -3.5]");
		REQUIRE(result.x == 1.0f && result.y == -3.5f);
	}
}

// MARK: Stream
TEST_CASE("Stream operations of a vector", "[vector-stream]") {
	Math::Vector2 vec(1.0f, -9.0f);

	INFO("vec = [1.0, -9.0]");

	std::stringstream output;
	output << vec;

	REQUIRE(output.str() == "[1.000, -9.000]");
}

// MARK: Basic operations
TEST_CASE("Basic operations of a vector", "[vector-basic]") {
	Math::Vector2 vec1(2.0f, 6.0f);
	Math::Vector2 vec2(-3.0, 4.0f);

	INFO("vec1 = [ 2.0, 6.0]");
	INFO("vec2 = [-3.0, 4.0]");

	SECTION("Normalization operation on a vector", "[vector-normalization]") {
		Math::Vector2 result = Math::Vector2::Normalize(vec1);
		INFO("result = [0.316, 0.949]");
		REQUIRE(std::round(result.x * 1000.0f) / 1000.0f == 0.316f);
		REQUIRE(std::round(result.y * 1000.0f) / 1000.0f == 0.949f);
	}

	SECTION("Magnitude operation on a vector", "[vector-magnitude]") {
		REQUIRE(std::round(Math::Vector2::Magnitude(vec1) * 1000.0f) / 1000.0f == 6.325f);
	}

	SECTION("Dot operation on two vectors", "[vector-dot]") {
		REQUIRE(Math::Vector2::Dot(vec1, vec2) == 18.0f);
	}
}
