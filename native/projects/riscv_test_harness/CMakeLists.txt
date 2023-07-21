add_executable(rvtest
	main.cpp
)

target_link_libraries(rvtest PUBLIC
	riscv::riscv
)
