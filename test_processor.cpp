#define CATCH_CONFIG_MAIN
#include "catch.h"
#include <sstream>

#include "PGMImageProcessor.h"
#include "PGMMetadata.h"


TEST_CASE( "No Initial Components" ) {
	PLZERI001::PGMImageProcessor p(PLZERI001::PGMMetadata(10, 10));
	REQUIRE(p.getComponentCount() == 0);
}
TEST_CASE( "Copied Processor References New Data" ) {
	PLZERI001::PGMImageProcessor p1(PLZERI001::PGMMetadata(10, 10));
	PLZERI001::PGMImageProcessor p2(p1);  // Copy contructor
	PLZERI001::PGMImageProcessor p3 = p2;  // Copy assignment
	std::stringstream(std::string(100, (char) 0)) >> p1;
	std::stringstream(std::string(100, (char) 255)) >> p2;
	// Make sure the copied components extract different components
	// now that their data has been replaced.
	REQUIRE(p1.extractComponents(100) == 0);
	REQUIRE(p2.extractComponents(100) == 1);
	REQUIRE(p3.getComponentCount() == 0);
}
TEST_CASE( "Move Processor Moves Data" ) {
	// Test move assignment
	PLZERI001::PGMImageProcessor p1(PLZERI001::PGMMetadata(10, 10));
	std::stringstream(std::string(100, (char) 255)) >> p1; // Pure white 10x10 image
	REQUIRE(p1.extractComponents(100, 5) == 1);
	PLZERI001::PGMImageProcessor p2 = std::move(p1);
	REQUIRE(p1.getComponentCount() == 0);  // Robbed component has its component count reset
	REQUIRE(p2.getComponentCount() == 1);  // New component has taken ownership of data
	// Not much to test for the move constructor. The
	// PGMImageProcessor isn't initialized with much data - it's
	// read in with the >> operator, so the move constructor doesn't
	// have to do much
	PLZERI001::PGMMetadata mdata(10, 10);
	PLZERI001::PGMImageProcessor p3(PLZERI001::PGMImageProcessor(mdata));
}
TEST_CASE( "Test Min/Max filtering & component sizes" ) {
	PLZERI001::PGMImageProcessor p(PLZERI001::PGMMetadata(10, 10));
	// Image something like this (+ is 255, o is 0):
	// +++++ooooo
	// ooooo+++++
	// ++++++++++
	// 0000000000
	// ++++++++++ x6
	std::stringstream(
		std::string(5, (char) 255) +
		std::string(10, (char) 0) +
		std::string(15, (char) 255) +
		std::string(10, (char) 0) +
		std::string(60, (char) 255)
		) >> p;
	// Extract with no initial min size, all components should be present
	REQUIRE(p.extractComponents(100) == 3);
	REQUIRE(p.getLargestSize() == 60);
	REQUIRE(p.getSmallestSize() == 5);
	p.filterComponentsBySize(6, 20);
	// Only one component between 6 and 20 pixels
	REQUIRE(p.getComponentCount() == 1);
}
TEST_CASE( "Test Boundary" ) {
	// 'window' shape with four equal quadrants, each 5x5 pixels
	// not the nicest way of constructing this!
	PLZERI001::PGMImageProcessor p(PLZERI001::PGMMetadata(11, 11));
	std::stringstream s;
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s << std::string(11, (char) 0);
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s << std::string(5, (char) 255) + std::string(1, (char) 0) + std::string(5, (char) 255);
	s >> p;
	p.extractComponents(100);
	REQUIRE( p.begin()->getBoundarySize() == 20 );
}
TEST_CASE( "Test Print Output" ) {
	PLZERI001::ConnectedComponent::currentid = 0;
	PLZERI001::PGMImageProcessor p(PLZERI001::PGMMetadata(10, 10));
	std::stringstream(std::string(100, (char) 255)) >> p; // Pure white 10x10 image
	p.extractComponents(100);
	const PLZERI001::ConnectedComponent component = *p.begin();
	REQUIRE( p.begin()->to_string() == "ConnectedComponent 1: 100 pixels (40 boundary)" );
}