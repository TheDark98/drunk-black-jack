add_library(deck STATIC deck.cpp)

target_include_directories(deck PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(deck PRIVATE generator PUBLIC card)