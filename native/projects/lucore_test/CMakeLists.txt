# this is a temporary project to test lucore functionality while it's in development

add_executable(lucore_test
	main.cpp
)

target_link_libraries(lucore_test PUBLIC
	lucore::lucore)
