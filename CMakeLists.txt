cmake_minimum_required(VERSION 4.1)
project(PlaygroundCpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(q1_max_gain_streak src/questions/1_max_gain_streak.cpp)
add_executable(q2_price_divergence src/questions/2_price_divergence.cpp)
add_executable(q3_largest_palindrome_product src/questions/3_largest_palindrome_product.cpp)
add_executable(q4_volume_spike_window src/questions/4_volume_spike_window.cpp)
add_executable(q5_duplicate_order_id src/questions/5_duplicate_order_id.cpp)
add_executable(q6_best_bid_tracker src/questions/6_best_bid_tracker.cpp)
add_executable(q7_cheapest_active_bid src/questions/7_cheapest_active_bid.cpp)

add_executable(mini_project_order_book
        src/mini_project/main.cpp
        src/mini_project/order_book.cpp
)

add_executable(Assessments
        src/Assessments/Assessments.cpp
        src/Assessments/permutations.cpp
        src/Assessments/preprocess_data.cpp
        src/Assessments/shared_interest.cpp
)

add_executable(PracticeProblems
        src/PracticeProblems/PracticeProblems.cpp
        src/PracticeProblems/deep_graph_copy.cpp
        src/PracticeProblems/example.cpp
        src/PracticeProblems/parser.cpp
)

add_executable(cppPlayground src/cppPlayground.cpp)
