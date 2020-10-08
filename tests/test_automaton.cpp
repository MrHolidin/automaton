#include <gtest/gtest.h>
#include <automaton.h>
#include <iostream>

class AutomatonTestCase : public ::testing::Test {
public:
    automaton all{1, 0, std::vector<size_t>{0}, {'a', 'b'}};
    automaton nothing{1, 0, std::vector<size_t>{}, {'a', 'b'}};
    automaton cicle{3, 0, std::vector<size_t>{0, 1, 2}, {'a', 'b', 'c'}};
    automaton simpl_auto{7, 0, std::vector<size_t>{2}, {'a', 'b'}};
    

    automaton hard_auto{4, 0, std::vector<size_t>{0, 3}, {'a', 'b'}};
    automaton d_hard_auto;
    automaton d_inv_auto;


    void DeterminizationSetUp() {
    	for (size_t i = 0; i < 2; ++i)
    		for (size_t j = 0; j < 2; ++j) {
    			hard_auto.add_edge(2 * i + j, 2 * (1 - i) + j, 'a');
    			hard_auto.add_edge(2 * i + j, 2 * i + 1 - j, 'b');
    		}
    	d_hard_auto = hard_auto.get_equivalent_DKA().get_minimum_DKA();
    	d_inv_auto = d_hard_auto.get_automaton_for_addition();
    }

    void SetUp() {

    	all.add_edge(0, 0, 'a');
    	all.add_edge(0, 0, 'b'); 

    	nothing.add_edge(0, 0, 'a');
    	nothing.add_edge(0, 0, 'b');

    	cicle.add_edge(0, 1, 'a');
    	cicle.add_edge(1, 2, 'b');
    	cicle.add_edge(2, 0, 'c');
    	cicle.add_edge(2, 0, 'a'); 

    	simpl_auto.add_edge(0, 0, 'a');
    	simpl_auto.add_edge(0, 0, 'b');
    	simpl_auto.add_edge(0, 1, 'a');
    	simpl_auto.add_edge(1, 2, 'a');
    	simpl_auto.add_edge(1, 2, 'b');
    	simpl_auto.add_edge(3, 4, 'b');

    	DeterminizationSetUp();

    }
};

TEST_F(AutomatonTestCase, SimpleAutoTest) {

    EXPECT_TRUE(all.valid_string("aaaaa"));
    EXPECT_TRUE(all.valid_string("ababa"));
    EXPECT_TRUE(all.valid_string(""));

    EXPECT_FALSE(nothing.valid_string("aaaaa"));
    EXPECT_FALSE(nothing.valid_string("ababa"));
    EXPECT_FALSE(nothing.valid_string(""));

    EXPECT_EQ(cicle.get_number_of_vertices(), 3);
    EXPECT_TRUE(cicle.valid_string("aba"));
    EXPECT_TRUE(cicle.valid_string("abc"));
    EXPECT_FALSE(cicle.valid_string("abb"));

    EXPECT_TRUE(simpl_auto.valid_string("bab"));
    EXPECT_FALSE(simpl_auto.valid_string("bbbb"));
    EXPECT_EQ(simpl_auto.get_equivalent_DKA().get_number_of_vertices(), 4);
}

TEST_F(AutomatonTestCase, DeterminizationAutoTest) {

    EXPECT_TRUE(hard_auto.valid_string("ab"));
    EXPECT_TRUE(d_hard_auto.valid_string("ab"));
    EXPECT_FALSE(hard_auto.valid_string("abaaa"));
    EXPECT_FALSE(d_hard_auto.valid_string("abaaa"));

    EXPECT_EQ(d_hard_auto.get_number_of_vertices(), 2);

    EXPECT_TRUE(d_inv_auto.valid_string("abaaa"));
    EXPECT_FALSE(d_inv_auto.valid_string("ab"));
    
}